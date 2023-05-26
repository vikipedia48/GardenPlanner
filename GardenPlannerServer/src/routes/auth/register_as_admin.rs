use sqlx::types::Uuid;
use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::{CookieJar, cookie::Cookie};
use crate::{utils, models};

#[derive(Debug, serde::Serialize, serde::Deserialize)]
#[serde(rename_all = "snake_case")]
pub struct Data {
    pub username: String,
	pub password: String,
}

pub async fn register_as_admin(
    jar: CookieJar,
    State(state): State<AppState>,
    Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.auth::register_as_admin() | {} | attempting to register user '{}' as admin", trace_id, data.username);

    // check if already logged in
    let session_with_user = match utils::validate_session_with_user("garden_planner.auth::register_as_admin()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};
    
    if !session_with_user.user.is_admin {
		let msg = "cannot create an admin user because you are not an admin";
		log::error!("garden_planner.auth::register_as_admin() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
    }
    
    // hash the password
    let hashed_password = match bcrypt::hash(data.password, 4) {
        Ok(hash) => hash,
        Err(err) => {
            log::error!("garden_planner.auth::register_as_admin() | {} | bcrypt hash error: {}", trace_id, err);
            return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
        },
    };

    let user = match sqlx::query!(r#"SELECT "garden_planner".create_user($1, $2, true) as "user!: models::User""#, data.username, hashed_password).fetch_one(&state.conn).await {
        Ok(user) => user.user,
        Err(err) => {
            let msg = "user already exists";
            log::error!("garden_planner.auth::register_as_admin() | {} | {}\nfrom db: {}", trace_id, msg, err);
            return (StatusCode::CONFLICT, msg).into_response();
        },
    };

    // create session for user
    let session = match sqlx::query!(r#"SELECT "garden_planner".create_session($1) as "session!: models::Session""#, user.id).fetch_one(&state.conn).await {
        Ok(session) => session.session,
        Err(err) => {
            let msg = "failed to create session";
            log::error!("garden_planner.auth::register_as_admin() | {} | {}\nfrom db: {}", trace_id, msg, err);
            return (StatusCode::INTERNAL_SERVER_ERROR, msg).into_response();
        },
    };

    let mut encode_buf = Uuid::encode_buffer();
    let session_id = session.id.as_hyphenated().encode_lower(&mut encode_buf).to_string();
    let jar = jar.add(Cookie::new("gardenSessionId", session_id));
    let res = models::PasswordlessUser::from(user);

	log::debug!("garden_planner.auth::register_as_admin() | {} | registered user as an admin: {:?}", trace_id, res);
    log::info!("garden_planner.auth::register_as_admin() | {} | successfully registered as admin", trace_id);
    (jar, Json::<models::PasswordlessUser>(res)).into_response()
}

use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use sqlx::types::Uuid;
use axum_extra::extract::{CookieJar, cookie::Cookie};
use crate::models;

#[derive(Debug, serde::Serialize, serde::Deserialize)]
#[serde(rename_all = "snake_case")]
pub struct Data {
    pub username: String,
	pub password: String,
}

pub async fn login(
    jar: CookieJar,
    State(state): State<AppState>,
    Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    
    log::info!("garden_planner.auth::login() | {} | attempting to login as user '{}'", trace_id, data.username);

    // check if already logged in
    if let Some(session_id) = jar.get("gardenSessionId") {
        if let Ok(session_id) = Uuid::try_parse(session_id.value()) {
            if let Ok(session_with_user) = sqlx::query!(r#"SELECT "garden_planner".validate_session_with_user($1) as "session_with_user!: models::SessionWithUser""#, session_id).fetch_one(&state.conn).await {
                let session_with_user = session_with_user.session_with_user;
                log::info!("garden_planner.auth::login() | {} | already logged in", trace_id);
                return (jar, Json::<models::User>(session_with_user.user)).into_response();
            }
        }
    }

    let user = match sqlx::query!(r#"SELECT "garden_planner".get_user_by_name($1) as "user!: models::User""#, data.username).fetch_one(&state.conn).await {
        Ok(user) => user.user,
        Err(err) => {
            let msg = "failed to get user";
            log::error!("garden_planner.auth::login() | {} | {}\nfrom db: {}", trace_id, msg, err);
            return (StatusCode::NOT_FOUND, msg).into_response();
        },
    };

    // verify the password
    match bcrypt::verify(data.password, &user.password) {
        Ok(true) => {
            log::info!("garden_planner.auth::login() | {} | password matches", trace_id)
        },
        Ok(false) => {
            let msg = "password mismatch";
            log::info!("garden_planner.auth::login() | {} | {}", trace_id, msg);
            return (StatusCode::UNAUTHORIZED, msg).into_response();
        },
        Err(err) => {
            log::error!("garden_planner.auth::login() | {} | bcrypt verify error: {}", trace_id, err);
            return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
        },
    }

    // create new session for user
    let session = match sqlx::query!(r#"SELECT "garden_planner".create_session($1) as "session!: models::Session""#, user.id).fetch_one(&state.conn).await {
        Ok(session) => session.session,
        Err(err) => {
            let msg = "failed to create session";
            log::error!("garden_planner.auth::login() | {} | {}\nfrom db: {}", trace_id, msg, err);
            return (StatusCode::INTERNAL_SERVER_ERROR, msg).into_response();
        },
    };

    let mut encode_buf = Uuid::encode_buffer();
    let session_id = session.id.as_hyphenated().encode_lower(&mut encode_buf).to_string();
    let jar = jar.add(Cookie::new("gardenSessionId", session_id));
    let res = models::PasswordlessUser::from(user);

	log::debug!("garden_planner.auth::login() | {} | logged in user: {:?}", trace_id, res);
    log::info!("garden_planner.auth::login() | {} | successfully logged in", trace_id);
    (jar, Json::<models::PasswordlessUser>(res)).into_response()
}

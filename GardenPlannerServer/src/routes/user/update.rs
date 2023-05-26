use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::utils;

#[derive(Debug, Clone, PartialEq, Eq, serde::Deserialize, serde::Serialize)]
pub struct Data {
	pub username: String,
	pub custom_plants_hash: String,
}

pub async fn update(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(user_id): Path<i64>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.user::update() | {} | attempting to update user '{}'", trace_id, user_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.user::update()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_user($1, $2) as "ok!: bool""#, user_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.user::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot update the user because you are not an admin or the owner";
		log::error!("garden_planner.user::update() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	match sqlx::query!(r#"SELECT "garden_planner".update_user($1, $2)"#, user_id, data.username).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.user::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    log::info!("garden_planner.user::update() | {} | successfully updated user", trace_id);
    (StatusCode::OK).into_response()
}

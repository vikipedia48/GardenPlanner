use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::utils;

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub name: String,
	pub plants: Vec<u8>,
}

pub async fn update(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(worker_id): Path<i64>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.worker::update() | {} | attempting to update worker '{}'", trace_id, worker_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.worker::update()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_worker($1, $2) as "ok!: bool""#, worker_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.worker::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot update the worker because you are not an admin or the owner";
		log::error!("garden_planner.worker::update() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	match sqlx::query!(r#"SELECT "garden_planner".update_worker($1, $2)"#, worker_id, data.name).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.worker::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    log::info!("garden_planner.worker::update() | {} | successfully updated worker", trace_id);
    (StatusCode::OK).into_response()
}

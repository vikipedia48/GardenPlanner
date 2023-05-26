use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub name: String,
	pub user_id: i64,
}

pub async fn create(
    jar: CookieJar,
    State(state): State<AppState>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.worker::create() | {} | attempting to create worker: {:?}", trace_id, data);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.worker::create()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_user($1, $2) as "ok!: bool""#, data.user_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.worker::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot create the worker because you are not an admin or the owner";
		log::error!("garden_planner.worker::create() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let worker = match sqlx::query!(r#"SELECT "garden_planner".create_worker($1, $2) as "worker!: models::Worker""#, data.name, data.user_id).fetch_one(&state.conn).await {
		Ok(worker) => worker.worker,
		Err(err) => {
			log::error!("garden_planner.worker::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.worker::create() | {} | created worker: {:?}", trace_id, worker);
    log::info!("garden_planner.worker::create() | {} | successfully created worker", trace_id);
    (Json::<models::Worker>(worker)).into_response()
}

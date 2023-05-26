use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get_all(
    jar: CookieJar,
    State(state): State<AppState>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.worker::get_all() | {} | attempting to get all workers", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.worker::get_all()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	if !session_with_user.user.is_admin {
		let msg = "cannot view these workers because you are not an admin";
		log::error!("garden_planner.worker::get_all() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let workers = match sqlx::query!(r#"SELECT "garden_planner".get_all_workers() as "worker!: models::Worker""#).fetch_all(&state.conn).await {
		Ok(workers) => workers.into_iter().map(|record| record.worker).collect::<Vec<models::Worker>>(),
		Err(err) => {
			log::error!("garden_planner.worker::get_all() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.worker::get_all() | {} | got workers: {:?}", trace_id, workers);
    log::info!("garden_planner.worker::get_all() | {} | successfully queried workers", trace_id);
    (Json::<Vec<models::Worker>>(workers)).into_response()
}

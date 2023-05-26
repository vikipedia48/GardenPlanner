use crate::state::AppState;
use axum::{
    extract::{Path, State},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(worker_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.worker::get() | {} | attempting to get worker '{}'", trace_id, worker_id);
    
	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.worker::get()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};
	
	let worker = match sqlx::query!(r#"SELECT "garden_planner".get_worker($1) as "worker!: models::Worker""#, worker_id).fetch_one(&state.conn).await {
		Ok(worker) => worker.worker,
		Err(err) => {
			let msg = "no such worker exists";
			log::error!("garden_planner.worker::get() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};

	if !session_with_user.user.is_admin && worker.user_id != session_with_user.user.id {
		let msg = "cannot get the worker because you are not the owner or the admin";
		log::error!("garden_planner.worker::get() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}

    log::info!("garden_planner.worker::get() | {} | successfully queried worker", trace_id);
    (Json::<models::Worker>(worker)).into_response()
}

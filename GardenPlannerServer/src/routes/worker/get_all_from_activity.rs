use crate::state::AppState;
use axum::{
    extract::{Path, State},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get_all_from_activity(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(activity_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.worker::get_all_from_activity() | {} | attempting to get all workers from activity '{}'", trace_id, activity_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.worker::get_all_from_activity()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_activity($1, $2) as "ok!: bool""#, activity_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.worker::get_all_from_activity() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the activity's workers because you are not an admin or the owner";
		log::error!("garden_planner.worker::get_all_from_activity() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let workers = match sqlx::query!(r#"SELECT "garden_planner".get_all_workers_from_activity($1) as "worker!: models::Worker""#, activity_id).fetch_all(&state.conn).await {
		Ok(workers) => workers.into_iter().map(|record| record.worker).collect::<Vec<models::Worker>>(),
		Err(err) => {
			let msg = "no such activity exists";
			log::error!("garden_planner.worker::get_all_from_activity() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};

	log::debug!("garden_planner.worker::get_all_from_activity() | {} | got workers: {:?}", trace_id, workers);
    log::info!("garden_planner.worker::get_all_from_activity() | {} | successfully queried workers", trace_id);
    (Json::<Vec<models::Worker>>(workers)).into_response()
}

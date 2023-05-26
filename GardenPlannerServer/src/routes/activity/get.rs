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
	Path(activity_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.activity::get() | {} | attempting to get activity '{}'", trace_id, activity_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.activity::get()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_activity($1, $2) as "ok!: bool""#, activity_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.activity::get() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the activity because you are not an admin or the owner";
		log::error!("garden_planner.activity::get() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let activity = match sqlx::query!(r#"SELECT "garden_planner".get_activity($1) as "activity!: models::Activity""#, activity_id).fetch_one(&state.conn).await {
		Ok(activity) => activity.activity,
		Err(err) => {
			let msg = "no such activity exists";
			log::error!("garden_planner.activity::get() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};

	log::debug!("garden_planner.activity::get() | {} | got activity: {:?}", trace_id, activity);
    log::info!("garden_planner.activity::get() | {} | successfully queried activity", trace_id);
    (Json::<models::Activity>(activity)).into_response()
}

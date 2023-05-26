use crate::state::AppState;
use axum::{
    extract::{Path, State},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get_all_from_garden(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(garden_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.activity::get_all_from_garden() | {} | attempting to get all activities from garden '{}'", trace_id, garden_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::get_all_from_garden()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_garden($1, $2) as "ok!: bool""#, garden_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.activity::get_all_from_garden() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the garden's activities because you are not an admin or the owner";
		log::error!("garden_planner.activity::get_all_from_garden() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let activities = match sqlx::query!(r#"SELECT "garden_planner".get_all_activities_from_garden($1) as "activity!: models::Activity""#, garden_id).fetch_all(&state.conn).await {
		Ok(activities) => activities.into_iter().map(|record| record.activity).collect::<Vec<models::Activity>>(),
		Err(err) => {
			let msg = "no such garden exists";
			log::error!("garden_planner.activity::get_all_from_garden() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};

	log::debug!("garden_planner.activity::get_all_from_garden() | {} | got activities: {:?}", trace_id, activities);
    log::info!("garden_planner.activity::get_all_from_garden() | {} | successfully queried activities", trace_id);
    (Json::<Vec<models::Activity>>(activities)).into_response()
}

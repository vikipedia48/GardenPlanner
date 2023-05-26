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
    log::info!("garden_planner.activity::get_all() | {} | attempting to get all gardens", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.activity::get_all()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	if !session_with_user.user.is_admin {
		let msg = "cannot get all activities because you are not an admin";
		log::error!("garden_planner.activity::get_all() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let activities = match sqlx::query!(r#"SELECT "garden_planner".get_all_activities() as "activity!: models::Activity""#).fetch_all(&state.conn).await {
		Ok(activities) => activities.into_iter().map(|record| record.activity).collect::<Vec<models::Activity>>(),
		Err(err) => {
			log::error!("garden_planner.activity::get_all() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.activity::get_all() | {} | got activities: {:?}", trace_id, activities);
    log::info!("garden_planner.activity::get_all() | {} | successfully queried activities", trace_id);
    (Json::<Vec<models::Activity>>(activities)).into_response()
}

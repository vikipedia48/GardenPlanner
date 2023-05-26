use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
};
use axum_extra::extract::CookieJar;
use crate::utils;

pub async fn delete(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(activity_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.activity::delete() | {} | attempting to delete activity '{}'", trace_id, activity_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.activity::delete()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_activity($1, $2) as "ok!: bool""#, activity_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.activity::delete() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot delete the activity because you are not an admin or the owner";
		log::error!("garden_planner.activity::delete() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}

	match sqlx::query!(r#"SELECT "garden_planner".delete_activity($1)"#, activity_id).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.activity::delete() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    log::info!("garden_planner.activity::delete() | {} | successfully deleted activity", trace_id);
	(StatusCode::OK).into_response()
}

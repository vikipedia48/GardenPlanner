use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(user_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.user::get() | {} | attempting to get user '{}'", trace_id, user_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.user::get()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_user($1, $2) as "ok!: bool""#, user_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.user::get() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the user because you are not an admin or the owner";
		log::error!("garden_planner.user::get() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let user = match sqlx::query!(r#"SELECT "garden_planner".get_user($1) as "user!: models::User""#, user_id).fetch_one(&state.conn).await {
		Ok(user) => user.user,
		Err(err) => {
			log::error!("garden_planner.user::get() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.user::get() | {} | got user: {:?}", trace_id, user);
    log::info!("garden_planner.user::get() | {} | successfully queried user", trace_id);
    (Json::<models::PasswordlessUser>(models::PasswordlessUser::from(user))).into_response()
}

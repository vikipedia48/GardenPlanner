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
    log::info!("garden_planner.user::get_all() | {} | attempting to get all gardens", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.user::get_all()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};
	
	if !session_with_user.user.is_admin {
		let msg = "cannot get all users because you are not an admin";
		log::error!("garden_planner.user::get_all() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let users = match sqlx::query!(r#"SELECT "garden_planner".get_all_users() as "user!: models::User""#).fetch_all(&state.conn).await {
		Ok(users) => users.into_iter().map(|record| record.user).collect::<Vec<models::User>>(),
		Err(err) => {
			log::error!("garden_planner.user::get_all() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.user::get_all() | {} | got users: {:?}", trace_id, users);
    log::info!("garden_planner.user::get_all() | {} | successfully queried users", trace_id);
    (Json::<Vec<models::PasswordlessUser>>(users.into_iter().map(Into::into).collect())).into_response()
}

use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
};
use axum_extra::extract::{CookieJar, cookie::Cookie};
use crate::utils;

pub async fn logout(
    jar: CookieJar,
    State(state): State<AppState>,
) -> Response {
    let trace_id = trace_id::generate_id();
    
    log::info!("garden_planner.auth::logout() | {} | attempting to logout user", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.auth::logout()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

    match sqlx::query!(r#"SELECT "garden_planner".delete_session($1)"#, session_with_user.session.id).fetch_one(&state.conn).await {
        Ok(_) => {},
        Err(err) => {
            let msg = "failed to delete session";
            log::error!("garden_planner.auth::logout() | {} | {}\nfrom db: {}", trace_id, msg, err);
            return (StatusCode::NOT_FOUND, msg).into_response();
        },
    };

    let jar = jar.remove(Cookie::named("gardenSessionId"));

    log::info!("garden_planner.auth::logout() | {} | successfully logged out", trace_id);
    (jar).into_response()
}

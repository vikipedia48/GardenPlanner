use axum::{
	response::{Response, IntoResponse},
    http::StatusCode,
};
use axum_extra::extract::CookieJar;
use uuid::Uuid;
use crate::{
	state::AppState, 
	models::{self, SessionWithUser},
};


pub async fn validate_session(origin: &str, trace_id: &str, jar: &CookieJar, state: &AppState) -> Result<models::Session, Response> {
	let session_id = validate_cookie(origin, trace_id, jar)?;

	let session = match sqlx::query!(r#"SELECT "garden_planner".validate_session($1) as "session!: models::Session""#, session_id).fetch_one(&state.conn).await {
		Ok(session) => session.session,
		Err(err) => {
			let msg = "invalid session id";
			log::error!("{} | {} | {}\nfrom db: {}", origin, trace_id, msg, err);
			return Err((StatusCode::UNAUTHORIZED, msg).into_response());
		},
	};
	
	Ok(session)
}

pub async fn validate_session_with_user(origin: &str, trace_id: &str, jar: &CookieJar, state: &AppState) -> Result<SessionWithUser, Response> {
	let session_id = validate_cookie(origin, trace_id, jar)?;

	let session_with_user = match sqlx::query!(r#"SELECT "garden_planner".validate_session_with_user($1) as "session_with_user!: models::SessionWithUser""#, session_id).fetch_one(&state.conn).await {
		Ok(session_with_user) => session_with_user.session_with_user,
		Err(err) => {
			let msg = "invalid session id";
			log::error!("{} | {} | {}\nfrom db: {}", origin, trace_id, msg, err);
			return Err((StatusCode::UNAUTHORIZED, msg).into_response());
		},
	};
	
	Ok(SessionWithUser { session: session_with_user.session, user: session_with_user.user })
}

fn validate_cookie(origin: &str, trace_id: &str, jar: &CookieJar) -> Result<Uuid, Response> {
	let session_id = match jar.get("gardenSessionId") {
        Some(session_id) => session_id,
        None => {
			let msg = "not logged in";
			log::error!("{} | {} | {}", origin, trace_id, msg);
			return Err((StatusCode::UNAUTHORIZED, msg).into_response());
		},
    };
	
	let session_id = match Uuid::try_parse(session_id.value()) {
		Ok(session_id) => session_id,
		Err(err) => {
			let msg = "invalid session id";
			log::error!("{} | {} | {}\nfrom db: {}", origin, trace_id, msg, err);
			return Err((StatusCode::UNAUTHORIZED, msg).into_response());
		},
	};

	Ok(session_id)
}
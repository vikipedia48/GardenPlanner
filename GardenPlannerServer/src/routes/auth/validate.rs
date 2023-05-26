use sqlx::types::Uuid;
use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::{CookieJar, cookie::Cookie};
use crate::models;

pub async fn validate(
    jar: CookieJar,
    State(state): State<AppState>,
) -> Response {
    let trace_id = trace_id::generate_id();
    
    // check if session id is present in cookie
    let session_id = match jar.get("gardenSessionId") {
        Some(session_id) => session_id,
        None => {
            let msg = "no session id found in cookie";
            log::error!("garden_planner.auth::validate() | {} | {}", trace_id, msg);
            return (StatusCode::UNAUTHORIZED, msg).into_response();
        },
    };

    log::info!("garden_planner.auth::validate() | {} | validate request for session id '{}'", trace_id, session_id.value());

    // parse session id into uuid
    let session_id = match Uuid::try_parse(session_id.value()) {
        Ok(session_id) => session_id,
        Err(err) => {
            let msg = "invalid session id";
            log::error!("garden_planner.auth::validate() | {} | uuid parse error: {}", trace_id, err);
            return (StatusCode::UNAUTHORIZED, msg).into_response();
        },
    };

    // validate the session and return the session and user
    let session_with_user = match sqlx::query!(r#"SELECT "garden_planner".validate_session_with_user($1) as "session_with_user!: models::SessionWithUser""#, session_id).fetch_one(&state.conn).await {
        Ok(session_with_user) => session_with_user.session_with_user,
        Err(err) => {
            let msg = format!("{}", err);
            log::error!("garden_planner.auth::validate() | {} | failed to validate session\nfrom db: {}", trace_id, err);
            return (StatusCode::UNAUTHORIZED, msg).into_response();
        },
    };

    let mut encode_buf = Uuid::encode_buffer();
    let session_id = session_with_user.session.id.as_hyphenated().encode_lower(&mut encode_buf).to_string();
    let jar = jar.add(Cookie::new("gardenSessionId", session_id));

    log::info!("garden_planner.auth::validate() | {} | session validated", trace_id);
    (jar, Json::<models::PasswordlessUser>(models::PasswordlessUser::from(session_with_user.user))).into_response()
}

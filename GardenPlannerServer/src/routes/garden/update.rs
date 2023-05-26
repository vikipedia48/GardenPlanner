use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use base64::Engine;
use crate::utils;

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub name: String,
	pub plants: String,
}

pub async fn update(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(garden_id): Path<i64>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.garden::update() | {} | attempting to update garden '{}'", trace_id, garden_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::update()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_garden($1, $2) as "ok!: bool""#, garden_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.garden::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot update the garden because you are not an admin or the owner";
		log::error!("garden_planner.garden::update() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}

	let plants = match base64::engine::general_purpose::STANDARD.decode(&data.plants) {
		Ok(plants) => plants,
		Err(err) => {
			let msg = "invalid base64 plants";
			log::error!("garden_planner.garden::update() | {} | base64 error: {}", trace_id, err);
			return (StatusCode::BAD_REQUEST, msg).into_response();
		},
	};
	
	match sqlx::query!(r#"SELECT "garden_planner".update_garden($1, $2, $3)"#, garden_id, data.name, plants).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.garden::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    log::info!("garden_planner.garden::update() | {} | successfully updated garden", trace_id);
    (StatusCode::OK).into_response()
}

use crate::state::AppState;
use axum::{
    extract::{State, Path},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::utils;

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub plant_id: i32,
	pub description: String,
	pub duration: Option<i32>,
	pub repeat: Option<i32>,
	pub start_date: Option<chrono::NaiveDate>,
	pub assigned_workers: Vec<i64>,
}

pub async fn update(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(activity_id): Path<i64>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.activity::update() | {} | attempting to update activity '{}'", trace_id, activity_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.activity::update()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_activity($1, $2) as "ok!: bool""#, activity_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.activity::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot update the activity because you are not an admin or the owner";
		log::error!("garden_planner.activity::update() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	match sqlx::query!(r#"SELECT "garden_planner".update_activity($1, $2, $3, $4, $5, $6, $7)"#, activity_id, data.plant_id, data.description, data.duration.unwrap_or(1), data.repeat.unwrap_or(0), data.start_date, &data.assigned_workers).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.activity::update() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    log::info!("garden_planner.activity::update() | {} | successfully updated activity", trace_id);
    (StatusCode::OK).into_response()
}

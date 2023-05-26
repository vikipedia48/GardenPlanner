use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub garden_id: i64,
	pub plant_id: i32,
	pub description: String,
	pub duration: Option<i32>,
	pub repeat: Option<i32>,
	pub start_date: Option<chrono::NaiveDate>,
	pub assigned_workers: Vec<i64>,
}

pub async fn create(
    jar: CookieJar,
    State(state): State<AppState>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.activity::create() | {} | attempting to create activity: {:?}", trace_id, data);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.activity::create()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_garden($1, $2) as "ok!: bool""#, data.garden_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.activity::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot create the activity because you are not an admin or the owner of the garden";
		log::error!("garden_planner.activity::create() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let activity = match sqlx::query!(r#"SELECT "garden_planner".create_activity($1, $2, $3, $4, $5, $6, $7) as "activity!: models::Activity""#, data.garden_id, data.plant_id, data.description, data.duration.unwrap_or(1), data.repeat.unwrap_or(0), data.start_date, &data.assigned_workers).fetch_one(&state.conn).await {
		Ok(activity) => activity.activity,
		Err(err) => {
			log::error!("garden_planner.activity::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	log::debug!("garden_planner.activity::create() | {} | created activity: {:?}", trace_id, activity);
    log::info!("garden_planner.activity::create() | {} | successfully created activity", trace_id);
    (Json::<models::Activity>(activity)).into_response()
}

use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use base64::Engine;
use crate::{utils, models};

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Data {
	pub name: String,
	pub user_id: i64,
	pub plants: String,
}

pub async fn create(
    jar: CookieJar,
    State(state): State<AppState>,
	Json(data): Json<Data>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.garden::create() | {} | attempting to create garden: {:?}", trace_id, data);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::create()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_user($1, $2) as "ok!: bool""#, data.user_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.garden::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot create the garden because you are not an admin or the owner";
		log::error!("garden_planner.garden::create() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}

	let plants = match base64::engine::general_purpose::STANDARD.decode(&data.plants) {
		Ok(plants) => plants,
		Err(err) => {
			let msg = "invalid base64 plants";
			log::error!("garden_planner.garden::create() | {} | base64 error: {}", trace_id, err);
			return (StatusCode::BAD_REQUEST, msg).into_response();
		},
	};

	let garden = match sqlx::query!(r#"SELECT "garden_planner".create_garden($1, $2, $3) as "garden!: models::Garden""#, data.name, data.user_id, plants).fetch_one(&state.conn).await {
		Ok(garden) => garden.garden,
		Err(err) => {
			log::error!("garden_planner.garden::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};
	
	// converts plants to base64, otherwise the same as garden
	let res = models::GardenRes::from_garden(garden);

	log::debug!("garden_planner.garden::create() | {} | created garden: {:?}", trace_id, res);
    log::info!("garden_planner.garden::create() | {} | successfully created garden", trace_id);
    (Json::<models::GardenRes>(res)).into_response()
}

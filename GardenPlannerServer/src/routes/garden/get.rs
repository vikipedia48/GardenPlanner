use crate::state::AppState;
use axum::{
    extract::{Path, State},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(garden_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.garden::get() | {} | attempting to get garden '{}'", trace_id, garden_id);
    
	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::get()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_garden($1, $2) as "ok!: bool""#, garden_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.garden::get() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the garden because you are not an admin or the owner";
		log::error!("garden_planner.garden::get() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let garden = match sqlx::query!(r#"SELECT "garden_planner".get_garden($1) as "garden!: models::Garden""#, garden_id).fetch_one(&state.conn).await {
		Ok(garden) => garden.garden,
		Err(err) => {
			let msg = "no such garden exists";
			log::error!("garden_planner.garden::get() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};
	
	// converts plants to base64, otherwise the same as garden
	let res = models::GardenRes::from_garden(garden);

	log::debug!("garden_planner.garden::get() | {} | got garden: {:?}", trace_id, res);
    log::info!("garden_planner.garden::get() | {} | successfully queried garden", trace_id);
    (Json::<models::GardenRes>(res)).into_response()
}

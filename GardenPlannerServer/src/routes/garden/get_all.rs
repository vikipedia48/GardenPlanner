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
    log::info!("garden_planner.garden::get_all() | {} | attempting to get all gardens", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::get_all()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	if !session_with_user.user.is_admin {
		let msg = "cannot get all gardens because you are not an admin";
		log::error!("garden_planner.garden::get_all() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let gardens = match sqlx::query!(r#"SELECT "garden_planner".get_all_gardens() as "garden!: models::Garden""#).fetch_all(&state.conn).await {
		Ok(gardens) => gardens.into_iter().map(|record| record.garden).collect::<Vec<models::Garden>>(),
		Err(err) => {
			log::error!("garden_planner.garden::get_all() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	// converts plants to base64, otherwise the same as garden
	let res = gardens.into_iter().map(models::GardenRes::from_garden).collect::<Vec<_>>();

	log::debug!("garden_planner.garden::get_all() | {} | got gardens: {:?}", trace_id, res);
    log::info!("garden_planner.garden::get_all() | {} | successfully queried gardens", trace_id);
    (Json::<Vec<models::GardenRes>>(res)).into_response()
}

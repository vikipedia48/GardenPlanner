use crate::state::AppState;
use axum::{
    extract::{Path, State},
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn get_all_from_user(
    jar: CookieJar,
    State(state): State<AppState>,
	Path(user_id): Path<i64>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.garden::get_all_from_user() | {} | attempting to get all gardens from user '{}'", trace_id, user_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.garden::get_all_from_user()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let has_perm = match sqlx::query!(r#"SELECT "garden_planner".check_perm_for_user($1, $2) as "ok!: bool""#, user_id, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(ok) => ok.ok,
		Err(err) => {
			log::error!("garden_planner.garden::get_all_from_user() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	if !has_perm {
		let msg = "cannot get the user's gardens because you are not an admin or the owner";
		log::error!("garden_planner.garden::get_all_from_user() | {} | {}", trace_id, msg);
		return (StatusCode::UNAUTHORIZED, msg).into_response();
	}
	
	let gardens = match sqlx::query!(r#"SELECT "garden_planner".get_all_gardens_from_user($1) as "garden!: models::Garden""#, user_id).fetch_all(&state.conn).await {
		Ok(gardens) => gardens.into_iter().map(|record| record.garden).collect::<Vec<models::Garden>>(),
		Err(err) => {
			let msg = "no such user exists";
			log::error!("garden_planner.garden::get_all_from_user() | {} | {}\nfrom db: {}", trace_id, msg, err);
			return (StatusCode::NOT_FOUND, msg).into_response();
		},
	};
	
	// converts plants to base64, otherwise the same as garden
	let res = gardens.into_iter().map(models::GardenRes::from_garden).collect::<Vec<_>>();

	log::debug!("garden_planner.garden::get_all_from_user() | {} | got gardens: {:?}", trace_id, res);
    log::info!("garden_planner.garden::get_all_from_user() | {} | successfully queried gardens", trace_id);
    (Json::<Vec<models::GardenRes>>(res)).into_response()
}

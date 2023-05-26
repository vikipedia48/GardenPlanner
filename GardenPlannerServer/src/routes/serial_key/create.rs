use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
    Json, 
};
use axum_extra::extract::CookieJar;
use base64::Engine;
use rsa::{signature::{Keypair, Signer}, RsaPrivateKey, pkcs8::{LineEnding}, Pkcs1v15Sign, pkcs1::EncodeRsaPublicKey, pkcs1v15::SigningKey, sha2::Sha256};
use digest::Digest;
use uuid::Uuid;
use crate::{utils, models};

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Res {
	pub encrypted_serial_key: String,
}

pub async fn create(
    jar: CookieJar,
    State(state): State<AppState>,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.serial_key::create() | {} | attempting to create serial key", trace_id);

	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.serial_key::create()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};

	let serial_key = Uuid::new_v4();
	let private_key = match RsaPrivateKey::new(&mut rand::thread_rng(), 2048) {
		Ok(private_key) => private_key,
		Err(err) => {
			log::error!("garden_planner.serial_key::create() | {} | rsa private key creation error: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	let hashed_serial_key = utils::hash_with_sha256(serial_key.as_bytes());
	let encrypted_serial_key = match private_key.sign_with_rng(&mut rand::thread_rng(), Pkcs1v15Sign::new::<Sha256>(), &hashed_serial_key) {
		Ok(encrypted_serial_key) => encrypted_serial_key,
		Err(err) => {
			log::error!("garden_planner.serial_key::create() | {} | rsa signing error: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};
	let encrypted_serial_key = base64::engine::general_purpose::STANDARD.encode(&encrypted_serial_key);

	let public_key = private_key.to_public_key();
	let public_key_pem = match public_key.to_pkcs1_pem(LineEnding::LF) {
		Ok(public_key_pem) => public_key_pem,
		Err(err) => {
			log::error!("garden_planner.serial_key::create() | {} | rsa pem conversion error: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	match sqlx::query!(r#"SELECT "garden_planner".create_serial_key($1, $2, $3) as "serial_key!: models::SerialKey""#, session_with_user.user.id, serial_key, public_key_pem).fetch_one(&state.conn).await {
		Ok(_) => {},
		Err(err) => {
			log::error!("garden_planner.serial_key::create() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

	let res = Res {
		encrypted_serial_key,
	};

	log::debug!("garden_planner.serial_key::create() | {} | created serial key: {:?}", trace_id, res);
    log::info!("garden_planner.serial_key::create() | {} | successfully created serial key", trace_id);
    (Json::<Res>(res)).into_response()
}

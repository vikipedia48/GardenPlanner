use base64::Engine;
use rsa::{signature::Verifier, RsaPublicKey, pkcs1::DecodeRsaPublicKey, Pkcs1v15Sign, sha2::Sha256, pkcs1v15::{VerifyingKey, Signature}};
use digest::Digest;
use crate::state::AppState;
use axum::{
    extract::State,
    http::StatusCode,
    response::{Response, IntoResponse},
};
use axum_extra::extract::CookieJar;
use crate::{utils, models};

pub async fn validate(
    jar: CookieJar,
    State(state): State<AppState>,
    encrypted_serial_key: String,
) -> Response {
    let trace_id = trace_id::generate_id();
    log::info!("garden_planner.serial_key::validate() | {} | attempting to validate serial key", trace_id);
    
	// validate session and permissions
    let session_with_user = match utils::validate_session_with_user("garden_planner.serial_key::validate()", &trace_id, &jar, &state).await {
		Ok(session_with_user) => session_with_user,
		Err(res) => return res,
	};
    
	let serial_key = match sqlx::query!(r#"SELECT "garden_planner".get_serial_key_from_user($1) as "serial_key: models::SerialKey""#, session_with_user.user.id).fetch_one(&state.conn).await {
		Ok(serial_key) => match serial_key.serial_key {
            Some(serial_key) => serial_key,
            None => {
                let msg = "no serial key found for user";
                log::error!("garden_planner.serial_key::validate() | {} | {}", trace_id, msg);
                return (StatusCode::UNAUTHORIZED, msg).into_response();
            },
        },
		Err(err) => {
			log::error!("garden_planner.serial_key::validate() | {} | from db: {}", trace_id, err);
			return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
		},
	};

    let encrypted_serial_key = match base64::engine::general_purpose::STANDARD.decode(&encrypted_serial_key) {
        Ok(encrypted_serial_key) => encrypted_serial_key,
        Err(err) => {
            log::error!("garden_planner.serial_key::validate() | {} | base64 error: {}", trace_id, err);
            return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
        },
    };

    let public_key = match RsaPublicKey::from_pkcs1_pem(&serial_key.public_key) {
        Ok(public_key) => public_key,
        Err(err) => {
            log::error!("garden_planner.serial_key::validate() | {} | rsa from pem conversion error: {}", trace_id, err);
            return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
        },
    };

    let signature = match Signature::try_from(encrypted_serial_key.as_slice()) {
        Ok(signature) => signature,
        Err(err) => {
            log::error!("garden_planner.serial_key::validate() | {} | signature conversion error: {}", trace_id, err);
            return (StatusCode::INTERNAL_SERVER_ERROR).into_response();
        },
    };

    let verifying_key: VerifyingKey<Sha256> = VerifyingKey::<Sha256>::new(public_key);
    let status_code = match verifying_key.verify(serial_key.original_key.as_bytes(), &signature) {
        Ok(_) => {
            log::info!("garden_planner.serial_key::validate() | {} | serial key validated", trace_id);
            StatusCode::OK
        },
        Err(err) => {
            log::error!("garden_planner.serial_key::validate() | {} | {}", trace_id, err);
            log::error!("garden_planner.serial_key::validate() | {} | serial key invalid", trace_id);
            StatusCode::UNAUTHORIZED
        },
    };

    (status_code).into_response()
}

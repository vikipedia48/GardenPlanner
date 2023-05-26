pub mod utils;
pub mod routes;
pub mod state;
pub mod models;

use axum::{
    routing::{get, post, delete, patch},
    Router,
};
use logger::loggers::StdoutLogger;
use sqlx::{postgres::{PgPoolOptions, PgConnectOptions}, ConnectOptions};
use state::*;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    // Load the .env file
    dotenvy::dotenv()?;

    // Initialize the logger
    StdoutLogger::new().with_module_level("garden_planner", log::LevelFilter::Trace).init();
    log::info!("initialized logging");

    app().await
}

pub async fn app() -> anyhow::Result<()> {
    // Envars
    let auth_port = std::env::var("GARDEN_PLANNER_SERVER_PORT").unwrap_or("5001".into());
    let db_host = std::env::var("DATABASE_HOST")?;
    let db_port = std::env::var("DATABASE_PORT")?;
    let db_name = std::env::var("DATABASE_NAME")?;
    let db_user = std::env::var("DATABASE_USER")?;
    let db_pass = std::env::var("DATABASE_PASSWORD")?;

    // App init
    let mut opts = PgConnectOptions::new()
        .host(&db_host)
        .port(db_port.parse::<u16>().expect("invalid database port"))
        .database(&db_name)
        .username(&db_user)
        .password(&db_pass);
    opts.disable_statement_logging();
    let conn = match PgPoolOptions::new().connect_with(opts).await {
        Ok(conn) => conn,
        Err(err) => {
            log::error!("could not connect to database: {}", err);
            return Err(err.into());
        }
    };
    log::info!("connected to database at '{}:{}/{}'", db_host, db_port, db_name);

    let state = AppState::from(AppStateInner {
        conn,
        // ...
    });

    let auth_routes = Router::new()
        .route("/login", post(routes::auth::login))
        .route("/logout", post(routes::auth::logout))
        .route("/register", post(routes::auth::register))
        .route("/register-as-admin", post(routes::auth::register_as_admin))
        .route("/validate", post(routes::auth::validate));

    let user_routes = Router::new()
        .route("/", get(routes::user::get_all))
        .route("/:user_id", get(routes::user::get))
        .route("/:user_id", delete(routes::user::delete))
        .route("/:user_id", patch(routes::user::update))
        .route("/:user_id/gardens", get(routes::garden::get_all_from_user))
        .route("/:user_id/workers", get(routes::worker::get_all_from_user));
        
    let garden_routes = Router::new()
        .route("/", get(routes::garden::get_all))
        .route("/", post(routes::garden::create))
        .route("/:garden_id", get(routes::garden::get))
        .route("/:garden_id", delete(routes::garden::delete))
        .route("/:garden_id", patch(routes::garden::update))
        .route("/:garden_id/activities", get(routes::activity::get_all_from_garden));

    let activity_routes = Router::new()
        .route("/", get(routes::activity::get_all))
        .route("/", post(routes::activity::create))
        .route("/:activity_id", get(routes::activity::get))
        .route("/:activity_id", delete(routes::activity::delete))
        .route("/:activity_id", patch(routes::activity::update))
        .route("/:activity_id/workers", get(routes::worker::get_all_from_activity));

    let worker_routes = Router::new()
        .route("/", get(routes::worker::get_all))
        .route("/", post(routes::worker::create))
        .route("/:worker_id", get(routes::worker::get))
        .route("/:worker_id", delete(routes::worker::delete))
        .route("/:worker_id", patch(routes::worker::update))
        .route("/:worker_id/activities", get(routes::activity::get_all_from_worker));
    
    let serial_key_routes = Router::new()
        .route("/", post(routes::serial_key::create))
        .route("/validate", post(routes::serial_key::validate));

    let app = Router::new()
        .nest("/auth", auth_routes)
        .nest("/users", user_routes)
        .nest("/gardens", garden_routes)
        .nest("/activities", activity_routes)
        .nest("/workers", worker_routes)
        .nest("/serial-key", serial_key_routes)
        .with_state(state);

    let addr = format!("127.0.0.1:{}", auth_port).parse().unwrap();
    log::info!("serving at '{}'", addr);
    if let Err(err) = axum::Server::bind(&addr).serve(app.into_make_service()).await {
        log::error!("error during serve: {}", err);
        return Err(err.into());
    }
    log::info!("finished serving app");

    Ok(())
}



#[cfg(test)]
mod tests {
    use std::{time::Duration, sync::Arc};
    use reqwest::{StatusCode, cookie, Url};

    use super::*;

    fn setup() {
        // Load the .env file
        dotenvy::dotenv().unwrap();

        // Initialize the logger
        StdoutLogger::new().with_module_level("garden_planner", log::LevelFilter::Trace).init();
        log::info!("initialized logging");
    }

    #[tokio::test]
    async fn login_test() {
        setup();

        let join_handle = tokio::spawn(async move {
            app().await
        });

        let server_port = std::env::var("GARDEN_PLANNER_SERVER_PORT").unwrap_or("5001".into());
        let url = format!("http://localhost:{}", server_port).parse::<Url>().unwrap();
        let jar = Arc::new(cookie::Jar::default());
        jar.add_cookie_str("gardenSessionId=24247b17-d701-41bc-86f0-f581d315d7e8", &url);
        let client = reqwest::Client::builder()
            .connect_timeout(Duration::from_secs(30))
            .pool_idle_timeout(None)
            .timeout(Duration::from_secs(30))
            .user_agent("belot.endpoint.tests")
            .cookie_provider(Arc::clone(&jar))
            .build()
            .unwrap();

        let url = format!("http://localhost:{}/auth/logout", server_port);
        let res = client.post(url)
            // .json(&serde_json::json!({
            //     "username": "test",
            //     "password": "test",
            // }))
            .send()
            .await;

        match res {
            Ok(res) => {
                log::info!("garden_planner.test | status: {:?}", res.status());
                match res.status() {
                    StatusCode::OK => {
                        // log::info!("garden_planner.test | {:?}", res.json::<Vec<models::GardenRes>>().await);
                        log::info!("garden_planner.test | logout successful");
                    },
                    _ => {
                        log::error!("garden_planner.test | {:?}", res.text().await);
                    },
                }
            },
            Err(err) => {
                log::info!("garden_planner.test | {:?}", err);
            }
        }

        join_handle.await.unwrap().unwrap();
    }
}
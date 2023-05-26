pub mod loggers;

#[cfg(test)]
mod tests {
    use crate::loggers::StdoutLogger;

    #[test]
    fn testsadas() {
        StdoutLogger::new().init();

        log::info!("belot.auth::register() | 124712847-aszx | successfully registered user");
        log::warn!("belot.auth::register() | 124712847-aszx | registered user is lacking that extra something");
        log::error!("belot.auth::register() | 124712847-aszx | failed to register user");

    }
}

use std::sync::Arc;

pub struct AppStateInner {
    pub conn: sqlx::PgPool,
}

#[derive(Clone)]
pub struct AppState(Arc<AppStateInner>);

impl std::ops::Deref for AppState {
    type Target = AppStateInner;

    fn deref(&self) -> &Self::Target {
        &*self.0
    }
}

impl From<AppStateInner> for AppState {
    fn from(state: AppStateInner) -> Self {
        AppState(Arc::new(state))
    }
}
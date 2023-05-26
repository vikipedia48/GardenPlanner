
#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct Session {
	pub id: uuid::Uuid,
	pub user_id: i64,
	pub active_expires_at: chrono::NaiveDateTime,
	pub idle_expires_at: chrono::NaiveDateTime,
}
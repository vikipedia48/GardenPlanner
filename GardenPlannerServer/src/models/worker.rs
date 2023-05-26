
#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct Worker {
	pub id: i64,
	pub name: String,
	pub user_id: i64,
	pub created_at: chrono::NaiveDateTime,
}

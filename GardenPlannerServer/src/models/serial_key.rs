use uuid::Uuid;


#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct SerialKey {
	pub user_id: i64,
	pub original_key: Uuid,
	pub public_key: String,
	pub issued_at: chrono::NaiveDateTime,
}


#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct User {
	pub id: i64,
	pub username: String,
	pub password: String,
	pub is_admin: bool,
	pub created_at: chrono::NaiveDateTime,
}

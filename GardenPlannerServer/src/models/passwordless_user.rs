
#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct PasswordlessUser {
	pub id: i64,
	pub username: String,
	pub is_admin: bool,
	pub created_at: chrono::NaiveDateTime,
}

impl std::convert::From<super::User> for PasswordlessUser {
	fn from(value: super::User) -> Self {
		Self {
			id: value.id,
			username: value.username,
			is_admin: value.is_admin,
			created_at: value.created_at,
		}
	}
}

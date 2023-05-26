
#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
#[sqlx(type_name = "SessionWithUser")]
pub struct SessionWithUser {
	pub session: super::Session,
	pub user: super::User,
}
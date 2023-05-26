use base64::Engine;

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct Garden {
	pub id: i64,
	pub name: String,
	pub user_id: i64,
	pub plants: Vec<u8>,
	pub created_at: chrono::NaiveDateTime,
}

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Res {
	pub id: i64,
	pub name: String,
	pub user_id: i64,
	pub plants: String,
	pub created_at: chrono::NaiveDateTime,
}

impl Res {
	pub fn from_garden(garden: Garden) -> Self {
		Self {
			id: garden.id,
			name: garden.name,
			user_id: garden.user_id,
			plants: base64::engine::general_purpose::STANDARD_NO_PAD.encode(&garden.plants),
			created_at: garden.created_at,
		}
	}
}


#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize, sqlx::Type)]
pub struct Activity {
	pub id: i64,
	pub garden_id: i64,
	pub plant_id: i32,
	pub description: Option<String>,
	pub duration: i32,
	pub repeat: i32,
	pub date: Option<chrono::NaiveDate>,
	pub created_at: chrono::NaiveDateTime,
}

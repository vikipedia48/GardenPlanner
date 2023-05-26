
#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize/*, sqlx::Type */)]
// #[sqlx(type_name = "ActivityWithWorkers")]
pub struct ActivityWithWorkers {
	pub activity: super::Activity,
	pub workers: Vec<super::Worker>,
}
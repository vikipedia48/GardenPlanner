CREATE TABLE "garden_planner"."SerialKey" (
	user_id BIGINT PRIMARY KEY,
	original_key UUID NOT NULL,
	public_key VARCHAR(16384) NOT NULL,
	issued_at TIMESTAMP NOT NULL DEFAULT (CURRENT_TIMESTAMP AT TIME ZONE 'UTC'),
	FOREIGN KEY (user_id) REFERENCES "garden_planner"."User"(id)
);
CREATE TABLE "garden_planner"."Activity" (
	id BIGSERIAL PRIMARY KEY,
	garden_id BIGINT NOT NULL,
	plant_id INT NOT NULL,
	description VARCHAR(1024),
	duration INT NOT NULL DEFAULT 1,
	repeat INT NOT NULL DEFAULT 0,
	start_date DATE,
	created_at TIMESTAMP NOT NULL DEFAULT (CURRENT_TIMESTAMP AT TIME ZONE 'UTC'),
	FOREIGN KEY (garden_id) REFERENCES "garden_planner"."Garden" (id) MATCH FULL ON DELETE CASCADE
);
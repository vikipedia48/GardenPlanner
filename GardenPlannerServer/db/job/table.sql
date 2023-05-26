CREATE TABLE "garden_planner"."Job" (
	worker_id BIGINT NOT NULL,
	activity_id BIGINT NOT NULL,
	FOREIGN KEY (worker_id) REFERENCES "garden_planner"."Worker" (id) MATCH FULL ON DELETE CASCADE,
	FOREIGN KEY (activity_id) REFERENCES "garden_planner"."Activity" (id) MATCH FULL ON DELETE CASCADE,
	PRIMARY KEY (worker_id, activity_id)
);
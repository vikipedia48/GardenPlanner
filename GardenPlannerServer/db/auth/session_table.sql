CREATE TABLE "garden_planner"."Session" (
	id uuid DEFAULT uuid_generate_v4() PRIMARY KEY,
	user_id BIGINT NOT NULL,
	active_expires_at TIMESTAMP NOT NULL,
	idle_expires_at TIMESTAMP NOT NULL,
	FOREIGN KEY (user_id) REFERENCES "garden_planner"."User" (id) MATCH FULL ON DELETE CASCADE
);
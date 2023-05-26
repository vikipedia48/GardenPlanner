CREATE OR REPLACE FUNCTION "garden_planner"."get_all_workers_from_activity"(
	_activity_id BIGINT
)
RETURNS SETOF "garden_planner"."Worker" AS $$
BEGIN
	RETURN QUERY
		SELECT w.* FROM "garden_planner"."Worker" w
		JOIN "garden_planner"."Job" j ON w.id = j.worker_id
		JOIN "garden_planner"."Activity" a ON a.id = j.activity_id
		WHERE a.id = _activity_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_workers_from_activity(4);
*/
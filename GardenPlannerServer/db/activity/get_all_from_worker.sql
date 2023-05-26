CREATE OR REPLACE FUNCTION "garden_planner"."get_all_activities_from_worker"(
	_worker_id BIGINT
)
RETURNS SETOF "garden_planner"."Activity" AS $$
BEGIN
	RETURN QUERY 
		SELECT a.* FROM "garden_planner"."Activity" a
		JOIN "garden_planner"."Worker" w ON w.id = a.garden_id
		WHERE w.id = _worker_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_activities_from_worker(4);
*/
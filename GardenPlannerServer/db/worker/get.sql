CREATE OR REPLACE FUNCTION "garden_planner"."get_worker"(
	_worker_id BIGINT
)
RETURNS "garden_planner"."Worker" AS $$
DECLARE res "garden_planner"."Worker"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."Worker"
	WHERE id = _worker_id;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_worker(4);
*/
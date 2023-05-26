CREATE OR REPLACE FUNCTION "garden_planner"."create_job"(
	_worker_id BIGINT,
	_activity_id BIGINT
)
RETURNS "garden_planner"."Job" AS $$
DECLARE res "garden_planner"."Job"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."Job" (worker_id, activity_id)
		VALUES (_worker_id, _activity_id)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_job(1, 4);
*/
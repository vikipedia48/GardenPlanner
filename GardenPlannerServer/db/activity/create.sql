CREATE OR REPLACE FUNCTION "garden_planner"."create_activity"(
	_garden_id BIGINT,
	_plant_id INT,
	_desc VARCHAR(1024),
	_duration INT,
	_repeat INT,
	_start_date DATE,
	_assigned_workers BIGINT[]
)
RETURNS "garden_planner"."Activity" AS $$
DECLARE res "garden_planner"."Activity"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."Activity" (garden_id, plant_id, description, duration, repeat, start_date)
		VALUES (_garden_id, _plant_id, _desc, _duration, _repeat, _start_date)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	INSERT INTO "garden_planner"."Job" (worker_id, activity_id)
	SELECT unnest(_assigned_workers), res.id;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_activity(4, 1, 'sadnja paprike', 1, 0, '2018-01-01', ARRAY[1, 2, 3]);
*/
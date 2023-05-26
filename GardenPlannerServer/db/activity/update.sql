CREATE OR REPLACE FUNCTION "garden_planner"."update_activity"(
	_activity_id BIGINT,
	_plant_id INT,
	_desc VARCHAR(1024),
	_duration INT,
	_repeat INT,
	_start_date DATE,
	_assigned_workers BIGINT[]
)
RETURNS VOID AS $$
BEGIN		
	UPDATE "garden_planner"."Activity"
	SET plant_id = _plant_id,
		description = _desc,
		duration = _duration,
		repeat = _repeat,
		start_date = _start_date
	WHERE id = _activity_id;

	/* inefficient, but who care lol */
	DELETE FROM "garden_planner"."Job"
	WHERE activity_id = _activity_id;

	INSERT INTO "garden_planner"."Job" (worker_id, activity_id)
	SELECT unnest(_assigned_workers), _activity_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".update_activity(4, 2, 'sadnja paprike', 1, 0, '2018-01-01', ARRAY[1, 2, 3]);
*/
CREATE OR REPLACE FUNCTION "garden_planner"."update_worker"(
	_worker_id BIGINT,
	_name VARCHAR(64)
)
RETURNS VOID AS $$
BEGIN
	UPDATE "garden_planner"."Worker"
	SET name = _name
	WHERE id = _worker_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".update_worker(4, 'rokoviktor');
*/
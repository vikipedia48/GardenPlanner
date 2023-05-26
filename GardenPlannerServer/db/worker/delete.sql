CREATE OR REPLACE FUNCTION "garden_planner"."delete_worker"(
	_worker_id BIGINT
)
RETURNS VOID AS $$
BEGIN
	DELETE FROM "garden_planner"."Worker"
	WHERE id = _worker_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_worker(4);
*/
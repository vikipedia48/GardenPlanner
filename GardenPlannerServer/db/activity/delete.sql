CREATE OR REPLACE FUNCTION "garden_planner"."delete_activity"(
	_activity_id BIGINT
)
RETURNS VOID AS $$
BEGIN
	DELETE FROM "garden_planner"."Activity"
	WHERE id = _activity_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_activity(4);
*/
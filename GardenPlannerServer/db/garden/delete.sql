CREATE OR REPLACE FUNCTION "garden_planner"."delete_garden"(
	_garden_id BIGINT
)
RETURNS VOID AS $$
BEGIN
	DELETE FROM "garden_planner"."Garden"
	WHERE id = _garden_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_garden(4);
*/
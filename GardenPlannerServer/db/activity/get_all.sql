CREATE OR REPLACE FUNCTION "garden_planner"."get_all_activities"()
RETURNS SETOF "garden_planner"."Activity" AS $$
BEGIN
	RETURN QUERY SELECT * FROM "garden_planner"."Activity";
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_activities();
*/
CREATE OR REPLACE FUNCTION "garden_planner"."get_all_gardens"()
RETURNS SETOF "garden_planner"."Garden" AS $$
BEGIN
	RETURN QUERY SELECT * FROM "garden_planner"."Garden";
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_gardens();
*/
CREATE OR REPLACE FUNCTION "garden_planner"."get_all_workers"()
RETURNS SETOF "garden_planner"."Worker" AS $$
BEGIN
	RETURN QUERY SELECT * FROM "garden_planner"."Worker";
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_workers();
*/
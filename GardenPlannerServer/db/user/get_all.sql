CREATE OR REPLACE FUNCTION "garden_planner"."get_all_users"()
RETURNS SETOF "garden_planner"."User" AS $$
BEGIN
	RETURN QUERY SELECT * FROM "garden_planner"."User";
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_users();
*/
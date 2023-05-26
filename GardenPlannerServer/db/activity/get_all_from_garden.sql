CREATE OR REPLACE FUNCTION "garden_planner"."get_all_activities_from_garden"(
	_garden_id BIGINT
)
RETURNS SETOF "garden_planner"."Activity" AS $$
BEGIN
	RETURN QUERY 
		SELECT a.* FROM "garden_planner"."Activity" a
		JOIN "garden_planner"."Garden" g ON g.id = a.garden_id
		WHERE g.id = _garden_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_activities_from_garden(4);
*/
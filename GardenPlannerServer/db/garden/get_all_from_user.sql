CREATE OR REPLACE FUNCTION "garden_planner"."get_all_gardens_from_user"(
	_user_id BIGINT
)
RETURNS SETOF "garden_planner"."Garden" AS $$
BEGIN
	RETURN QUERY 
		SELECT g.* FROM "garden_planner"."Garden" g
		JOIN "garden_planner"."User" u ON u.id = g.user_id
		WHERE u.id = _user_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_gardens_for_user(4);
*/
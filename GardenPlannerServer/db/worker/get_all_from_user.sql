CREATE OR REPLACE FUNCTION "garden_planner"."get_all_workers_from_user"(
	_user_id BIGINT
)
RETURNS SETOF "garden_planner"."Worker" AS $$
BEGIN
	RETURN QUERY 
		SELECT w.* FROM "garden_planner"."Worker" w
		JOIN "garden_planner"."User" u ON u.id = w.user_id
		WHERE u.id = _user_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_workers_from_user(4);
*/
CREATE OR REPLACE FUNCTION "garden_planner"."check_perm_for_activity"(
	_activity_id BIGINT,
	_user_id BIGINT
)
RETURNS BOOLEAN AS $$
BEGIN
	RETURN EXISTS (
		SELECT 1 FROM "garden_planner"."Activity" a
		JOIN "garden_planner"."Garden" g ON g.id = a.garden_id
		JOIN "garden_planner"."User" u ON u.id = g.user_id OR u.is_admin
		WHERE _activity_id = a.id AND (u.is_admin OR u.id = _user_id)
	);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".check_perm_for_activity(4, 3);
*/
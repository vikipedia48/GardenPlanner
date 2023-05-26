CREATE OR REPLACE FUNCTION "garden_planner"."check_perm_for_garden"(
	_garden_id BIGINT,
	_user_id BIGINT
)
RETURNS BOOLEAN AS $$
BEGIN
	RETURN EXISTS (
		SELECT 1 FROM "garden_planner"."Garden" g
		JOIN "garden_planner"."User" u ON u.id = g.user_id OR u.is_admin
		WHERE _garden_id = g.id AND (u.is_admin OR u.id = _user_id)
	);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".check_perm_for_garden(4);
*/
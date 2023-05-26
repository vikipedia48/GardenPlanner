CREATE OR REPLACE FUNCTION "garden_planner"."check_perm_for_user"(
	_target_user_id BIGINT,
	_user_id BIGINT
)
RETURNS BOOLEAN AS $$
BEGIN
	RETURN EXISTS (
		SELECT 1 FROM "garden_planner"."User"
		WHERE _user_id = id AND (is_admin OR _user_id = _target_user_id)
	);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".check_perm_for_user(4, 4);
*/
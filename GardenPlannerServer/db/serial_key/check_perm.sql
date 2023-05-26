CREATE OR REPLACE FUNCTION "garden_planner"."check_perm_for_serial_key"(
	_serial_key_id BIGINT,
	_user_id BIGINT
)
RETURNS BOOLEAN AS $$
BEGIN
	RETURN EXISTS (
		SELECT 1 FROM "garden_planner"."SerialKey" k
		JOIN "garden_planner"."User" u ON u.id = k.user_id OR u.is_admin
		WHERE _serial_key_id = k.id AND (u.is_admin OR u.id = _user_id)
	);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".check_perm_for_serial_key(4);
*/
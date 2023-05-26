CREATE OR REPLACE FUNCTION "garden_planner"."check_perm_for_worker"(
	_worker_id BIGINT,
	_user_id BIGINT
)
RETURNS BOOLEAN AS $$
BEGIN
	RETURN EXISTS (
		SELECT 1 FROM "garden_planner"."Worker" w
		JOIN "garden_planner"."User" u ON u.id = w.user_id OR u.is_admin
		WHERE _worker_id = w.id AND (u.is_admin OR u.id = _user_id)
	);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".check_perm_for_worker(4, 4);
*/
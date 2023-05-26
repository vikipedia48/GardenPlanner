CREATE OR REPLACE FUNCTION "garden_planner"."delete_user"(
	_user_id BIGINT
)
RETURNS VOID AS $$
BEGIN
	DELETE FROM "garden_planner"."User"
	WHERE id = _user_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_user(4);
*/
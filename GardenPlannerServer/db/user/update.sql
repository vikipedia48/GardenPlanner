CREATE OR REPLACE FUNCTION "garden_planner"."update_user"(
	_user_id BIGINT,
	_username VARCHAR(32)
)
RETURNS VOID AS $$
BEGIN
	UPDATE "garden_planner"."User"
	SET username = _username
	WHERE id = _user_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".update_user(4, 'asiudaisdas');
*/
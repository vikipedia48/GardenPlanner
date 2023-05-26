CREATE OR REPLACE FUNCTION "garden_planner"."create_user"(
	_username VARCHAR(32),
	_password VARCHAR(1024),
	_is_admin BOOLEAN
)
RETURNS "garden_planner"."User" AS $$
DECLARE res "garden_planner"."User"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."User" (username, password, is_admin)
		VALUES (_username, _password, _is_admin)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_user('bloo', 'asduaisfzaigagiahfkashdas', false);
*/
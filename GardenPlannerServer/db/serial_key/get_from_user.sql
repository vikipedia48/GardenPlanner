CREATE OR REPLACE FUNCTION "garden_planner"."get_serial_key_from_user"(
	_user_id BIGINT
)
RETURNS "garden_planner"."SerialKey" AS $$
DECLARE res "garden_planner"."SerialKey"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."SerialKey"
	WHERE user_id = _user_id;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_serial_key_from_user(4);
*/
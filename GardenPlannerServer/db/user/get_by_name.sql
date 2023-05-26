CREATE OR REPLACE FUNCTION "garden_planner"."get_user_by_name"(
	_username VARCHAR(32)
)
RETURNS "garden_planner"."User" AS $$
DECLARE res "garden_planner"."User"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."User"
	WHERE username = _username;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_user_by_name('viktor');
*/
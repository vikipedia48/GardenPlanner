CREATE OR REPLACE FUNCTION "garden_planner"."get_user"(
	_user_id BIGINT
)
RETURNS "garden_planner"."User" AS $$
DECLARE res "garden_planner"."User"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."User"
	WHERE id = _user_id;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_user(4);
*/
CREATE OR REPLACE FUNCTION "garden_planner"."get_session"(
	_session_id uuid
)
RETURNS "garden_planner"."Session" AS $$
DECLARE res "garden_planner"."Session"%ROWTYPE;
BEGIN
	SELECT DISTINCT s.* INTO res FROM "garden_planner"."Session" s
	JOIN "garden_planner"."User" u ON u.id = s.user_id
	WHERE s.id = _session_id;
	
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_session('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx');
*/
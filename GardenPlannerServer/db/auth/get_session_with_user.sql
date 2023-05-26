CREATE OR REPLACE FUNCTION "garden_planner"."get_session_with_user"(
	_session_id uuid
)
RETURNS "garden_planner"."SessionWithUser" AS $$
DECLARE res_session "garden_planner"."Session"%ROWTYPE;
		res_user "garden_planner"."User"%ROWTYPE;
		res "garden_planner"."SessionWithUser";
BEGIN
    SELECT DISTINCT * INTO res_session FROM "garden_planner"."Session" s
    WHERE s.id = _session_id;
	
	SELECT DISTINCT u.* INTO res_user FROM "garden_planner"."User" u
	JOIN "garden_planner"."Session" s ON u.id = s.user_id
	WHERE s.id = _session_id;
	
	SELECT res_session, res_user INTO res;
	
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_session_with_user('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx');
*/
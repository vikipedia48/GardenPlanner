CREATE OR REPLACE FUNCTION "garden_planner"."validate_session_with_user"(
	_session_id uuid
)
RETURNS "garden_planner"."SessionWithUser" AS $$
DECLARE res_session "garden_planner"."Session"%ROWTYPE;
		res_user "garden_planner"."User"%ROWTYPE;
		now TIMESTAMP := CURRENT_TIMESTAMP AT TIME ZONE 'UTC';
		res "garden_planner"."SessionWithUser";
BEGIN
    SELECT DISTINCT * INTO res_session FROM "garden_planner"."Session" s
    WHERE s.id = _session_id;
	
	SELECT DISTINCT u.* INTO res_user FROM "garden_planner"."User" u
	JOIN "garden_planner"."Session" s ON u.id = s.user_id
	WHERE s.id = _session_id;
	
	IF res_session IS NULL THEN
		RAISE EXCEPTION 'No such session found';
	END IF;
	
	IF now > res_session.idle_expires_at THEN
		RAISE EXCEPTION 'Dead session';
	ELSIF now > res_session.active_expires_at THEN
		SELECT "garden_planner".renew_session(_session_id), res_user INTO res;
	ELSE
		SELECT res_session, res_user INTO res;
	END IF;
	
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".validate_session_with_user('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx');
*/
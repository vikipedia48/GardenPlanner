CREATE OR REPLACE FUNCTION "garden_planner"."validate_session"(
	_session_id uuid
)
RETURNS "garden_planner"."Session" AS $$
DECLARE _session "garden_planner"."Session"%ROWTYPE;
		now TIMESTAMP := CURRENT_TIMESTAMP AT TIME ZONE 'UTC';
BEGIN
	SELECT DISTINCT * INTO _session FROM "garden_planner"."Session"
	WHERE id = _session_id;
	
	IF _session is NULL THEN
		RAISE EXCEPTION 'No such session found';
	END IF;
	
	IF now > _session.idle_expires_at THEN
		RAISE EXCEPTION 'Dead session';
	ELSIF now > _session.active_expires_at THEN
		RETURN "garden_planner".renew_session(_session_id);
	ELSE
		RETURN _session;
	END IF;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".validate_session(10);
*/
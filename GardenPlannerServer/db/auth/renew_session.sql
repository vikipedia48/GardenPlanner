 CREATE OR REPLACE FUNCTION "garden_planner"."renew_session"(
	_session_id uuid
) 
RETURNS "garden_planner"."Session" AS $$
DECLARE _user_id BIGINT;
BEGIN
	SELECT user_id INTO _user_id FROM "garden_planner"."Session"
	WHERE id = _session_id;
	
	IF _user_id IS NULL THEN
		RAISE EXCEPTION 'No such session found';
	END IF;

	DELETE FROM "garden_planner"."Session"
	WHERE id = _session_id;

	RETURN "garden_planner".create_session(_user_id);
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".renew_session('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx');
*/
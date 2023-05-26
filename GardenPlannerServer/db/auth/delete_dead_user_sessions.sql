CREATE OR REPLACE FUNCTION "garden_planner"."delete_dead_user_sessions"(
	_user_id BIGINT
) 
RETURNS void AS $$
DECLARE now TIMESTAMP := CURRENT_TIMESTAMP AT TIME ZONE 'UTC';
BEGIN
	DELETE FROM "garden_planner"."Session"
	WHERE user_id = _user_id AND now > idle_expires_at;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_dead_user_sessions();
*/
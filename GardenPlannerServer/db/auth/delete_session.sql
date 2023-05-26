CREATE OR REPLACE FUNCTION "garden_planner"."delete_session"(
	_session_id uuid
)
RETURNS void AS $$
BEGIN
	DELETE FROM "garden_planner"."Session"
	WHERE id = _session_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_session(10);
*/
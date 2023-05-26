CREATE OR REPLACE FUNCTION "garden_planner"."create_session"(
	_user_id BIGINT
)
RETURNS "garden_planner"."Session" AS $$
DECLARE res "garden_planner"."Session"%ROWTYPE;
		active_expires TIMESTAMP;
		idle_expires TIMESTAMP;
BEGIN
	SELECT 
		active_expires_at, idle_expires_at INTO active_expires, idle_expires
	FROM "garden_planner".default_expires();

	WITH inserted AS (
		INSERT INTO "garden_planner"."Session" (user_id, active_expires_at, idle_expires_at)
		VALUES (_user_id, active_expires, idle_expires)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_session(10);
*/
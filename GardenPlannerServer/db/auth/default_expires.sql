CREATE OR REPLACE FUNCTION "garden_planner"."default_expires"() 
RETURNS TABLE (active_expires_at TIMESTAMP, idle_expires_at TIMESTAMP) AS $$
BEGIN
	RETURN QUERY SELECT 
		CURRENT_TIMESTAMP AT TIME ZONE 'UTC' + interval '1 day', 
		CURRENT_TIMESTAMP AT TIME ZONE 'UTC' + interval '1 week';
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "belot-auth".default_expires();
*/
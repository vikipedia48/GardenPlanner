CREATE OR REPLACE FUNCTION "garden_planner"."get_activity"(
	_activity_id BIGINT
)
RETURNS "garden_planner"."Activity" AS $$
DECLARE res "garden_planner"."Activity"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."Activity"
	WHERE id = _activity_id;
	
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_activity(4);
*/
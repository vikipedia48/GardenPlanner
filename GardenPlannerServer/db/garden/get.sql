CREATE OR REPLACE FUNCTION "garden_planner"."get_garden"(
	_garden_id BIGINT
)
RETURNS "garden_planner"."Garden" AS $$
DECLARE res "garden_planner"."Garden"%ROWTYPE;
BEGIN
	SELECT DISTINCT * INTO res FROM "garden_planner"."Garden"
	WHERE id = _garden_id;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_garden(4);
*/
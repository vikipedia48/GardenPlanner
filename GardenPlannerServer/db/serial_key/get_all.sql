CREATE OR REPLACE FUNCTION "garden_planner"."get_all_serial_keys"()
RETURNS SETOF "garden_planner"."SerialKey" AS $$
BEGIN
	RETURN QUERY SELECT * FROM "garden_planner"."SerialKey";
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".get_all_serial_keys();
*/
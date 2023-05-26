CREATE OR REPLACE FUNCTION "garden_planner"."delete_serial_key"(
	_key_id BIGINT
)
RETURNS VOID AS $$
BEGIN
	DELETE FROM "garden_planner"."SerialKey"
	WHERE id = _key_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".delete_serial_key('dausdh-dasdja-asdjas-sdad');
*/
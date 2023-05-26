CREATE OR REPLACE FUNCTION "garden_planner"."create_serial_key"(
	_user_id BIGINT,
	_original_key UUID,
	_public_key VARCHAR(16384)
)
RETURNS "garden_planner"."SerialKey" AS $$
DECLARE res "garden_planner"."SerialKey"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."SerialKey" (user_id, original_key, public_key)
		VALUES (_user_id, _original_key, _public_key)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_serial_key(2, 'chernobog');
*/
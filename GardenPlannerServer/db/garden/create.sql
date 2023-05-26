CREATE OR REPLACE FUNCTION "garden_planner"."create_garden"(
	_name VARCHAR(64),
	_user_id BIGINT,
	_plants BYTEA
)
RETURNS "garden_planner"."Garden" AS $$
DECLARE res "garden_planner"."Garden"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."Garden" (name, user_id, plants)
		VALUES (_name, _user_id, _plants)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_garden('viktorova basca', 4, E'asiudaisdas');
*/
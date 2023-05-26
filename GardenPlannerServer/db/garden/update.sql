CREATE OR REPLACE FUNCTION "garden_planner"."update_garden"(
	_garden_id BIGINT,
	_name VARCHAR(64),
	_plants BYTEA
)
RETURNS VOID AS $$
BEGIN
	UPDATE "garden_planner"."Garden"
	SET name = _name,
		plants = _plants
	WHERE id = _garden_id;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".update_garden(4, 'rokova basca', E'asiudaisdas');
*/
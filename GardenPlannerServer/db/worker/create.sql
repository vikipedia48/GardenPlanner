CREATE OR REPLACE FUNCTION "garden_planner"."create_worker"(
	_name VARCHAR(64),
	_user_id BIGINT
)
RETURNS "garden_planner"."Worker" AS $$
DECLARE res "garden_planner"."Worker"%ROWTYPE;
BEGIN
	WITH inserted AS (
		INSERT INTO "garden_planner"."Worker" ("name", user_id)
		VALUES (_name, _user_id)
		RETURNING *
	)
	SELECT DISTINCT * INTO res FROM inserted;

	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

/*
SELECT "garden_planner".create_worker('vlado hitler', 4);
*/
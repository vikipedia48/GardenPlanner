CREATE OR REPLACE FUNCTION "garden_planner"."get_all_activities_with_workers_from_garden"(
	_garden_id BIGINT
)
RETURNS "garden_planner"."ActivityWithWorkers" AS $$
DECLARE res_activity "garden_planner"."Activity"%ROWTYPE;
		res_workers "garden_planner"."Worker"[];
		res "garden_planner"."ActivityWithWorkers";
BEGIN		
    SELECT DISTINCT a.* INTO res_activity FROM "garden_planner"."Activity" a
	JOIN "garden_planner"."Garden" g ON g.id = a.garden_id
	WHERE g.id = _garden_id;
	
	SELECT w.* INTO res_workers FROM "garden_planner"."Worker" w
	JOIN "garden_planner"."Job" j ON w.id = j.worker_id
	JOIN "garden_planner"."Activity" a ON a.id = j.activity_id
	WHERE a.id = res_activity.id;
	
	SELECT res_activity, res_workers INTO res;
	
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;


/*
SELECT "garden_planner".get_all_activities_from_garden(4);
*/
CREATE TYPE "garden_planner"."ActivityWithWorkers" AS (
	"activity" "garden_planner"."Activity",
	"workers" "garden_planner"."Worker"[]
);
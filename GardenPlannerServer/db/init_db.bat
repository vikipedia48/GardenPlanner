@echo off

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\check_perm.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\create.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\delete.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\get.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\get_by_name.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\get_all.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\user\update.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\session_table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\types\session_with_user.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\create_session.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\default_expires.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\delete_dead_user_sessions.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\get_session.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\get_session_with_user.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\renew_session.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\validate_session.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\validate_session_with_user.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\check_perm.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\create.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\delete.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\get.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\get_all.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\get_all_from_user.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\garden\update.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\check_perm.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\create.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\delete.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\get.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\get_all.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\get_all_from_user.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\update.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\auth\types\activity_with_workers.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\check_perm.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\create.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\delete.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\get.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\get_all.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\get_all_from_garden.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\get_all_from_garden_with_workers.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\update.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\job\table.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\job\create.sql

"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\worker\get_all_from_activity.sql
"D:\Apps\Postgres\bin\psql.exe" -U postgres -d postgres -a -f D:\Coding\garden-planner\db\activity\get_all_from_worker.sql

py build_rel.py "./examples/thwomp.c" 0x58 g_profile_THWOMP --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check 1df51145d619c6ea20dc40f77d8e80b6
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/switch_op.c" 0x1A0 g_profile_SwitchOperator --md5_check cd385ac9214bea7128830ba4a0616f49
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/npc_test.c" 0x58 g_profile_NPC_Test --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check 654323782533b3d485b936390aa48298
if %errorlevel% neq 0 exit /b %errorlevel%


py build_rel.py "./examples/thwomp.c" 0x58 g_profile_THWOMP --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check ebe4d8bfc8c836cccb8cff5d1f56cf54
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/switch_op.c" 0x1A0 g_profile_SwitchOperator --md5_check cd385ac9214bea7128830ba4a0616f49
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/npc_test.c" 0x58 g_profile_NPC_Test --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check 9811b281c84bf3ab5163de205e71a8c4
if %errorlevel% neq 0 exit /b %errorlevel%

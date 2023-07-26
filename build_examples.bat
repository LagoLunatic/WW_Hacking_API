
py build_rel.py "./examples/thwomp.c" 0x58 g_profile_THWOMP --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check 811b96a10639bbdaf22074838adf4494
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/switch_op.c" 0x1A0 g_profile_SwitchOperator --md5_check 27b792d713c7955a96d3ce63ad06f913
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/npc_test.c" 0x58 g_profile_NPC_Test --rels_arc "../Random Seeds/WW Random EnemyRandoTest/files/RELS.arc" --md5_check 82d2078763efa3082ba5470ebf4920e5
if %errorlevel% neq 0 exit /b %errorlevel%

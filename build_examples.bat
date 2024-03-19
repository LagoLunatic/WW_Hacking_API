
py build_rel.py "./examples/thwomp.c" 0x58 g_profile_THWOMP --rels_arc "../Random Seeds/WW Random TestSeed/files/RELS.arc" --md5_check 811b96a10639bbdaf22074838adf4494
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/switch_op.c" 0x1A0 g_profile_SwitchOperator --md5_check afb511459b2cb5dcd3597a95109783d8
if %errorlevel% neq 0 exit /b %errorlevel%

py build_rel.py "./examples/npc_test.c" 0x58 g_profile_NPC_Test --rels_arc "../Random Seeds/WW Random TestSeed/files/RELS.arc" --md5_check 4f5f2b54a4b10b7f73d9790e6cfa7ee2
if %errorlevel% neq 0 exit /b %errorlevel%

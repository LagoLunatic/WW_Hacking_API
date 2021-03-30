
struct fopNpc_npc_c;
struct fopAc_ac_c;
enum PhaseState;

struct Test_Npc_Cpp : fopNpc_npc_c {
    PhaseState _create();
    bool _delete();
    bool _execute();
    bool _draw();
};

PhaseState Test_Npc_Cpp_Create(fopAc_ac_c *pActor) {
  Test_Npc_Cpp * actorCast = (Test_Npc_Cpp *)pActor;
  return actorCast->_create();
}

bool Test_Npc_Cpp_Delete(fopAc_ac_c *pActor) {
  Test_Npc_Cpp * actorCast = (Test_Npc_Cpp *)pActor;
  return actorCast->_delete();
}

bool Test_Npc_Cpp_Execute(fopAc_ac_c *pActor) {
  Test_Npc_Cpp * actorCast = (Test_Npc_Cpp *)pActor;
  return actorCast->_execute();
}

bool Test_Npc_Cpp_IsDelete(fopAc_ac_c *pActor) {
  return true;
}

bool Test_Npc_Cpp_Draw(fopAc_ac_c *pActor) {
  Test_Npc_Cpp * actorCast = (Test_Npc_Cpp *)pActor;
  return actorCast->_draw();
}

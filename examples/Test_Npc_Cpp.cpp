#include "../vanilla_defines/ww_defines.h"
#include "RelDefines.h"
#include "Test_Npc_Cpp.h"


struct fopNpc_npc_c;
struct fopAc_ac_c;
enum PhaseState;


/* *** Begin Struct Definitions *** */

struct Test_Npc_Cpp : fopNpc_npc_c {
    PhaseState _create();
    bool _delete();
    bool _execute();
    bool _draw();
};

PhaseState Test_Npc_Cpp::_create() {
  OSReport("Creating!\n");
  return cPhs_COMPLEATE_e;
}

bool Test_Npc_Cpp::_delete() {
  OSReport("Deleting!\n");
  return true;
}

bool Test_Npc_Cpp::_execute() {
  OSReport("Executing!\n");
  return true;
}

bool Test_Npc_Cpp::_draw() {
  OSReport("Drawing!\n");
  return true;
}

/* *** End Struct Definitions *** */


/* *** Begin Profile Definitions *** */

// Interface functions for the game engine to call
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

// Function pointer table so the engine knows how to call the above functions
profile_method_class l_daTestNPCCpp_Method {
  {
    &Test_Npc_Cpp_Create,
    &Test_Npc_Cpp_Delete,
    &Test_Npc_Cpp_Execute,
    &Test_Npc_Cpp_IsDelete,
    &Test_Npc_Cpp_Draw,
  },
  
  0,
  0,
  0
};

// Information that the engine needs to know about the actor
const f_pc_profile__Profile_Actor g_profile_Test_NPC_CPP = {
  {
    -3,
    7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    -3,
    0xB5, // Actor ID
    0,
    0,
    &g_fpcLf_Method,
    sizeof(Test_Npc_Cpp),
    0,
    0,
    &g_fopAc_Method,
  },
  
  0x9F,
  0,
  0,
  &l_daTestNPCCpp_Method,
  fopAcM__Status__None,
  fopAc_ac_c__Type__Some_NPCs,
  0,
  0,
  0
};

/* *** End Profile Definitions *** */

#include "../vanilla_defines/ww_defines.h"
#include "Test_Npc_Cpp.h"

/* *** Begin Required Constants *** */
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
/* *** End Required Constants *** */


/* *** Begin REL Link Functions *** */
#define SECTION(S) __attribute__((section(S)))
typedef void (*VoidFuncPtr)(void);

extern "C" {
  void ModuleProlog();
  void ModuleEpilog();
  void ModuleUnresolved();
}

VoidFuncPtr _ctors[] SECTION(".ctors") { 0 };
VoidFuncPtr _dtors[] SECTION(".dtors") { 0 };

void _prolog() {
  DynamicLink__ModuleConstructorsX((void **)(&_ctors));
  DynamicLink__ModuleProlog();
}

void _epilog() {
  DynamicLink__ModuleEpilog();
  DynamicLink__ModuleDestructorsX((void **)(&_dtors));
}

void _unresolved() {
  DynamicLink__ModuleUnresolved();
}
/* *** End REL Link Functions *** */


/* *** Begin Implemented Class Functions *** */
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
/* *** End Implemented Class Functions *** */

#include "linking_Test.h"
#include "f_pc_profile.h"

#define SECTION( S ) __attribute__ ((section (S)))

void dDlst_derive_c::draw() {
  return;
}

int dDlst_derive_c::test_func() {
  return 25;
}

typedef void (*VoidFuncPtr)(void);

extern "C" {
  void ModuleProlog();
  void ModuleEpilog();
  void ModuleUnresolved();
  
  f_pc_profile::profile_leaf_method_class g_fpcLf_Method;
  f_pc_profile::profile_method_class g_fopAc_Method;
}

static int funcA(dDlst_derive_c* test) {
  test->draw();
  return 25;
}

f_pc_profile::profile_method_class l_daNPCTest_Method {
  {
    &funcA,
    &funcA,
    &funcA,
    &funcA,
    &funcA,
  },
  
  0,
  0,
  0,
};

f_pc_profile::Profile_Actor g_profile_Daytimer {
  {
    -3,
    7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    3,
    0xB5, // Actor ID
    0,
    0,
    &g_fpcLf_Method,
    sizeof(dDlst_derive_c),
    0,
    0,
    &g_fopAc_Method,
  },
  
  0x9F,
  0,
  0,
  &l_daNPCTest_Method,
  f_pc_profile::fopAcM__Status::None,
  f_pc_profile::fopAc_ac_c__Type::Some_NPCs,
  0,
  0,
  0
};

VoidFuncPtr dummy_global_constructor SECTION(".ctors") = 0;
VoidFuncPtr dummy_global_destructor SECTION(".dtors") = 0;

/** REL LINK FUNCTIONS **/
/** Called to link and unlink the REL. Don't change these! **/
void _prolog() {
  ModuleProlog();
}

void _epilog() {
  ModuleEpilog();
}

void _unresolved() {
  ModuleUnresolved();
}

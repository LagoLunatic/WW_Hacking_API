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
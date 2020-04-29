
#include "../vanilla_defines/ww_defines.h"
#include "./npc_test.h"

static void * _ctors SECTION(".ctors");
static void * _dtors SECTION(".dtors");

void _prolog() {
  DynamicLink__ModuleConstructorsX(&_ctors);
  DynamicLink__ModuleProlog();
}

void _epilog() {
  DynamicLink__ModuleEpilog();
  DynamicLink__ModuleDestructorsX(&_dtors);
}

void _unresolved() {
  DynamicLink__ModuleUnresolved();
}


int daNPCTest_createSolidHeap_CB(NPC_Test_class* this) {
  J3DModelData* modelData = dRes_control_c__getIDRes("Md", 0x5C, g_dComIfG_gameInfo.mResCtrl.mObjectInfo, 0x40);
  
  J3DAnmTransformKey* animData = dRes_control_c__getIDRes("Md", 0x1E, g_dComIfG_gameInfo.mResCtrl.mObjectInfo, 0x40);
  
  this->parent.mpMcaMorf = (mDoExt_McaMorf*)JKernel__operator_new(sizeof(mDoExt_McaMorf));
  
  mDoExt_McaMorf__mDoExt_McaMorf(
    this->parent.mpMcaMorf, 0x1, modelData, 0, 0,
    animData, 2, 1.0f, 0, -1, 0x1, 0,
    0x80000, 0x11020022
  );
  
  this->parent.mpMcaMorf->mpModel->mpUserData = (pointer)this;
  
  return 1;
}

int daNPCTest_Create(NPC_Test_class* this) {
  PhaseState phaseState = dComIfG_resLoad(&this->mPhaseRequest, "Md");
  if (phaseState != cPhs_COMPLEATE_e) {
    // Not finished loading yet, check again next frame.
    return phaseState;
  }
  
  int maxHeapMemoryNeeded = 0; // No maximum
  int error = fopAcM_entrySolidHeap(&this->parent.parent, (int (*)(fopAc_ac_c *))&daNPCTest_createSolidHeap_CB, maxHeapMemoryNeeded);
  if (error == 0) {
    return cPhs_ERROR_e;
  }
  
  this->parent.parent.mpMtx = &this->parent.mpMcaMorf->mpModel->mBaseMtx;
  mDoExt_McaMorf__setMorf(this->parent.mpMcaMorf, 0.0f);
  
  return cPhs_COMPLEATE_e;
}

int daNPCTest_IsDelete(NPC_Test_class* this) {
  return 1;
}

int daNPCTest_Delete(NPC_Test_class* this) {
  dComIfG_resDelete(&this->mPhaseRequest, "Md");
  
  return 1;
}

int daNPCTest_Draw(NPC_Test_class* this) {
  dScnKy_env_light_c__settingTevStruct(
    &g_env_light, settingTevStruct__LightType__Actor,
    &this->parent.parent.mCurrent.mPos, &this->parent.parent.mTevStr
  );
  
  dScnKy_env_light_c__setLightTevColorType(&g_env_light, this->parent.mpMcaMorf->mpModel, &this->parent.parent.mTevStr);
  
  mDoExt_McaMorf__entryDL(this->parent.mpMcaMorf);
  
  return 1;
}

int daNPCTest_Execute(NPC_Test_class* this) {
  //OSReport("mCurrent pos: (%f, %f, %f)", this->parent.parent.mCurrent.mPos.x, this->parent.parent.mCurrent.mPos.y, this->parent.parent.mCurrent.mPos.z);
  
  // Play the animation.
  mDoExt_McaMorf__play(this->parent.mpMcaMorf, &this->parent.parent.mCurrent.mPos, 0, 0);
  
  // Update the model's position.
  PSMTXTrans(this->parent.parent.mCurrent.mPos.x, this->parent.parent.mCurrent.mPos.y, this->parent.parent.mCurrent.mPos.z, &mDoMtx_stack_c__now);
  PSMTXCopy(&mDoMtx_stack_c__now, &this->parent.mpMcaMorf->mpModel->mBaseMtx);
  
  // Calculate how the vertices should be deformed by the animation.
  mDoExt_McaMorf__calc(this->parent.mpMcaMorf);
  
  return 1;
}

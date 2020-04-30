
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

void daNPCTest__daNPCTest(NPC_Test_class* this) {
  fopAc_ac_c__fopAc_ac_c(&this->parent.parent);
  
  this->parent.field_0x29c = 0;
  this->parent.field_0x29b = 0;
  this->parent.field_0x2cc = 0;
  this->parent.field_0x2d0 = 0;
  this->parent.field_0x32c = 0;
  
  // Initialize the actor's Cc collision (so it prevents other actors from walking into it).
  dCcD_GStts__dCcD_GStts(&this->parent.mStts.mGStts);
  this->parent.mStts.parent.vtbl = &dCcD_Stts____vt.parent;
  this->parent.mStts.mGStts.parent.vtbl = &dCcD_Stts____vt.mGtts_vtbl;
  // Cylinder shaped Cc collision.
  dCcD_GObjInf__dCcD_GObjInf(&this->parent.mCyl.parent);
  this->parent.mCyl.mCylAttr.parent.mAab.vtbl = &cM3dGAab____vt;
  this->parent.mCyl.parent.parent.parent.parent.vtbl = &dCcD_Cyl____vt.parent;
  this->parent.mCyl.mCylAttr.parent.vtbl = &dCcD_Cyl____vt.mCyl_vtbl;  
  this->parent.mCyl.mCylAttr.mCyl.vtbl = &dCcD_Cyl____vt.mCylAttr_vtbl;
  
  this->parent.mCurrMsgBsPcId = -1;
  this->parent.mpCurrMsg = 0;
  
  this->parent.vtbl = (fopNpc_npc_c__vtbl*)&daNpc_Test_c_vtbl;
}

int daNPCTest_Create(NPC_Test_class* this) {
  if ((this->parent.parent.mMiscFlags & fopAc_ac_c__MiscFlags__Constructed) == 0) {
    daNPCTest__daNPCTest(this);
    this->parent.parent.mMiscFlags |= fopAc_ac_c__MiscFlags__Constructed;
  }
  
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
  
  dCcD_Stts__Init(&this->parent.mStts, 0xff, 0xff, &this->parent.parent);
  this->parent.mCyl.parent.parent.parent.mpStts = &this->parent.mStts.parent;
  dCcD_Cyl__Set(&this->parent.mCyl, &d_npc__dNpc_cyl_src);
  
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
  
  // Set the actor's Cc collision (so it prevents other actors from walking into it).
  fopNpc_npc_c__setCollision(&this->parent, 50.0f, 140.0f);
  
  return 1;
}

int daNPCTest__next_msgStatus(NPC_Test_class* this, ulong* msgIDPtr) {
  return 0x10;
}

ulong daNPCTest__getMsg(NPC_Test_class* this) {
  return 0;
}

void daNPCTest__anmAtr(NPC_Test_class* this, ushort unk) {
  return;
}

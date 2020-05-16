
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
  
  dNpc_EventCut_c__setActorInfo2(&this->eventActor, "Md1", &this->parent);
  
  this->parent.mJntCtrl.field_0xc = 0;
  this->parent.mJntCtrl.field_0xb = 0;
  this->parent.field_0x2cc = 0;
  this->parent.field_0x2d0 = 0;
  this->parent.field_0x32c = 0;
  
  this->parent.parent.mAttentionFlags[1] = 0xAB;
  this->parent.parent.mAttentionFlags[3] = 0xA9;
  this->parent.parent.mInteractFlags = fopAc_ac_c__InteractFlags__Targetable_B | fopAc_ac_c__InteractFlags__Talkable | fopAc_ac_c__InteractFlags__PlayEnemyMusic;
  
  // Initialize the actor's Bg collision checker (so it stops moving when hitting walls and floors).
  dBgS_Acch__dBgS_Acch(&this->parent.mObjAcch.parent);
  this->parent.mObjAcch.parent.mChk.vtbl = &dBgS_ObjAcch____vt.mChk_vtbl;
  this->parent.mObjAcch.parent.mDChk.mPolyPassChk.parent.vtbl = &dBgS_ObjAcch____vt.mDChk_vtbl.mPolyPassChk_vtbl;
  this->parent.mObjAcch.parent.mDChk.mGrpPassChk.parent.vtbl = &dBgS_ObjAcch____vt.mDChk_vtbl.mGrpPassChk_vtbl;
  this->parent.mObjAcch.parent.mDChk.mPolyPassChk.parent.mPass0 = 1;
  dBgS_AcchCir__dBgS_AcchCir(&this->parent.mAcchCir);
  
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
  
  // Initialize the actor's path.
  int pathIndex = (this->parent.parent.parent.parent.mParameters & 0x000000FF);
  if (pathIndex != 0xFF) {
    dNpc_PathRun_c__setInf(&this->mPathRun, pathIndex, this->parent.parent.mCurrent.mRoomNo, true);
    if (this->mPathRun.mPath == 0) {
      return cPhs_ERROR_e;
    }
    this->parent.parent.mStatus &= ~fopAcM__Status__DoNotExecuteIfDidNotDraw;
    
    dNpc_PathRun_c__getPoint(&this->parent.parent.mCurrent.mPos, &this->mPathRun, this->mPathRun.mCurrPointIndex);
    this->parent.parent.mNext.mPos.x = this->parent.parent.mCurrent.mPos.x;
    this->parent.parent.mNext.mPos.y = this->parent.parent.mCurrent.mPos.y;
    this->parent.parent.mNext.mPos.z = this->parent.parent.mCurrent.mPos.z;
    
    dNpc_PathRun_c__incIdxLoop(&this->mPathRun);
  }
  
  // Set the actor's Bg collision checker with a half-height of 30 and a radius of 50.
  dBgS_AcchCir__SetWall(&this->parent.mAcchCir, 30.0f, 50.0f);
  dBgS_Acch__Set(&this->parent.mObjAcch.parent,
                 &this->parent.parent.mCurrent.mPos,
                 &this->parent.parent.mNext.mPos,
                 &this->parent.parent, 1,
                 &this->parent.mAcchCir,
                 &this->parent.parent.mVelocity,
                 0, 0);
  
  // Set the actor's Cc collision.
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
  
  // Determine if we've reached the current path point and advance to the next one if so.
  bool reachedCurrPoint = dNpc_PathRun_c__chkPointPass(&this->mPathRun, &this->parent.parent.mCurrent.mPos, this->mPathRun.mGoingForwards);
  if (reachedCurrPoint) {
    dNpc_PathRun_c__nextIdxAuto(&this->mPathRun);   
  }
  
  // Calculate Y rotation towards the next path point.
  cXyz outPointPos;
  short outYRot;
  dNpc_PathRun_c__getPoint(&outPointPos, &this->mPathRun, this->mPathRun.mCurrPointIndex);
  dNpc_calc_DisXZ_AngY(&this->parent.parent.mCurrent.mPos, &outPointPos, 0, &outYRot);
  
  // Gradually turn towards the desired Y rotation (maximum 0x500 angle units = 7 degrees turned per frame).
  cLib_addCalcAngleS2(&this->parent.parent.mCurrent.mRot.y, outYRot, 1, 0x500);
  
  // Update position based on velocity and rotation.
  this->parent.parent.mVelocityFwd = 10.0f;
  fopAcM_posMoveF(&this->parent.parent, &this->parent.mStts.parent.mCcMove);
  
  // Correct this actor's position relative to Bg collision (walkable collision such as floors or walls it may have run into).
  dBgS_Acch__CrrPos(&this->parent.mObjAcch.parent, &g_dComIfG_gameInfo.mPlay.mBgS);
  
  // Update the model's transform.
  PSMTXTrans(this->parent.parent.mCurrent.mPos.x, this->parent.parent.mCurrent.mPos.y, this->parent.parent.mCurrent.mPos.z, &mDoMtx_stack_c__now);
  mDoMtx_YrotM(&mDoMtx_stack_c__now, this->parent.parent.mCurrent.mRot.y);
  PSMTXCopy(&mDoMtx_stack_c__now, &this->parent.mpMcaMorf->mpModel->mBaseMtx);
  
  // Calculate how the vertices should be deformed by the animation.
  mDoExt_McaMorf__calc(this->parent.mpMcaMorf);
  
  // Set the actor's Cc collision with a height of 50 and a radius of 140.
  fopNpc_npc_c__setCollision(&this->parent, 50.0f, 140.0f);
  
  return 1;
}

int daNPCTest__next_msgStatus(NPC_Test_class* this, ulong* msgIDPtr) {
  return fopNpc_npc_c__MessageStatus__Finished;
}

ulong daNPCTest__getMsg(NPC_Test_class* this) {
  if(dKy_daynight_check()) {
	  return 0;
  }
  
  return 1;
}

void daNPCTest__anmAtr(NPC_Test_class* this, ushort unk) {
  return;
}

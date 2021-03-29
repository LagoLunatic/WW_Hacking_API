#pragma once

#include "f_pc_profile.h"

namespace f_op_actor {
  
  class fopAc_ac_c : class leafdraw_class {
  public:
  // Variables
    /* 0xC0 */ int mAcType; // 0x04 bytes
    /* 0xC4 */ struct create_tag_class mAcTg; // 0x14 bytes
    /* 0xD8 */ struct create_tag_class mDwTg; // 0x14 bytes
    /* 0xEC */ struct profile_method_class * mSubMtd; // 0x04 bytes
    /* 0xF0 */ struct JKRHeap * mpHeap; // 0x04 bytes
    /* 0xF4 */ struct dEvt_info_c mEvtInfo; // 0x18 bytes
    /* 0x10C */ struct dKy_tevstr_c mTevStr; // 0xB0 bytes
    /* 0x1BC */ short mSetID; // 0x02 bytes
    /* 0x1BE */ enum fopAc_ac_c__Type mActorType; // 0x01 bytes
    /* 0x1BF */ char mCullType; // 0x01 bytes
    /* 0x1C0 */ char mDemoObjId; // 0x01 bytes
    /* 0x1C1 */ char mSubtype; // 0x01 bytes
    /* 0x1C2 */ enum GbaNames mGbaName; // 0x01 bytes
    /* 0x1C3 */ char field_0x1c3; // 0x01 bytes
    /* 0x1C4 */ enum fopAcM__Status mStatus; // 0x04 bytes
    /* 0x1C8 */ enum fopAc_ac_c__Condition mCondition; // 0x04 bytes
    /* 0x1CC */ int mParentPcId; // 0x04 bytes
    /* 0x1D0 */ struct actor_place mOrig; // 0x14 bytes
    /* 0x1E4 */ struct actor_place mNext; // 0x14 bytes
    /* 0x1F8 */ struct actor_place mCurrent; // 0x14 bytes
    /* 0x20C */ struct csXyz mCollisionRot; // 0x06 bytes
    /* 0x212 */ char field_0x212; // 0x01 bytes
    /* 0x213 */ char field_0x213; // 0x01 bytes
    /* 0x214 */ struct cXyz mScale; // 0x0C bytes
    /* 0x220 */ struct cXyz mVelocity; // 0x0C bytes
    /* 0x22C */ struct MTX34 * mpCullMtx; // 0x04 bytes
    /* 0x230 */ struct cXyz mCullSizeBoxMin; // 0x0C bytes
    /* 0x23C */ struct cXyz mCullSizeBoxMax; // 0x0C bytes
    /* 0x248 */ float mCullFarDistanceRatio; // 0x04 bytes
    /* 0x24C */ struct J3DModel * mpGrabbedShadowModel; // 0x04 bytes /* Used by daPy_lk_c::drawShadow */
    /* 0x250 */ struct JntHit_c * mpArrowStopJntHit; // 0x04 bytes /* Used by daArrow_c::setStopActorMatri */
    /* 0x254 */ float mVelocityFwd; // 0x04 bytes
    /* 0x258 */ float mAccelerationY; // 0x04 bytes
    /* 0x25C */ float mVelocityYMin; // 0x04 bytes
    /* 0x260 */ struct cXyz mEyePos; // 0x0C bytes
    /* 0x26C */ char mAttentionDistances[8]; // 0x08 bytes
    /* 0x274 */ struct cXyz mAttentionPos; // 0x0C bytes
    /* 0x280 */ enum fopAc_ac_c__AttentionFlags mAttentionFlags; // 0x04 bytes
    /* 0x284 */ char mMaxHp; // 0x01 bytes
    /* 0x285 */ char mCurrHp; // 0x01 bytes
    /* 0x286 */ char field_0x286; // 0x01 bytes
    /* 0x287 */ char field_0x287; // 0x01 bytes
    /* 0x288 */ int mItemTableIdx; // 0x04 bytes
    /* 0x28C */ char mItemStealNum; // 0x01 bytes
    /* 0x28D */ char mItemStealLeft; // 0x01 bytes
    /* 0x28E */ char field_0x28e; // 0x01 bytes
    /* 0x28F */ char field_0x28f; // 0x01 bytes

  // Functions
    fopAc_ac_c();
    ~fopAc_ac_c();
  }
  
  int fopAc_IsActor(fopAc_ac_c *pActor);
  void fopAc_Draw(fopAc_ac_c *pActor);
  f_pc_profile::profile_method_class * fopAc_Execute(fopAc_ac_c *pActor);
  int fopAc_IsDelete(fopAc_ac_c *pActor);
  int fopAc_Delete(fopAc_ac_c *pActor);
  int fopAc_Create(fopAc_ac_c *pActor);
}
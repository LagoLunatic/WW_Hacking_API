#pragma once

namespace f_pc_profile {
  
  enum fopAcM__Status {
    DoNotDrawNextFrame=16777216,
    IsBeingCarried=8192,
    IsBossOrMiniBoss=67108864,
    AppearsOnMiniMap=32,
    DoNotExecuteIfDidNotDraw=128,
    DoesNotPause=131072,
    DoNotDrawIfCulled=256,
    Frozen=1024,
    None=0
  };

  enum fopAc_ac_c__Type { /* The Enemy type is checked in a number of different places, the other types are never checked. */
      Regular=0,
      Enemy=2,
      Wildlife_and_misc=3,
      Link=1,
      Some_NPCs=4
  };

  struct profile_leaf_method_class {
    /* 0x00 */ int (* mpCreate)(); // 0x04 bytes
    /* 0x04 */ int (* mpDelete)(); // 0x04 bytes
    /* 0x08 */ int (* mpExecute)(); // 0x04 bytes
    /* 0x0C */ int (* mpIsDelete)(); // 0x04 bytes
    /* 0x10 */ int (* mpDraw)(); // 0x04 bytes
  };
  
  struct profile_method_class {
    /* 0x00 */ profile_leaf_method_class base;
    /* 0x14 */ int (* mpUnkFunc1)(); // 0x04 bytes
    /* 0x18 */ int (* mpUnkFunc2)(); // 0x04 bytes
    /* 0x1C */ int (* mpUnkFunc3)(); // 0x04 bytes
  };
  
  struct Profile_Base {
    /* 0x00 */ int mLayerID; // 0x04 bytes
    /* 0x04 */ unsigned short mListID; // 0x02 bytes /* Affects execution order of actors in a given frame. Lower numbers execute first. */
    /* 0x06 */ unsigned short mListPrio; // 0x02 bytes
    /* 0x08 */ unsigned short mPName; // 0x02 bytes
    /* 0x0A */ char field_0xa; // 0x01 bytes
    /* 0x0B */ char field_0xb; // 0x01 bytes
    /* 0x0C */ profile_leaf_method_class * mpMtd0; // 0x04 bytes
    /* 0x10 */ int mSize; // 0x04 bytes
    /* 0x14 */ int mSizeOther; // 0x04 bytes
    /* 0x18 */ int mDefaultParameters; // 0x04 bytes
    /* 0x1C */ profile_method_class * mpMtd1; // 0x04 bytes /* This is the method table for Ovlp. See 803720b0 */
  };
  
  struct Profile_Actor {
    /* 0x00 */ Profile_Base base;
    /* 0x20 */ short mDrawPriority; // 0x02 bytes
    /* 0x22 */ char field_0x22; // 0x01 bytes
    /* 0x23 */ char field_0x23; // 0x01 bytes
    /* 0x24 */ profile_method_class * mpMtd2; // 0x04 bytes
    /* 0x28 */ fopAcM__Status mStatus; // 0x04 bytes
    /* 0x2C */ fopAc_ac_c__Type mActorType; // 0x01 bytes
    /* 0x2D */ char mCullType; // 0x01 bytes
    /* 0x2E */ char field_0x2e; // 0x01 bytes
    /* 0x2F */ char field_0x2f; // 0x01 bytes
  };
  
  Profile_Actor* fpcPf_Get(short param_1);
  
}
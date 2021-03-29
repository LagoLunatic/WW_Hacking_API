#pragma once

#include "f_op_actor.h"

namespace d_npc {
  
  class fopNpc_npc_c : f_op_actor::fopAc_ac_c {
  // Variables
    struct dNpc_JntCtrl_c mJntCtrl; // 0x34 bytes
    struct dNpc_EventCut_c mEventCut; // 0x64 bytes
    char field_0x328; // 0x01 bytes
    char field_0x329; // 0x01 bytes
    char field_0x32a; // 0x01 bytes
    char field_0x32b; // 0x01 bytes
    int field_0x32c; // 0x04 bytes
    struct mDoExt_McaMorf * mpMcaMorf; // 0x04 bytes
    struct dBgS_ObjAcch mObjAcch; // 0x1C4 bytes
    struct dBgS_AcchCir mAcchCir; // 0x40 bytes
    struct dCcD_Stts mStts; // 0x3C bytes
    struct dCcD_Cyl mCyl;
    unsigned long mCurrMsgID;
    unsigned long mEndMsgID;
    int mCurrMsgBsPcId;
    struct msg_class * mpCurrMsg;
    char field_0x6b4;
    char field_0x6b5;
    char field_0x6b6;
    char field_0x6b7;
    char field_0x6b8;
    char field_0x6b9;
    char field_0x6ba;
    char field_0x6bb;
    char field_0x6bc;
    char field_0x6bd;
    char field_0x6be;
    char field_0x6bf;
    
  // Functions
    int next_msgStatus(unsigned long *param_1);
    int getMsg();
    void anmAtr(unsigned short param_1);
    void setCollision(float radius, float height);
    int talk(int param_1);
  }
  
}
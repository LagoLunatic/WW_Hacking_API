
enum SwOpLogicalOperation {
  SwOp_AND=0,
  SwOp_NAND=1,
  SwOp_OR=2,
  SwOp_NOR=3,
  SwOp_XOR=4,
  SwOp_XNOR=5,
};

enum SwOpState {
  SwOpState_CheckConditionMet=0,
  SwOpState_Timer=1,
  SwOpState_StartingEvent=2,
  SwOpState_DuringEvent=3,
  SwOpState_CheckConditionUnmet=4,
  SwOpState_Deactivated=5,
};

typedef struct SwitchOperator_class {
  fopAc_ac_c parent;
  
  enum SwOpState mState;
  
  enum SwOpLogicalOperation mOperation;
  bool mContinuous;
  
  u8 mSwitchToSet;
  u8 mFirstSwitchToCheck;
  u8 mNumSwitchesToCheck;
  
  u8 mEVNTIndexToStart;
  s16 mEventIndexToStart;
  
  u8 mTotalFramesToWait;
  u8 mRemainingFramesToWait;
} SwitchOperator_class;

int daSwOp_Create(SwitchOperator_class* this);
int daSwOp_IsDelete(SwitchOperator_class* this);
int daSwOp_Delete(SwitchOperator_class* this);
int daSwOp_Draw(SwitchOperator_class* this);
int daSwOp_Execute(SwitchOperator_class* this);

void daSwOp__conditionMetCheck(SwitchOperator_class* this);
void daSwOp__timerCountdown(SwitchOperator_class* this);
void daSwOp__eventStartCheck(SwitchOperator_class* this);
void daSwOp__eventEndCheck(SwitchOperator_class* this);
void daSwOp__conditionUnmetCheck(SwitchOperator_class* this);

bool daSwOp__isCondition(SwitchOperator_class* this);

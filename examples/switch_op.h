
enum SwOpLogicalOperation {
  SwOp_AND=0,
  SwOp_NAND=1,
  SwOp_OR=2,
  SwOp_NOR=3,
  SwOp_XOR=4,
  SwOp_XNOR=5,
};

typedef struct SwitchOperator_class {
  fopAc_ac_c parent;
  
  enum SwOpLogicalOperation mOperation;
  bool mContinuous;
  
  u8 mSwitchToSet;
  u8 mFirstSwitchToCheck;
  u8 mNumSwitchesToCheck;
  
  u8 mEVNTIndexToStart;
  s16 mEventIndexToStart;
  u8 mEventProgressState;
  
  u8 mTotalFramesToWait;
  u8 mRemainingFramesToWait;
  
  bool isDisabled;
} SwitchOperator_class;

int daSwOp_Create(SwitchOperator_class* this);
int daSwOp_IsDelete(SwitchOperator_class* this);
int daSwOp_Delete(SwitchOperator_class* this);
int daSwOp_Draw(SwitchOperator_class* this);
int daSwOp_Execute(SwitchOperator_class* this);

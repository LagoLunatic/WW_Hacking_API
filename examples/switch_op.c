
#include "../vanilla_defines/ww_defines.h"
#include "./switch_op.h"

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


int daSwOp_Create(SwitchOperator_class* this) {
  this->mOperation           = (this->parent.parent.parent.mParameters & 0x0000000F);
  this->mContinuous          = (this->parent.parent.parent.mParameters & 0x00000010) >> 4;
  this->mSwitchToSet         = (this->parent.parent.parent.mParameters & 0x0000FF00) >> 8;
  this->mFirstSwitchToCheck  = (this->parent.parent.parent.mParameters & 0x00FF0000) >> 0x10;
  this->mNumSwitchesToCheck  = (this->parent.parent.parent.mParameters & 0xFF000000) >> 0x18;
  this->mEVNTIndexToStart    = (this->parent.mOrig.mRot.x & 0x00FF);
  this->mTotalFramesToWait   = (this->parent.mOrig.mRot.z & 0xFFFF);
  
  this->mEventIndexToStart = dEvent_manager_c__getEventIdx(&g_dComIfG_gameInfo.mPlay.mEventMgr, 0, this->mEVNTIndexToStart);
  
  // Initialize the counter.
  this->mRemainingFramesToWait = this->mTotalFramesToWait;
  
  if (this->mSwitchToSet == 0xFF) {
    this->mState = SwOpState_Deactivated;
  } else {
    bool switchIsAlreadySet = dSv_info_c__isSwitch(&g_dComIfG_gameInfo.mSvInfo, this->mSwitchToSet, this->parent.mCurrent.mRoomNo);
    if (switchIsAlreadySet) {
      if (this->mContinuous) {
        this->mState = SwOpState_CheckConditionUnmet;
      } else {
        this->mState = SwOpState_Deactivated;
      }
    } else {
      this->mState = SwOpState_CheckConditionMet;
    }
  }
  
  
  return cPhs_COMPLEATE_e;
}

int daSwOp_IsDelete(SwitchOperator_class* this) {
  return 1;
}

int daSwOp_Delete(SwitchOperator_class* this) {
  return 1;
}

int daSwOp_Draw(SwitchOperator_class* this) {
  return 1;
}

int daSwOp_Execute(SwitchOperator_class* this) {
  switch (this->mState) {
    case SwOpState_CheckConditionMet:
      daSwOp__conditionMetCheck(this);
      break;
    case SwOpState_Timer:
      daSwOp__timerCountdown(this);
      break;
    case SwOpState_StartingEvent:
      daSwOp__eventStartCheck(this);
      break;
    case SwOpState_DuringEvent:
      daSwOp__eventEndCheck(this);
      break;
    case SwOpState_CheckConditionUnmet:
      daSwOp__conditionUnmetCheck(this);
      break;
    case SwOpState_Deactivated:
      // Do nothing.
      break;
  }
  
  return 1;
}

void daSwOp__conditionMetCheck(SwitchOperator_class* this) {
  bool conditionMet = daSwOp__isCondition(this);
  if (conditionMet) {
    if (this->mTotalFramesToWait > 0) {
      // If there's a delay to wait, let the timer state take care of setting the switch.
      this->mState = SwOpState_Timer;
    } else if (this->mEventIndexToStart != -1) {
      // If there's an event to start, let the event state take care of setting the switch.
      this->mState = SwOpState_StartingEvent;
    } else {
      // Otherwise, set the switch directly.
      dSv_info_c__onSwitch(&g_dComIfG_gameInfo.mSvInfo, this->mSwitchToSet, this->parent.mCurrent.mRoomNo);
      if (this->mContinuous) {
        this->mState = SwOpState_CheckConditionUnmet;
      } else {
        this->mState = SwOpState_Deactivated;
      }
    }
  }
}

void daSwOp__timerCountdown(SwitchOperator_class* this) {
  if (this->mContinuous) {
    bool conditionMet = daSwOp__isCondition(this);
    if (!conditionMet) {
      // Condition no longer met, something changed while we were waiting for the timer.
      // Reset the counter and go back to checking if the condition is met.
      this->mRemainingFramesToWait = this->mTotalFramesToWait;
      this->mState = SwOpState_CheckConditionMet;
      return;
    }
  }
  
  if (this->mRemainingFramesToWait <= 0) {
    // Timer has finished counting down.
    if (this->mEventIndexToStart != -1) {
      // If there's an event to start, let the event state take care of setting the switch.
      this->mState = SwOpState_StartingEvent;
    } else {
      // Otherwise, set the switch directly.
      dSv_info_c__onSwitch(&g_dComIfG_gameInfo.mSvInfo, this->mSwitchToSet, this->parent.mCurrent.mRoomNo);
      if (this->mContinuous) {
        // For a SwOp that is both continuous and has a timer, we simply deactivate it once the
        // countdown has successfully reached zero.
        // This is so puzzles where you have to keep the condition met for a certain period of time
        // don't reset after you have met the condition for that period of time.
        this->mState = SwOpState_Deactivated;
      }
    }
    return;
  }
  
  // Count down.
  this->mRemainingFramesToWait--;
}

void daSwOp__eventStartCheck(SwitchOperator_class* this) {
  if (this->parent.mEvtInfo.mCommand == dEvt__ActorCommand__InDemo) {
    // The event has started. Now we have to wait for it to end.
    this->mState = SwOpState_DuringEvent;
    // We also set this SwOp's switch now.
    // This allows us to know not to start this event again in the future if the room is reloaded.
    dSv_info_c__onSwitch(&g_dComIfG_gameInfo.mSvInfo, this->mSwitchToSet, this->parent.mCurrent.mRoomNo);
    return;
  }
  
  if (this->mContinuous) {
    bool conditionMet = daSwOp__isCondition(this);
    if (!conditionMet) {
      // Condition no longer met, something changed while we were waiting for the event.
      // Stop trying to start the event and go back to checking if the condition is met.
      this->mState = SwOpState_CheckConditionMet;
      return;
    }
  }
  
  // Ask to start the event.
  // We call this every frame until the event actually starts.
  // This can take a while if, we're trying to start the event while another event is ongoing.
  // Doing it this way guarantees that this event will play as soon as the other event has finished,
  // with no delay between them.
  fopAcM_orderOtherEventId(&this->parent, this->mEventIndexToStart, this->mEVNTIndexToStart, 0xFFFF, 0, 1);
}

void daSwOp__eventEndCheck(SwitchOperator_class* this) {
  bool eventFinished = dEvent_manager_c__endCheck(&g_dComIfG_gameInfo.mPlay.mEventMgr, this->mEventIndexToStart);
  if (eventFinished) {
    if (this->mContinuous) {
      this->mState = SwOpState_CheckConditionUnmet;
    } else {
      this->mState = SwOpState_Deactivated;
    }
    // Setting this flag indicates this event should be finished, and the game should go back to
    // checking if any other events are ready to play.
    g_dComIfG_gameInfo.mPlay.mEvtCtrl.mStateFlags |= 8;
  }
}

void daSwOp__conditionUnmetCheck(SwitchOperator_class* this) {
  // This state is only used if the "continuous" param is set to true.
  bool conditionMet = daSwOp__isCondition(this);
  if (!conditionMet) {
    // Condition no longer met, something changed while we were waiting.
    // Unset this SwOp's switch and go back to checking if the condition is met.
    this->mState = SwOpState_CheckConditionMet;
    dSv_info_c__offSwitch(&g_dComIfG_gameInfo.mSvInfo, this->mSwitchToSet, this->parent.mCurrent.mRoomNo);
  }
}

bool daSwOp__isCondition(SwitchOperator_class* this) {
  u8 switchToCheck = this->mFirstSwitchToCheck;
  int numSet = 0;
  int numUnset = 0;
  for (int i = 0; i < this->mNumSwitchesToCheck; i++) {
    bool currSwitchIsSet = dSv_info_c__isSwitch(&g_dComIfG_gameInfo.mSvInfo, switchToCheck, this->parent.mCurrent.mRoomNo);
    
    if (currSwitchIsSet) {
      numSet++;
    } else {
      numUnset++;
    }
    
    switchToCheck++;
  }
  
  bool conditionMet = false;
  if (this->mOperation == SwOp_AND && numUnset == 0) {
    conditionMet = true;
  } else if (this->mOperation == SwOp_NAND && numUnset > 0) {
    conditionMet = true;
  } else if (this->mOperation == SwOp_OR && numSet > 0) {
    conditionMet = true;
  } else if (this->mOperation == SwOp_NOR && numSet == 0) {
    conditionMet = true;
  } else if (this->mOperation == SwOp_XOR && numSet == 1) {
    conditionMet = true;
  } else if (this->mOperation == SwOp_XNOR && numSet != 1) {
    conditionMet = true;
  }
  
  return conditionMet;
}


profile_method_class l_daSwOp_Method = {
  .parent = {
    .mpCreate = &daSwOp_Create,
    .mpDelete = &daSwOp_Delete,
    .mpExecute = &daSwOp_Execute,
    .mpIsDelete = &daSwOp_IsDelete,
    .mpDraw = &daSwOp_Draw,
  },
  .mpUnkFunc1 = 0,
  .mpUnkFunc2 = 0,
  .mpUnkFunc3 = 0,
};

const f_pc_profile__Profile_Actor g_profile_SwitchOperator = {
  .parent = {
    .mLayerID = -3,
    .mListID = 7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    .mListPrio = -3,
    .mPName = 0x1F6, // Actor ID
    0,
    0,
    .mpMtd0 = &g_fpcLf_Method,
    .mSize = sizeof(SwitchOperator_class),
    .mSizeOther = 0,
    .mDefaultParameters = 0,
    .mpMtd1 = &g_fopAc_Method,
  },
  
  .mDrawPriority = 0x9F,
  0,
  0,
  .mpMtd2 = &l_daSwOp_Method,
  .mStatus = 0,
  .mActorType = fopAc_ac_c__Type__Regular,
  .mCullType = 0,
  0,
  0,
};

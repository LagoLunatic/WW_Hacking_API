#include "../vanilla_defines/ww_defines.h"
#include "RelDefines.h"

#define RES_NAME "Daytimer"
#define TIMER_BLO_NAME "daytimer.blo"

struct fopNpc_npc_c;
struct fopAc_ac_c;
enum PhaseState;


/* *** Begin Struct Definitions *** */

// Displaylist class for the clock's UI elements
struct dDlst_clock_c : dDlst_base_c {
  virtual void draw() override;
  
  J2DScreen * mpClockUI;
  
  J2DTextBox * mpTimeText;
  J2DTextBox * mpTimeTextShadow;
};

void dDlst_clock_c::draw() {
  mpClockUI->draw(0.0, 0.0, &g_dComIfG_gameInfo.mp2DOrthoGraph);
}

// Main actor for updating and drawing the clock
struct daClock : fopAc_ac_c {
  PhaseState _create();
  bool _delete();
  bool _execute();
  bool _draw();
  
private:
  request_of_phase_process_class mPhaseRequest;
  JKRArchive * mpTimerResArc;
  dDlst_clock_c mClockDlst;
  
  PhaseState _createUI();
};

PhaseState daClock::_create() {
  // Make a request to load the resource archive and wait until it's been loaded.
  PhaseState resLoadState = dComIfG_resLoad(&mPhaseRequest, RES_NAME);
  if (resLoadState != cPhs_COMPLEATE_e) {
    return resLoadState;
  }
  
  dRes_info_c * resInfo = dRes_control_c::getResInfo(RES_NAME, g_dComIfG_gameInfo.mResCtrl.mObjectInfo, 0x40);
  mpTimerResArc = resInfo->mpArchive;
  
  return _createUI();
}

PhaseState daClock::_createUI() {
  J2DScreen * clockScreen = new J2DScreen();
  if (clockScreen == 0) {
    OSReport("daClock: Failed to initialize J2DScreen!");
    return cPhs_ERROR_e;
  }
  
  clockScreen->set(TIMER_BLO_NAME, mpTimerResArc);
  
  J2DTextBox * mainTextBox = (J2DTextBox *)clockScreen->search(0x74696D57);
  if (mainTextBox == 0) {
    OSReport("daClock: Failed to find main time textbox!");
    return cPhs_ERROR_e;
  }
  
  mainTextBox->mpFont->mMonospaceFlag = true;
  mainTextBox->mpFont->mMonospaceWidth = 15;
  
  J2DTextBox * shadowTextBox = (J2DTextBox *)clockScreen->search(0x74696D42);
  if (shadowTextBox == 0) {
    OSReport("daClock: Failed to find time shadow textbox!");
    return cPhs_ERROR_e;
  }
  
  shadowTextBox->mpFont->mMonospaceFlag = true;
  shadowTextBox->mpFont->mMonospaceWidth = 15;
  
  mClockDlst.mpClockUI = clockScreen;
  mClockDlst.mpTimeText = mainTextBox;
  mClockDlst.mpTimeTextShadow = shadowTextBox;
  
  return cPhs_COMPLEATE_e;
}

bool daClock::_delete() {
  delete(mClockDlst.mpClockUI);
  dComIfG_resDelete(&mPhaseRequest, RES_NAME);
  
  return true;
}

bool daClock::_execute() {
  int curMin = dKy_getdaytime_minute();
  
  // The game returns the hour in 24hr format, so let's make it 12hr instead.
  int curHour = dKy_getdaytime_hour() % 12;
  if (curHour == 0) {
    curHour = 12;
  }
  
  snprintf(mClockDlst.mpTimeText->mpStringPtr, 6, "%02d:%02d", curHour, curMin);
  snprintf(mClockDlst.mpTimeTextShadow->mpStringPtr, 6, "%02d:%02d", curHour, curMin);
  
  return true;
}

bool daClock::_draw() {
  g_dComIfG_gameInfo.mDlstList.set(&g_dComIfG_gameInfo.mDlstList.mp2DOpa,
                                   &g_dComIfG_gameInfo.mDlstList.mp2DOpaEnd,
                                   &mClockDlst);

  return true;
}

/* *** End Struct Definitions *** */


/* *** Begin Profile Definitions *** */

// Interface functions for the game engine to call
PhaseState daClock_Create(fopAc_ac_c *pActor) {
  daClock * actorCast = (daClock *)pActor;
  return actorCast->_create();
}

bool daClock_Delete(fopAc_ac_c *pActor) {
  daClock * actorCast = (daClock *)pActor;
  return actorCast->_delete();
}

bool daClock_Execute(fopAc_ac_c *pActor) {
  daClock * actorCast = (daClock *)pActor;
  return actorCast->_execute();
}

bool daClock_IsDelete(fopAc_ac_c *pActor) {
  return true;
}

bool daClock_Draw(fopAc_ac_c *pActor) {
  daClock * actorCast = (daClock *)pActor;
  return actorCast->_draw();
}

// Function pointer table so the engine knows how to call the above functions
profile_method_class l_daClock_Method {
  {
    &daClock_Create,
    &daClock_Delete,
    &daClock_Execute,
    &daClock_IsDelete,
    &daClock_Draw,
  },
  
  0,
  0,
  0
};

// Information that the engine needs to know about the actor
const f_pc_profile__Profile_Actor g_profile_daClock = {
  {
    -3,
    7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    -3,
    0xB5, // Actor ID
    0,
    0,
    &g_fpcLf_Method,
    sizeof(daClock),
    0,
    0,
    &g_fopAc_Method,
  },
  
  0x9F,
  0,
  0,
  &l_daClock_Method,
  fopAcM__Status__None,
  fopAc_ac_c__Type__Some_NPCs,
  0,
  0,
  0
};

/* *** End Profile Definitions *** */

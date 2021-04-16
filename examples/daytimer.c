#define RES_NAME "Daytimer"
#define TIMER_BLO_NAME "daytimer.blo"
#define TIMER_VAL 450

#include "../vanilla_defines/ww_defines.h"
#include "./daytimer.h"

static void * _ctors SECTION(".ctors");
static void * _dtors SECTION(".dtors");

static void* dlst_vtbl[] = {
    NULL, NULL, // rtti placeholder
    dDlst_base_c__dDlst_base_c_destructor, // parent destructor
    daDayTimerDLst__draw,
};

int daDayTimer__Create(DayTimer_class* this) {
  // Load the archive.
  PhaseState phaseState = dComIfG_resLoad(&this->mPhaseRequest, RES_NAME);
  if (phaseState != cPhs_COMPLEATE_e) {
    // Not finished loading yet, check again next frame.
    return phaseState;
  }
  
  // Load our resource archive and store a pointer to it
  dRes_info_c* resInfo = dRes_control_c__getResInfo(RES_NAME, g_dComIfG_gameInfo.mResCtrl.mObjectInfo, 0x40);
  this->mTimerResArc = resInfo->mpArchive;
  
  daDayTimer__CreateUI(this);
  
  return cPhs_COMPLEATE_e;
}

void daDayTimer__CreateUI(DayTimer_class* this) {
  this->mDLst.mClockUI = (J2DScreen*)JKernel__operator_new(sizeof(J2DScreen));
  if (this->mDLst.mClockUI != 0) {
    // Rectangle that defines the bounds of our parent J2DPane.
    // Covers the entire screen (640x480)
    TBox2__float__ rect = {
      .mTL.x = 0.0f,
      .mTL.y = 0.0f,
      .mBR.x = 640.0f,
      .mBR.y = 480.0f
    };
    
    this->mDLst.parent.vtbl = dlst_vtbl;
    
    // Create the parent J2DPane object for our UI elements.
    J2DPane__J2DPane(&this->mDLst.mClockUI->parent);
    this->mDLst.mClockUI->parent.mName = 0x726f6f74; // 0x726f6f74 = 'root'
    this->mDLst.mClockUI->parent.mPosTL = rect.mTL;
    this->mDLst.mClockUI->parent.mPosBR = rect.mBR;
    this->mDLst.mClockUI->field_0xcf = 0;
    
    // This is sort of a hack because this is a C toolkit. We need to manually set what vtable our
    // UI element uses. Even though mClockUI is a J2DScreen object, and its parent is a J2DPane,
    // we set the parent to use J2DScreen's vtable since J2DScreen is a child class of J2DPane.
    this->mDLst.mClockUI->parent.vtbl = &J2DScreen____vt;
    
    // Initialize our J2DScreen object with the BLO from our resource archive.
    J2DScreen__set(this->mDLst.mClockUI, TIMER_BLO_NAME, this->mTimerResArc);
    
    // Get a reference to the main textbox to update the time string (0x74696D57 = 'timW')
    this->mDLst.mTimeText = (J2DTextBox*)J2DPane__search((J2DPane*)this->mDLst.mClockUI, 0x74696D57);
    if (!this->mDLst.mTimeText) {
      OSReport("Failed to find time textbox!\n");
    }
    
    this->mDLst.mTimeText->mpFont->parent.field_0x5 = 1; // Set monospace flag
    this->mDLst.mTimeText->mpFont->parent.field_0x8 = 15; // Set monospace width to 15px
    
    // Get a reference to the dropshadow textbox to update the time string (0x74696D42 = 'timB')
    this->mDLst.mTimeTextShadow = (J2DTextBox*)J2DPane__search((J2DPane*)this->mDLst.mClockUI, 0x74696D42);
    if (!this->mDLst.mTimeTextShadow) {
      OSReport("Failed to find time shadow textbox!\n");
    }
    
    this->mDLst.mTimeTextShadow->mpFont->parent.field_0x5 = 1; // Set monospace flag
    this->mDLst.mTimeTextShadow->mpFont->parent.field_0x8 = 15; // Set monospace width to 15px
  }
}

int daDayTimer__createSolidHeap_CB(DayTimer_class* this) {
  return 1;
}

int daDayTimer__Execute(DayTimer_class* this) {  
  int CurMin = dKy_getdaytime_minute();
  
  // The game returns the hour in 24hr format, so let's make it 12hr instead.
  int CurHour = dKy_getdaytime_hour() % 12;
  if (CurHour == 0) {
    CurHour = 12;
  }
  
  // Update the time strings via snprintf (like printf but writes to a char buffer)
  MSL_C_PPCEABI_bare_H__snprintf(this->mDLst.mTimeText->mpStringPtr, 6, "%02d:%02d", CurHour, CurMin);
  MSL_C_PPCEABI_bare_H__snprintf(this->mDLst.mTimeTextShadow->mpStringPtr, 6, "%02d:%02d", CurHour, CurMin);
  
  return 1;
}

int daDayTimer__IsDelete(DayTimer_class* this) {
  return 1;
}

int daDayTimer__Delete(DayTimer_class* this) {
  J2DScreen__J2DScreen_destructor(this->mDLst.mClockUI);
  JKernel__operator_delete((JKRHeap *)this->mDLst.mClockUI);
  
  dComIfG_resDelete(&this->mPhaseRequest, RES_NAME);
  
  return 1;
}

int daDayTimer__Draw(DayTimer_class* this) {
  dDlst_list_c__set(&g_dComIfG_gameInfo.mDlstList,
                    &g_dComIfG_gameInfo.mDlstList.mp2DOpa,
                    &g_dComIfG_gameInfo.mDlstList.mp2DOpaEnd,
                    &this->mDLst);
  
  return 1;
}

void daDayTimerDLst__draw(DayTimer_Dlst_class* this) {
  J2DScreen__draw(this->mClockUI, 0.0, 0.0, &g_dComIfG_gameInfo.mp2DOrthoGraph->parent);
}

/** REL LINK FUNCTIONS **/
/** Called to link and unlink the REL. Don't change these! **/
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

/** CONSTANTS **/
profile_method_class l_daDayTimer_Method = {
  .parent = {
    .mpCreate = &daDayTimer__Create,
    .mpDelete = &daDayTimer__Delete,
    .mpExecute = &daDayTimer__Execute,
    .mpIsDelete = &daDayTimer__IsDelete,
    .mpDraw = &daDayTimer__Draw,
  },
  .mpUnkFunc1 = 0,
  .mpUnkFunc2 = 0,
  .mpUnkFunc3 = 0,
};

const f_pc_profile__Profile_Actor g_profile_DayTimer = {
  .parent = {
    .mLayerID = -3,
    .mListID = 7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    .mListPrio = -3,
    .mPName = 0xB5, // Actor ID
    0,
    0,
    .mpMtd0 = &g_fpcLf_Method,
    .mSize = sizeof(DayTimer_class),
    .mSizeOther = 0,
    .mDefaultParameters = 0,
    .mpMtd1 = &g_fopAc_Method,
  },
  
  .mDrawPriority = 0x9F,
  0,
  0,
  .mpMtd2 = &l_daDayTimer_Method,
  .mStatus = 0,
  .mActorType = fopAc_ac_c__Type__Regular,
  .mCullType = 0,
  0,
  0,
};
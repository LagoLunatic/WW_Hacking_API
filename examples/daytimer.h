/** A clock actor that displays the current in-game time. **/
/** Adds its own UI element to the screen via a BLO file. **/

typedef struct DayTimer_Dlst_class {
  dDlst_base_c parent;
  
  J2DScreen* mClockUI; // Pointer to the base UI element for the clock
  
  J2DTextBox* mTimeText; // Pointer to the textbox that displays the main time text
  J2DTextBox* mTimeTextShadow; // Pointer to the textbox that displays the time's dropshadow
  
} DayTimer_Dlst_class;

typedef struct DayTimer_class {
  fopAc_ac_c parent;
  request_of_phase_process_class mPhaseRequest;
  
  JKRArchive* mTimerResArc; // Pointer to our resource archive (BLO, images)
  DayTimer_Dlst_class mDLst;

} DayTimer_class;

/** INTERFACE FUNCTIONS **/
/** Called directly by the game for construction, destruction, and updating. **/
int daDayTimer__Create(DayTimer_class* this);
int daDayTimer__Execute(DayTimer_class* this);
int daDayTimer__Draw(DayTimer_class* this);
int daDayTimer__IsDelete(DayTimer_class* this);
int daDayTimer__Delete(DayTimer_class* this);

/** CONSTRUCTION FUNCTIONS **/
/** These set up our actor when it's first created. **/
void daDayTimer__daDayTimer(DayTimer_class* this);
int daDayTimer__createSolidHeap_CB(DayTimer_class* this);
void daDayTimer__CreateUI(DayTimer_class* this);

void daDayTimerDLst__draw(DayTimer_Dlst_class* this);

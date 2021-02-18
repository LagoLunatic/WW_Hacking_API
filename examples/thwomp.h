
typedef struct THWOMP_class {
  fopAc_ac_c parent;
  
  request_of_phase_process_class mPhaseRequest;
  mDoExt_3DlineMat0_c mLine;
  J3DModel* mpModel;
  dBgW* mpCollision;
  //dBgS_AcchCir mAcchCir;
  //dBgS_Acch mCollisionChecker;
  MTX34 mMtx;
  
  bool sawPlayer;
  bool playerIsRiding;
  int framesUntilStartChase;
  bool isChasing;
} THWOMP_class;

int daThwomp_Create(THWOMP_class* this);
int daThwomp_IsDelete(THWOMP_class* this);
int daThwomp_Delete(THWOMP_class* this);
int daThwomp_Draw(THWOMP_class* this);
int daThwomp_Execute(THWOMP_class* this);

void THWOMP_test_pushpull(fopAc_ac_c* this, fopAc_ac_c * otherActor, short unk3, PushPullLabel unk4);
int createSolidHeap_CB(THWOMP_class* this);

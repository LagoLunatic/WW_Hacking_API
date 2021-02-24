
typedef struct NPC_Test_class {
  fopNpc_npc_c parent;
  dNpc_EventCut_c eventActor;
  
  request_of_phase_process_class mPhaseRequest;
  dNpc_PathRun_c mPathRun;
  
  int mCurrActionIndex;
  s8 mCurrCutIdx;
  
  short mMaxFollowRotVel;
  s8 mHeadJntIdx;
  s8 mSpineJntIdx;
  
  dItem_data__ItemNo mXyzItemId;
} NPC_Test_class;

struct JntCtrl_Params {
  short mMaxHeadRot;
  short mMinHeadRot;
  short mMaxSpineRot;
  short mMinSpineRot;
  
  short unk_0x00;
  short unk_0x04;
  short unk_0x08;
  short unk_0x0C;
  short unk_0x10;
  
  short mDesiredFollowRotVel;
  
  float mAttArrowYOffset;
  
  bool headOnlyFollow;
};

/** INTERFACE FUNCTIONS **/
/** Called directly by the game for construction, destruction, and updating. **/
int daNPCTest_Create(NPC_Test_class* this);
int daNPCTest_Execute(NPC_Test_class* this);
int daNPCTest_Draw(NPC_Test_class* this);
int daNPCTest_IsDelete(NPC_Test_class* this);
int daNPCTest_Delete(NPC_Test_class* this);

/** CONSTRUCTION FUNCTIONS **/
/** These set up our actor when it's first created. **/
void daNPCTest__daNPCTest(NPC_Test_class* this);
int daNPCTest_createSolidHeap_CB(NPC_Test_class* this);
void daNPCTest__InitCollision(NPC_Test_class* this);
int daNPCTest__InitPath(NPC_Test_class* this);
void daNPCTest__InitJntCtrl(NPC_Test_class* this, J3DModelData* modelData);

/** EXECUTION STATE FUNCTIONS **/
/** These handle the actor's frame to frame execution state - whether it's waiting, walking, talking, etc. **/
void daNPCTest__wait_action(NPC_Test_class* this);
int daNPCTest__talk(NPC_Test_class* this);
void daNPCTest__UpdatePathFollowing(NPC_Test_class* this);
void daNPCTest__setMtx(NPC_Test_class* this, bool unk);

/** ATTENTION FUNCTIONS **/
/** These handle Link's ability to interact with the actor - L-targeting, talking to it, etc. as well as the NPC's ability to react to Link's presence. **/
bool daNPCTest__chkAttention(NPC_Test_class* this);
void daNPCTest__setAttention(NPC_Test_class* this, bool unk);
int daNPCTest_nodeCallBack(J3DNode* node, int unk);
void daNPCTest__lookBack(NPC_Test_class* this);

/** MESSAGE FUNCTIONS **/
/** In NPCs that can speak, these handle the flow of the conversation. **/
ulong daNPCTest__getMsg(NPC_Test_class* this);
int daNPCTest__next_msgStatus(NPC_Test_class* this, ulong* msgIDPtr);

/** This allows NPCs to check certain conditions to decide if they will accept being talked to with items on the X/Y/Z buttons. Returns true if they will, false if they won't.
The itemSlotIndex passed in is the index of the EquippedItem that the player used to initiate the conversation. **/
bool daNPCTest__XyzCheckCB(NPC_Test_class* this, int itemSlotIndex);
/**  **/
bool daNPCTest__ChkTalk(NPC_Test_class* this);

/** EVENT FUNCTIONS **/
/** These handle how the actor participates in the game's event system, like being talked to or having event actions executed. **/
void daNPCTest__eventOrder(NPC_Test_class* this);
void daNPCTest__checkOrder(NPC_Test_class* this);
void daNPCTest__event_proc(NPC_Test_class* this);
void daNPCTest__privateCut(NPC_Test_class* this);
void daNPCTest__event_actionInit(NPC_Test_class* this, int staffId);
void daNPCTest__event_action(NPC_Test_class* this);
void daNPCTest__endEvent(NPC_Test_class* this);

/** MISC FUNCTIONS **/
/** These don't really fit in a single category. **/
void daNPCTest__anmAtr(NPC_Test_class* this, ushort unk);

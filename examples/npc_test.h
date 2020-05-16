
typedef struct NPC_Test_class {
  fopNpc_npc_c parent;
  dNpc_EventCut_c eventActor;
  
  request_of_phase_process_class mPhaseRequest;
  dNpc_PathRun_c mPathRun;
} NPC_Test_class;

void _prolog();
void _epilog();
void _unresolved();
int daNPCTest_Create(NPC_Test_class* this);
int daNPCTest_IsDelete(NPC_Test_class* this);
int daNPCTest_Delete(NPC_Test_class* this);
int daNPCTest_Draw(NPC_Test_class* this);
int daNPCTest_Execute(NPC_Test_class* this);
int daNPCTest__next_msgStatus(NPC_Test_class* this, ulong* msgIDPtr);
ulong daNPCTest__getMsg(NPC_Test_class* this);
void daNPCTest__anmAtr(NPC_Test_class* this, ushort unk);

profile_method_class l_daNPCTest_Method = {
  .parent = {
    .mpCreate = &daNPCTest_Create,
    .mpDelete = &daNPCTest_Delete,
    .mpExecute = &daNPCTest_Execute,
    .mpIsDelete = &daNPCTest_IsDelete,
    .mpDraw = &daNPCTest_Draw,
  },
  .mpUnkFunc1 = 0,
  .mpUnkFunc2 = 0,
  .mpUnkFunc3 = 0,
};

const f_pc_profile__Profile_Actor g_profile_NPC_Test = {
  .parent = {
    .mLayerID = -3,
    .mListID = 7, // Affects execution order of actors in a given frame. Lower numbers execute first.
    .mListPrio = -3,
    .mPName = 0xB5, // Actor ID
    0,
    0,
    .mpMtd0 = &g_fpcLf_Method,
    .mSize = sizeof(NPC_Test_class),
    .mSizeOther = 0,
    .mDefaultParameters = 0,
    .mpMtd1 = &g_fopAc_Method,
  },
  
  .mDrawPriority = 0x9F,
  0,
  0,
  .mpMtd2 = &l_daNPCTest_Method,
  .mStatus = 0,
  .mActorType = fopAc_ac__Type__Some_NPCs,
  .mCullType = 0,
  0,
  0,
};

const fopNpc_npc_c__vtbl daNpc_Test_c_vtbl = {
  0,
  0,
  .next_msgStatus = (pointer)&daNPCTest__next_msgStatus,
  .getMsg = (pointer)&daNPCTest__getMsg,
  .anmAtr = (pointer)&daNPCTest__anmAtr,
};

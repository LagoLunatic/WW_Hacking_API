#include "../../vanilla_defines/ww_defines.h"

void CreateHotestFunc(){
     cXyz scale = {1.0f, 1.0f, 1.0f};
     cXyz position = {0.0f, 0.0f, 0.0f};;
     csXyz angle = {0.0f, 0.0f, 0.0f};
     int *stageNo =(int *)0x803C53A4;
     //int roomNo = dStage_roomControl_c__mStayNo;
     static int has_run = 0;
     if (*stageNo == 0){
          if(!has_run){
          fopAcM_create(0x00B5, 0, &position, dStage_roomControl_c__mStayNo, &angle, &scale, 0xFF, 0);
          has_run = 1;
          }
     }
     else{
     fopAcM_create(0x00B5, 0, &position, dStage_roomControl_c__mStayNo, &angle, &scale, 0xFF, 0);
     }
}

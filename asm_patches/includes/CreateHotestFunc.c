#include "../../vanilla_defines/ww_defines.h"

void CreateHotestFunc(){
     cXyz scale = {1.0f, 1.0f, 1.0f};
     cXyz position = {0.0f, 0.0f, 0.0f};;
     csXyz angle = {0.0f, 0.0f, 0.0f};
     fopAcM_create(0x00B5, 0, &position, dStage_roomControl_c__mStayNo, &angle, &scale, 0xFF, 0);
}


#include "../../vanilla_defines/ww_defines.h"

void CreateHotestFunc(){
     cXyz scale = {1.0f, 1.0f, 1.0f};                  // Initialize scale, position and angle
     cXyz position = {0.0f, 0.0f, 0.0f};;
     csXyz angle = {0.0f, 0.0f, 0.0f};
     static byte sumCount = 0x00;                      // Initialize a counter for how many actors load from Fa.arc

     for(int i=0; i<=63; i++){
     byte *byte1 = (byte *)(0x803e0bc8 + i*0x24);      // List of loaded resource archives from res/Object starts at 0x803e0bc8
     byte *byte2 = (byte *)(0x803e0bc9 + i*0x24);      // Every object takes 36 bytes (36=0x24)
     byte *byte3 = (byte *)(0x803e0bca + i*0x24);      // The first seven bytes tell the resource name. In our case FA = 46 61 00 00 00 00 00
     byte *byte4 = (byte *)(0x803e0bcb + i*0x24);      
     byte *byte5 = (byte *)(0x803e0bcc + i*0x24);      // We iterate trough the list
     byte *byte6 = (byte *)(0x803e0bcd + i*0x24);
     byte *byte7 = (byte *)(0x803e0bce + i*0x24);
     byte *count = (byte *)(0x803e0bd7 + i*0x24);      // The 15th and 16th bytes tell how many actors load from this resource. For us the 16th byte is enough.

                                                       // If FA arc is loaded, and at least one actor loads from it, sum the amount of actors doing this.
          if((*byte1 == 0x46 && *byte2 == 0x61 && *byte3 == 0x00 && *byte4 == 0x00 && *byte5 == 0x00 && *byte6 == 0x00 && *byte7 == 0x00) && (*count != 0x00)){
               sumCount = sumCount + *count;
          }
     
     }


     if(sumCount == 0x00){                             // Create the hotest actor (0x00b5) only if no actor loads from Fa.arc
          fopAcM_create(0x00B5, 0, &position, dStage_roomControl_c__mStayNo, &angle, &scale, 0xFF, 0);
          //hasBeenCreated = false;
     }

     else{
          sumCount = 0x00;                             // Set sumCount to zero for, so that the actor can be generated on room transitions
     }
}
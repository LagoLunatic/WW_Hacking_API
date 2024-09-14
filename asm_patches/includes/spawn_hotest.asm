.open "sys/main.dol"
 .org 0x80043238                 ; In dStage_dt_c_roomReLoader
 bl CreateHotest

 .org @NextFreeSpace
 .global CreateHotest
 CreateHotest:
                                 ; Since we have subroutines, we need to store our link register
 stwu sp, -0x10(sp)              ; Save the stack frame
 mflr r0                         ; Move the LR into r0 
 stw r0, 0x14(sp)                ; Store LR in the stack

                                 ; First we call bl	0x80328F8C since we overwrote this call.
 lis   r12, 0x8032               ; Load the upper 16 bits of the address 0x8032 into r12
 ori   r12, r12, 0x8F8C          ; Load the lower 16 bits of the address 0x8F6C into r12
 mtlr  r12                       ; Move the target address from r12 into the link register 
 blrl                            ; Branch to the address in the link register and link back

 bl CreateHotestFunc             ; We call our custom function stored in "includes/CreateHotestFunc.c"

 lwz r0, 0x14(sp)                ; Restore LR from the stack
 mtlr r0                         ; Move the value back into the LR
 addi sp, sp, 0x10               ; Restore the stack frame
 blr                             ; Return

 .include "includes/CreateHotestFunc.c"
.close





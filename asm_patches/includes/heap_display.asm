
; Enables the heap display left in the game's code for viewing how much memory is free in real time.

.open "sys/boot.bin"
; Change a variable in the ISO header to allow the heap display to be used.
.org 0x07
  .byte 0x91
.close

.open "sys/main.dol"
; Default the heap display to on when booting up the game so it doesn't need to be toggled on with R+Z on controller 3.
.org 0x800063E7
  .byte 0x01
; Default tab of the heap display to 1 instead of 4 so it doesn't need to be changed with L+Z on controller 3.
.org 0x803F60E8
  .byte 0x01
; Remove a check that a controller must be connected to port 3 for the heap display to be shown.
.org 0x800084A0
  nop
.close

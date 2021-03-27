
; nop out a couple lines so the long intro movie is skipped.
.open "sys/main.dol"
.org 0x80232C78
  nop
.org 0x80232C88
  nop
.close

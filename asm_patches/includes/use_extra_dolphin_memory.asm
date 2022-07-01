
; This patch updates the game's code so that it can make use of more than the 24MB of RAM the GameCube has.
; To enable this feature in Dolphin:
; * Go to Config -> Advanced -> Memory Override
; * Tick the checkbox that says "Enable Emulated Memory Size Override"
; * Change the MEM1 size to 32MB
; Note that this is not usable on real hardware, so any hack that uses this will be emulator-only.

; Change these values to customize how the new memory gets distributed to different heaps.
; The numbers are in bytes, but you can edit just the first number for MB.
; Note: The System heap will just take however much memory is left unused by the other heaps.
; e.g. If 8MB is added in total, with 2MB to the Archive heap and 5MB to the Game heap, the System heap gets 1MB added to it.
; Note: It's not recommended to add more than 8MB in total, as this will cause bugs like glitched palettes or crashes.
.set added_total_size,        8 * 1024*1024
.set added_command_heap_size, 0 * 1024*1024
.set added_archive_heap_size, 2 * 1024*1024
.set added_game_heap_size,    5 * 1024*1024

.set vanilla_command_heap_size, 0x00001000
.set vanilla_archive_heap_size, 0x00A51400
.set vanilla_game_heap_size,    0x002CE800

.set command_heap_size, vanilla_command_heap_size + added_command_heap_size
.set archive_heap_size, vanilla_archive_heap_size + added_archive_heap_size
.set game_heap_size, vanilla_game_heap_size + added_game_heap_size
.set command_archive_game_heaps_combined_size, command_heap_size+archive_heap_size+game_heap_size

.set new_total_mem1_size, 24 * 1024*1024 + added_total_size
.set mem1_end_address, 0x80000000 + new_total_mem1_size

.open "sys/bi2.bin"
; Change a variable in the ISO header to allow the heap display to be used.
.org 0x04
  .int new_total_mem1_size
.close

.open "sys/main.dol"
.org 0x8000C7A4 ; In mDoMch_Create(void)
  lis r0, mem1_end_address@h
.org 0x8000C9D0 ; In mDoMch_Create(void)
  li r3, command_heap_size
.org 0x8000C9DC ; In mDoMch_Create(void)
  lis r3, archive_heap_size@ha
  addi r3, r3, archive_heap_size@l
.org 0x8000C9EC ; In mDoMch_Create(void)
  lis r3, game_heap_size@ha
  addi r3, r3, game_heap_size@l
.org 0x8000C844 ; In mDoMch_Create(void)
  ; Update the System heap size by taking the negation of the other three combined
  addis r3, r31, -command_archive_game_heaps_combined_size@ha
  addi r0, r3, -command_archive_game_heaps_combined_size@l
.close


.open "sys/main.dol"

; Load the BGM file after System.arc is finished loading.
.org 0x8022DC20 ; In phase_1(dScnLogo_c *)
  b load_bgm_file

; Replace all the static variable loads of the list variables to instead call functions that dynamically return the list variable in the BGM file.

; Replace loads of JAIZelBasic::m_scene_info(void)
.org 0x802AA6E0 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA708 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA774 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA794 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA800 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA820 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA8B4 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA948 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA990 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AA9D4 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAA18 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAA7C ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAAC0 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAB04 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAB48 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAB8C ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AABFC ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAC58 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAC78 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  nop
.org 0x802AAC90 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_stage_bgm_info_list
  mr r0, r3

; Replace checks that hardcode the number of stage BGM infos to instead load the count from th efile.
.org 0x802AAECC
  bl compare_r29_to_stage_entry_count_minus_1
.org 0x802AAED8
  bl compare_r29_to_stage_entry_count_minus_1
.org 0x802AA3D4
  bl compare_r30_to_stage_entry_count

; Replace loads of JAIZelBasic::m_isle_info(void)
.org 0x802A2C0C ; In JAIZelBasic::zeldaGFrameWork(void)
  bl get_island_bgm_info_list
  nop
.org 0x802AA30C ; In JAIZelBasic::startIsleBgm(void)
  bl get_island_bgm_info_list
  nop
.org 0x802AA510 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop
.org 0x802AA558 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop
.org 0x802AA5A0 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop
.org 0x802AA5E8 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop
.org 0x802AA608 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop
.org 0x802AA620 ; In JAIZelBasic::setScene(long, long, long, long)
  bl get_island_bgm_info_list
  nop

; Replace loads of JAIZelBasic::m_dy_wave_set_1st(void)
.org 0x802AB468 ; In JAIZelBasic::load1stDynamicWave(void)
  bl get_first_dynamic_scene_wave_list
  nop
.org 0x802AB540 ; In JAIZelBasic::load1stDynamicWave(void)
  bl get_first_dynamic_scene_wave_list
  mr r4, r3
.org 0x802AB69C ; In JAIZelBasic::check1stDynamicWave(void)
  bl get_first_dynamic_scene_wave_list
  mr r4, r3

; Replace loads of JAIZelBasic::m_dy_wave_set_2nd(void)
.org 0x802AB3DC ; In JAIZelBasic::load1stDynamicWave(void)
  bl get_second_dynamic_scene_wave_list
  nop
.org 0x802AB7F4 ; In JAIZelBasic::load2ndDynamicWave(void)
  bl get_second_dynamic_scene_wave_list
  mr r4, r3

; Replace loads of JAIZelBasic::spot_dir_name(void)
.org 0x802AAEAC ; In JAIZelBasic::spotNameToId(char *)
  bl get_bgm_spot_name_list
  mr r30, r3

.org @NextFreeSpace

.global load_bgm_file
load_bgm_file:
  stw r30, -29840(r13) ; Replace the line of code we overwrote to jump here
  
  ;lis r3, 0x8036280C@ha ; "System" string
  ;addi r3, r3, 0x8036280C@l
  ;li r4, 5 ; File index for the BGM file
  ;lis r5, 0x803E0BC8@ha ; Object resources list
  ;addi r5, r5, 0x803E0BC8@l
  ;li r6, 0x40 ; Max loaded object resources
  ;bl dRes_control_c__getRes
  ;mr. r30, r3
  ;bne bgm_file_loaded_successfully
  
  lis r3, 0x8036280C@ha ; "System" string
  addi r3, r3, 0x8036280C@l
  lis r4, bgm_file_name@ha ; "bgm_dat.bin" string
  addi r4, r4, bgm_file_name@l
  lis r5, 0x803E0BC8@ha ; Object resources list
  addi r5, r5, 0x803E0BC8@l
  li r6, 0x40 ; Max loaded object resources
  bl dRes_control_c__getNameRes
  mr. r30, r3
  bne bgm_file_loaded_successfully
  
  ; Failed to load the BGM file. Show an assertion error.
  bl JUTAssertion__getSDevice
  
  lis r4, 0x80362820@ha
  addi r4, r4, 0x80362820@l ; "d_s_logo.cpp"
  li r5, 0 ; Line number, just put a dummy value
  lis r6, bgm_file_is_not_0_assertion_message@ha
  addi r6, r6, bgm_file_is_not_0_assertion_message@l
  bl JUTAssertion__showAssert
  
  lis r3, 0x80362820@ha
  addi r3, r3, 0x80362820@l ; "d_s_logo.cpp"
  li r4, 0 ; Line number, just put a dummy value
  lis r5, bgm_file_not_found_error_message@ha
  addi r5, r5, bgm_file_not_found_error_message@l
  crclr 4*cr1+eq ; Clear the EQ bit of CR1 (unknown why this is done)
  bl OSPanic

bgm_file_loaded_successfully:
  ; Store the pointer as a global variable so it can be more easily accessed later.
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  stw r30, 0 (r3)
  
  ; We also need to convert the string offsets in the spot names list to pointers instead.
  lwz r3, 0x00 (r30) ; Stage entry count
  mtctr r3
  lwz r3, 0x1C (r30) ; Spot names list offset
  add r3, r3, r30 ; Convert offset to pointer
  
convert_spot_name_offset_to_pointer_loop_start:
  lwz r4, 0 (r3) ; Read the stage name string offset
  add r4, r4, r30 ; Convert offset to pointer
  stw r4, 0 (r3) ; Store the stage name string pointer
  
  addi r3, r3, 4
  bdnz convert_spot_name_offset_to_pointer_loop_start
  
  ; Return
  b 0x8022DC24


.global get_stage_bgm_info_list
get_stage_bgm_info_list:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r4, 0x04 (r3) ; Stage BGM info list offset
  add r3, r3, r4 ; Convert offset to pointer
  blr

.global get_island_bgm_info_list
get_island_bgm_info_list:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r4, 0x0C (r3) ; Island BGM info list offset
  add r3, r3, r4 ; Convert offset to pointer
  blr

.global get_first_dynamic_scene_wave_list
get_first_dynamic_scene_wave_list:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r4, 0x10 (r3) ; 1st dynamic scene wave list offset
  add r3, r3, r4 ; Convert offset to pointer
  blr

.global get_second_dynamic_scene_wave_list
get_second_dynamic_scene_wave_list:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r4, 0x14 (r3) ; 2nd dynamic scene wave list offset
  add r3, r3, r4 ; Convert offset to pointer
  blr

.global get_bgm_spot_name_list
get_bgm_spot_name_list:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r4, 0x1C (r3) ; Spot names list
  add r3, r3, r4 ; Convert offset to pointer
  blr

.global compare_r29_to_stage_entry_count_minus_1
compare_r29_to_stage_entry_count_minus_1:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r3, 0 (r3) ; Stage BGM info count
  subi r3, r3, 1
  cmpw r29, r3
  blr

.global compare_r30_to_stage_entry_count
compare_r30_to_stage_entry_count:
  lis r3, bgm_file_pointer@ha
  addi r3, r3, bgm_file_pointer@l
  lwz r3, 0 (r3)
  lwz r3, 0 (r3) ; Stage BGM info count
  cmpw r30, r3
  blr


; Placeholder for a global variable pointing to the custom BGM file, for easy access.
.global bgm_file_pointer
bgm_file_pointer:
  .int 0

.global bgm_file_name
bgm_file_name:
  .string "bgm_dat.bin"
.global bgm_file_is_not_0_assertion_message
bgm_file_is_not_0_assertion_message:
  .string "bgmFile != 0"
.global bgm_file_not_found_error_message
bgm_file_not_found_error_message:
  .string "BGM file not found. System.arc must have a file named 'bgm_dat.bin'."

.close

#pragma once

#include "f_pc_base.h"
#include "f_pc_profile.h"

namespace f_pc_leaf {

  class leafdraw_class : base_process_class {
    /* 0xB8 */ f_pc_profile::profile_leaf_method_class * mpSubMtd; // 0x04 bytes
    /* 0xBC */ char field_0xbc; // 0x01 bytes
    /* 0xBD */ char field_0xbd; // 0x01 bytes
    /* 0xBE */ struct draw_priority_class mDwPi; // 0x02 bytes
  }

}
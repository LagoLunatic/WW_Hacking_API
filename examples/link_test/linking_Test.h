#include "dDlst_base_c.h"

class dDlst_derive_c : dDlst_base_c {
public:
    dDlst_derive_c() {}
    virtual void draw() override;
    virtual int test_func() override;
};
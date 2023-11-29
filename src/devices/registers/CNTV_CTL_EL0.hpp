#pragma once
#include "base/defines.hpp"

namespace registers::CNTV_CTL_EL0 {
    STANDARD_FUNCTIONS(CNTV_CTL_EL0);

    FIELD_BIT_FCT(ISTATUS, 2);
    FIELD_BIT_FCT(IMASK, 1);
    FIELD_BIT_FCT(ENABLE, 0);
}

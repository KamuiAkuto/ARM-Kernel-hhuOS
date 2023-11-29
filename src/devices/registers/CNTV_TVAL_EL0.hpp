#pragma once
#include "base/defines.hpp"


namespace registers::CNTV_TVAL_EL0 {
    STANDARD_FUNCTIONS(CNTV_TVAL_EL0);

    FIELD_FUNCTIONS(TimerValue, 31, 0);
}

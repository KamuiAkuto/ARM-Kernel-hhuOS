#pragma once

#include "registers/CNTV_TVAL_EL0.hpp"
#include "registers/CNTV_CTL_EL0.hpp"
#include "registers/CNTV_CVAL_EL0.hpp"

#include "peripherals/gic.hpp"

#define TIMER_IRQ     27
#define TIMESLOT_TIME 0xfffffff

namespace devices::Sytemtimer
{

    namespace {
        uint64_t time = 0;
    }

    namespace timer = registers::CNTV_CTL_EL0;
    namespace value = registers::CNTV_TVAL_EL0;
    namespace gic = peripherals::GIC;

    inline void init() {
        gic::configIrq(TIMER_IRQ, gic::trigger::edge);
        gic::setPriority(TIMER_IRQ, 0);
        gic::setTarget(TIMER_IRQ, gic::cpuTarget::interface0);
        gic::clearPending(TIMER_IRQ);
        gic::enableInt(TIMER_IRQ);
    }

    inline void rest() {
        timer::set(0);
        value::set(TIMESLOT_TIME);
        time++;
        timer::IMASK(false);
        timer::ENABLE(true);
    }

    inline void start() {
        init();
        rest(); 
    }

    /*inline uint64_t getSystemTimeExact() {
        return time + value::get();
    }*/

    inline uint64_t getSystemTime() {
        return time;
    }
}

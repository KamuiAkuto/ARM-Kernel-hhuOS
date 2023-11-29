#pragma once

#include "Thread.hpp"

namespace kernel::dispatcher {
    inline void contextSwitch(Thread* next) {
        auto currentProcess = (Thread*)registers::TPIDR_EL0::get();

        //get stack pointer and use x28
        __asm __volatile__ ("mrs x28, SP_EL0");
        //save exception return address
        __asm __volatile__ ("mrs x0, ELR_EL1");
        __asm __volatile__ ("str X0, [x28, #-8]!");

        //save stack pointer to current pcb
        __asm __volatile__ ("mov %[sp], x28" : [sp] "=r" (currentProcess->stackPointer));

        registers::TPIDR_EL0::set((int64_t)next);

        //load stack pointer from next pcb
        __asm __volatile__ ("MOV x28, %[sp]" : /*no out*/ : [sp] "r" (next->stackPointer));

        //load exception return address
        __asm __volatile__ ("ldr X0, [x28], #8");
        __asm __volatile__ ("msr ELR_EL1, x0");
        //set stack pointer
        __asm __volatile__ ("msr SP_EL0, x28");
    }
}

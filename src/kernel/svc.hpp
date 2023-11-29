#pragma once

#include <stdint.h>

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wreturn-type"

#define VAL_S(...) #__VA_ARGS__
#define TOSTRING_S(...) VAL_S(__VA_ARGS__)

#define SVC_COMMAND(number)      __asm volatile (TOSTRING_S(svc number))

#define startOs_NUM         0x00
#define exit_NUM            0x01
#define halt_NUM            0x02

#define waitingOnLock_NUM   0x10
#define signalUnlock_NUM    0x11

#define schedulerBlock_NUM  0x20
#define schedulerDeblock_NUM  0x21

#define createThread_NUM    0x30
#define waitOnThread_NUM    0x31
#define getReturn_NUM       0x32
#define yield_NUM           0x33
#define killThread_NUM      0x34
#define createThreadNoStack_NUM    0x35

namespace SVC {
    inline void startOs() {
        SVC_COMMAND(startOs_NUM);
    }
    
    extern "C" inline void exit(int exitCode) {
        SVC_COMMAND(exit_NUM);
        //wait for context switch
        while(1);
    }

    inline void halt() {
        SVC_COMMAND(halt_NUM);
        //wait for context switch
        while(1);
    }

    inline void waitingOnLock() {
        SVC_COMMAND(waitingOnLock_NUM);
    }

    inline void signalUnlock() {
        SVC_COMMAND(signalUnlock_NUM);
    }

    inline void schedulerBlock() {
        SVC_COMMAND(schedulerBlock_NUM);
    }

    inline void schedulerDeblock(uint64_t id) {
        SVC_COMMAND(schedulerDeblock_NUM);
    }
}

#pragma GCC diagnostic pop
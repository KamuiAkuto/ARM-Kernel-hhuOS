#pragma once
#include <stdint.h>

#include "src/devices/registers/TPIDR_EL0.hpp"
#include "src/kernel/thread.hpp"

#include "src/kernel/svc.hpp"

#include "src/devices/timer.hpp"

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wreturn-type"

namespace SVC {
    inline uint64_t createThread(int64_t sectorBegin, int64_t stackTop) {
        SVC_COMMAND(createThread_NUM);
    }

    inline uint64_t createThread(int64_t sectorBegin) {
        SVC_COMMAND(createThread_NUM);
    }

    inline bool waitOnThread(uint64_t id) {
        SVC_COMMAND(waitOnThread_NUM);
    }

    inline uint64_t getReturn(uint64_t id, int64_t* returnVale) {
        SVC_COMMAND(getReturn_NUM);
    }

    inline uint64_t killThread(uint64_t) {
        SVC_COMMAND(killThread_NUM);
    }
}

#pragma GCC diagnostic pop

namespace lib {
    namespace thread {
        typedef uint64_t id;

         inline uint64_t createThread(int64_t sectorBegin, int64_t stackTop) {
            return SVC::createThread(sectorBegin, stackTop);
        }

    }

    namespace this_thread {
        inline void yield() {
            SVC_COMMAND(yield_NUM);
        }

        inline thread::id get_id() {
            auto th = (kernel::Thread*)registers::TPIDR_EL0::get();
            return th->pid;
        }

        // returns 0 if error, becuase 0 pid is reversed
        inline uint64_t join(uint64_t id, int64_t& returnVale) {
            auto res = SVC::waitOnThread(id);
            if(res) {
                return SVC::getReturn(id, &returnVale);
            }

            return 0;
        }

        // returns 0 if error, becuase 0 pid is reversed
        inline uint64_t join(uint64_t id) {
            auto res = SVC::waitOnThread(id);
            int64_t returnVale;
            if(res) {
                return SVC::getReturn(id, &returnVale);
            }

            return 0;
        }

        inline uint64_t stop(uint64_t id) {
            return SVC::killThread(id);
        }

        inline void delay(int ticks) {

            using devices::Sytemtimer::getSystemTime;

            auto expected = getSystemTime() + ticks;
            
            if(expected > getSystemTime()) {
                for( ; expected > getSystemTime(); )
                    yield();
            } else {
                // timer overflowed
                for( ; expected < getSystemTime(); )
                    yield();
            }
        }
    }

    
}

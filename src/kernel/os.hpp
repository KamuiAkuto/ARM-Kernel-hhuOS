#pragma once

#include "src/devices/registers/TPIDR_EL0.hpp"
#include "src/devices/timer.hpp"

#include "src/lib/mutex.hpp"

#include "scheduler.hpp"
#include "thread.hpp"

extern volatile uintptr_t  _console_stack_top;
#define CONSOLE_STACK_ADD  (int64_t)& _console_stack_top
extern volatile uintptr_t  _console_start;
#define CONSOLE_BEGIN_ADD  (int64_t)& _console_start  

namespace kernel { 

    namespace currentThread = registers::TPIDR_EL0;
    using namespace devices;

    // private
    namespace {
        lib::list<Thread> processes;
    }

    // get the next unused id
    int generateId() {
        int expected = 1;
        auto lastMatch = processes.begin();
        bool jump = false;
        for (auto it = processes.begin(); it != processes.end(); ) {
            if (expected == it->pid) {
                expected++;

                if (jump) {
                    it = lastMatch;
                    jump = false;
                } else {
                    lastMatch = it;
                    ++it;
                }
            }
            else {
                ++it;
                jump = true;
            }
        }
        return expected;
    }

    uint64_t createThread(int64_t sectorBegin, int64_t stackTop) {
        auto id = generateId();
        processes.push_back( { State::created, id, sectorBegin, stackTop } );
        schedular::ready(processes.back());
        return id;
    }

    uint64_t createThread(int64_t sectorBegin) {
        auto id = generateId();
        processes.push_back( { State::created, id, sectorBegin } );
        schedular::ready(processes.back());
        return id;
    }

    Thread* getThread(uint64_t id) {
        for(auto &th : processes)
            if(th.pid == id)
                return &th;
        return nullptr;
    }

    inline void removeThread(Thread* th) {
        processes.remove(*th);
    }

    inline void schedulerDeblock(uint64_t id) {
        auto th = getThread(id);
        schedular::deblock(th);
    }

    inline void start() {
        peripherals::GIC::init();

        currentThread::set((int64_t)&idleThread);
        createThread(CONSOLE_BEGIN_ADD, CONSOLE_STACK_ADD);
        Sytemtimer::start(); 
    }

    inline void exitThread(int returnCode) {
        auto that = (Thread*)currentThread::get();

        schedular::exit();

        that->returnCode = returnCode;
        if(that->waitingParent != nullptr)
            schedular::deblock(that->waitingParent);
    }

    inline uint64_t killThread(uint64_t id) {
        auto that = getThread(id);
        if(that == nullptr) return 0;
        schedular::kill(that);
        processes.remove(*that);
        return id;
    }

    inline bool waitOnThread(uint64_t id) {
        lib::mutex mu;
        lib::lock_guard { mu };
        auto th = getThread(id);
        auto that = (Thread*)currentThread::get();

        if(th == nullptr) return false;

        if(th->state == State::terminated) 
            return true;

        th->waitingParent = that;

        schedular::block();
        // should be true will be checked again in getReturn
        return true;
    }

    inline uint64_t getReturn(uint64_t id, int64_t* returnValue) {
        auto th = getThread(id);
        auto that = (Thread*)currentThread::get();

        if(th == nullptr) return 0;

        if(th->state == State::terminated) {
            *returnValue = th->returnCode;
            removeThread(th);
            return id;
        }
        return -1;

    }

    inline void waitingOnLock() {
        schedular::yield();
    }

    inline void signalUnlock() {
        // nothing
    }

    inline void halt() {
        while (1)
        {
            
        }
        
    }

    inline void timerInterrupt() {
        Sytemtimer::rest();
        schedular::preempt();
    }

}

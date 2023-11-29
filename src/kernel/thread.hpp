#pragma once
#include <stdint.h>

#include "src/lib/queue.hpp"
#include "src/lib/list.hpp"

#define pushSp(x)        sp = sp - 1; *sp = x;  

namespace kernel {
    typedef enum {
        created = 0,
        //ready,
        running,
        waiting,
        blocked,
        terminated
    }State;
    
    struct Thread {
        State state;
        int64_t pid;
        int64_t stackPointer;
        
        // possible list of thread control blocks for multithreading per process

        int64_t returnCode;

        Thread * waitingParent = nullptr;

        //load the stack with need data for context switch
        int64_t preLoadStack(int64_t begin, int64_t stack) {
            auto sp = (int64_t*)stack;

            // push x30 (LR)
            pushSp((int64_t)SVC::exit);

            // push x0 to x29
            for (int i = 0; i < 30; i++)
            {
                pushSp(i);
            }

            // push v0 to v32
            // vector are 128 bit aka two push
            int t  = 0;
            for (int i = 0; i < 64; i = i +2)
            {
                pushSp(0);
                pushSp(t);
                t++;
            }

            // push save process status register
            pushSp(0x80000000);
            // push elr
            pushSp(begin);

            
            return (int64_t)sp;
        }

        Thread(State state, int64_t pid, int64_t begin, int64_t stackPointer) :
                state(state), pid(pid), stackPointer(preLoadStack(begin, stackPointer)) { }

        Thread(State state, int64_t pid) :
        	    state(state), pid(pid) { }

        Thread(State state, int64_t pid, int64_t begin) :
                state(state), pid(pid) {
            stackPointer = preLoadStack(begin, (int64_t)new int64_t[4000]);
        }

        Thread() {}

        inline bool operator==(const Thread& rhs) { 
            return (state == rhs.state) 
                    && (pid == rhs.pid) 
                    && (stackPointer == rhs.stackPointer);
        }
        inline bool operator!=(const Thread& rhs) { return !(*this == rhs); }

    };
}

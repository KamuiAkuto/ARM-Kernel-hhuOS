#pragma once

#include "src/devices/registers/TPIDR_EL0.hpp"

#include "src/lib/algorithm.hpp"

#include "dispatcher.hpp"
#include "thread.hpp"

extern volatile uintptr_t  _idle_stack_top;
#define IDLE_STACK_ADD  (int64_t)& _idle_stack_top
extern volatile uintptr_t  _idle_start;
#define IDLE_BEGIN_ADD  (int64_t)& _idle_start

namespace kernel {
    // private
    namespace {
        //not correct stack address will be overwritten by first context switch 
        Thread idleThread = { State::running, 0 };
    }
}

namespace kernel::schedular {

    // private
    namespace {
        lib::queue<Thread*> queue;
    }

    namespace currentProcess = registers::TPIDR_EL0;

    /*void schedule () {
        
    }*/
    
    // Thread in ReadyQueue eintragen
    void ready (Thread* that) {
        queue.push(that);
        that->state = State::waiting;
    }

    // Thread mit 'Gewalt' terminieren
    void kill (Thread* that) {
        if((Thread*)currentProcess::get() == that)
            return;
        that->state = State::terminated;
        queue.remove(that);

        Thread* next;
        auto nextOp = queue.front();
        if(nextOp != nullptr) {
            next = *nextOp;
            queue.pop();
        } else {
            next = &idleThread;
        }

        auto current = (Thread*)currentProcess::get();
        if(next == current) {
            return;
        }

        dispatcher::contextSwitch(next);
    }
    
    // Thread terminiert sich selbst
    void exit () {
        auto that = (Thread*)currentProcess::get();
        that->state = State::terminated;
        queue.remove(that);

        Thread* next;
        auto nextOp = queue.front();
        if(nextOp != nullptr) {
            next = *nextOp;
            queue.pop();
        } else {
            next = &idleThread;
        }

        auto current = (Thread*)currentProcess::get();
        if(next == current) {
            return;
        }

        dispatcher::contextSwitch(next);
    }

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield () {
        Thread* next;
        auto nextOp = queue.front();
        if(nextOp != nullptr) {
            next = *nextOp;
            queue.pop();
        } else {
            next = &idleThread;
        }

        auto current = (Thread*)currentProcess::get();
        if(next == current) {
            return;
        }
           
        next->state = State::running;

        // only queue if its not the idle and was running
        if(current->pid != 0 && current->state == State::running) {
            current->state = State::waiting;
            queue.push(current);
        }
        dispatcher::contextSwitch(next);
    }

    void preempt () {
        if(!queue.empty())
            yield();
        else if (((Thread*)currentProcess::get())->state == State::terminated)
            yield;
    }

    // Umschalten auf naechsten Thread, aber den 
    // Aufrufer nicht in die Ready-Queue einfuegen
    void block () {

        auto current = (Thread*)currentProcess::get();
        if(current->pid == 0) return;

        Thread* next;
        auto nextOp = queue.front();
        if(nextOp != nullptr) {
            next = *nextOp;
            queue.pop();
        } else {
            next = &idleThread;
        }
        
        if(next == current) {
            return;
        }
        
        current->state = State::blocked;
        next->state = State::running;
        dispatcher::contextSwitch(next);
    }
    
    // 'that' wieder in die Ready-Queue einfuegen
    void deblock (Thread* that) {
        if(that->pid == 0) return;
        that->state = State::waiting;
        queue.push(that);
    }
}

#pragma once
#include <stdint.h>

#define GICD 0x08000000
#define GICC 0x08010000

#define GICC_CTLR     *((volatile uint32_t*)(GICC + 0x000))   // CPU Interface Control Register
#define GICC_PMR      *((volatile uint32_t*)(GICC + 0x004))   // Interrupt Priority Mask Register
#define GICC_BPR      *((volatile uint32_t*)(GICC + 0x008))   // Binary Point Register
#define GICC_IAR      *((volatile uint32_t*)(GICC + 0x00C))   // Interrupt Acknowledge Register
#define GICC_EOIR     *((volatile uint32_t*)(GICC + 0x010))   // End of Interrupt Register

#define GICD_CTLR           *((volatile uint32_t*)(GICD + 0x000))                       // Distributor Control Register
#define GICD_ICFGR(n)       *((volatile uint32_t*)(uintptr_t)(GICD + 0xc00 + (4*(n))))  // Interrupt Configuration Registers
#define GICD_ICPENDR(n)     *((volatile uint32_t*)(uintptr_t)(GICD + 0x280 + (4*(n))))  // Interrupt Clear-Pending Registers
#define GICD_ISENABLER(n)   *((volatile uint32_t*)(uintptr_t)(GICD + 0x100 + (4*(n))))  // Interrupt Set-Enable Registers
#define GICD_ICENABLER(n)   *((volatile uint32_t*)(uintptr_t)(GICD + 0x180 + (4*(n))))  // Interrupt Clear-Enable Registers
#define GICD_ITARGETSR(n)   *((volatile uint32_t*)(uintptr_t)(GICD + 0x800 + (4*(n))))  // Interrupt Processor Targets Registers
#define GICD_IPRIORITYR(n)  *((volatile uint32_t*)(uintptr_t)(GICD + 0x400 + (4*(n))))  // Interrupt Priority Registers

//irq per register ICFGR
#define GICD_ICFGR_NUNBER       16
//irq per register IPRIORITY
#define GICD_IPRIORITY_NUMBER   4
//irq per register ITARGETSR
#define GICD_ITARGETSR_NUMBER   4

namespace peripherals::GIC {

    namespace {
        bool Initialized = false;
    }

    enum trigger {
        levelSensitive = 0x00,
        edge = 0x02,
    };

    enum cpuTarget {
        interface0 = (1<<0),
        interface1 = (1<<1),
        interface2 = (1<<2),
        interface3 = (1<<3),
        interface4 = (1<<4),
        interface5 = (1<<5),
        interface6 = (1<<6),
        interface7 = (1<<7),
    };
    
    const int SPURIOUS_INTID   = 1023;    // Spurious interrupts id 

    inline void gicdInit() {
        const int GIC_INT_MAX       = 64;

        GICD_CTLR = 0x00;   // Disable distributor
        
        const int GICD_INT_PER_REG  = 32;
        // disable and clear irqs
        for (uint32_t i = 0; (GIC_INT_MAX / GICD_INT_PER_REG) > i; i++){
            GICD_ICENABLER(i) = 0xFFFFFFFF;
            GICD_ICPENDR(i) = 0xFFFFFFFF;
        }

        // set interrupt priorities as the lowest            
        for (uint32_t i = 0; (GIC_INT_MAX / GICD_IPRIORITY_NUMBER) > i; i++) 
            GICD_IPRIORITYR(i) = 0xFFFFFFFF;

        // set target of shared peripherals to processor 0
        // the first spi is 32 => register 8 (32 / GICD_ITARGETSR_NUMBER)
        for (uint32_t i = 8; (GIC_INT_MAX / GICD_ITARGETSR_NUMBER ) > i; i++)
            GICD_ITARGETSR(i) |= cpuTarget::interface0;

        // set trigger type private peripheral interrupts level triggered
        // the first ppi is 16 => register 1 (16 / GICD_ICFGR_NUNBER)
        for (uint32_t i = 1; (GIC_INT_MAX / GICD_ICFGR_NUNBER) > i; i++)
            GICD_ICFGR(i) = trigger::levelSensitive;
        
        GICD_CTLR = 0x01;   // Enable distributor
    }

    inline void giccInit() {
        GICC_CTLR = 0x00;  //disable gicc

        GICC_PMR = 0xff;   //set priority to lowest
        GICC_BPR = 0x00;   //all interrupts in one group

        // IAR INTID MASK  = 0x3ff

        // clear active interrupts
        for (uint32_t pending_irq = (GICC_IAR & 0x3ff); pending_irq != SPURIOUS_INTID; pending_irq = (GICC_IAR & 0x3ff))
            GICC_EOIR = GICC_IAR;

        GICC_CTLR = 0x1;            //enable gicc
    }

    inline void configIrq(uint32_t irq, unsigned int config) {
        // GICD_ICFGR has 16 fields, each field has 2 bits
        uint32_t shift = (irq % GICD_ICFGR_NUNBER) * 2; 
        
        uint32_t value = GICD_ICFGR(irq / GICD_ICFGR_NUNBER);
        // Clear and Set the field corresponding to the irq
        GICD_ICFGR(irq / GICD_ICFGR_NUNBER) = (value = ~(0x03 << shift)) | (config << shift);
    }

    inline void setPriority(uint32_t irq, uint32_t prio) {
        // 8 bits per entry and 4 entries per register
        uint32_t shift = (irq % GICD_IPRIORITY_NUMBER) * 8;
        
        uint32_t value = GICD_IPRIORITYR(irq / GICD_IPRIORITY_NUMBER);
        GICD_IPRIORITYR(irq / GICD_IPRIORITY_NUMBER) = (value & ~(0xff << shift)) | (prio << shift);
    }

    inline void setTarget(uint32_t irq, uint32_t target) {
        // 8 bits per entry and 4 entries per register
        uint32_t shift = (irq % GICD_ITARGETSR_NUMBER) * 8;

        uint32_t value = GICD_ITARGETSR(irq / GICD_ITARGETSR_NUMBER);
        GICD_ITARGETSR(irq / GICD_ITARGETSR_NUMBER) = (value & ~(0xff << shift)) | (target << shift);
    }

    inline void enableInt(uint32_t irq) {
        // works because only write has effect so |= not needed
        // 32 entries per register
        GICD_ISENABLER(irq / 32) = (1 << ( irq % 32));
    }

    inline void clearPending(uint32_t irq) {
        // works because only write has effect so |= not needed
        // const int GICD_ISENABLER_BIT_NUMBER = 32;
        GICD_ICPENDR(irq / 32) = (1 << ( irq % 32));
    }

    inline void init() {
        // check if already init so it does
        // not change allready done settings
        if(Initialized) return;
        gicdInit();
        giccInit();
        Initialized = true;
    }

    inline uint32_t getCurrentINTID() {
        return GICC_IAR;
    }

    inline void writeEol(uint32_t intid) {
        GICC_EOIR = intid;
    }

}


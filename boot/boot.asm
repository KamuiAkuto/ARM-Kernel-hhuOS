.section ".text.boot"
.global _start

_start:
    // activate floating point access important do do first if any c code wants to use q registers
    mov x0,  #0x300000
    msr CPACR_EL1, x0

    //load os stack
    ldr x30, = stack_top
    mov sp, x30

    // load console sack pointer in el0 stack pointer
    // needed for the svc call
    ldr x30, = _idle_stack_top
    msr SP_EL0, x30

    //load vector e1 table
    ldr x0, = Vector_table_el1
    msr vbar_el1, x0

    // Instruction Synchronization Barrier
    // to make sure that the Instruction have been done
    // so that that the fpu is activated etc 
    ISB

    bl _init

    svc 0x00    //start os

    //-----------------------------------------------

    // load exception return address as beginning of the console section start
    // resets the stack
    ldr x30, = _idle_start
    msr ELR_EL1, x30
    
    // load console sack pointer in el0 stack pointer
    ldr x30, = _idle_stack_top
    msr SP_EL0, x30

    // load frame pointer
    mov x29, #0x00

    // load exit svc call function as a lr
    ldr x30, = exit

    //-----------------------------------------------

    // change SPSR_EL1 in a manner that 
    // after exception return the core switches from el1 to el0
    // the switch will also change the used stack pointer from el1 to el0
    ldr x0, =0b0000000000
    msr SPSR_EL1, x0

    // activate all interrupts
    msr DAIFClr, #0b10

    // trigger exception return
    eret

    bl _fini


.macro saveX0toX4 
str X0, [SP, #-8]!
STP X2, X1, [SP, #-16]!
STP X4, X3, [SP, #-16]!
.endm

.macro saveX5toX29
STP X6, X5, [SP, #-16]!
STP X8, X7, [SP, #-16]!
STP X10, X9, [SP, #-16]!
STP X12, X11, [SP, #-16]!
STP x14, X13, [SP, #-16]!
STP X16, X15, [SP, #-16]!
STP X18, X17, [SP, #-16]!
STP X20, X19, [SP, #-16]!
STP X22, X21, [SP, #-16]!
STP X24, X23, [SP, #-16]!
STP X26, X25, [SP, #-16]!
STP X28, X27, [SP, #-16]!
str X29, [SP, #-8]!
.endm

.macro saveV0toV31
STP Q1, Q0, [SP, #-32]!     //32 because 128 bit register not 64 bit ones
STP Q3, Q2, [SP, #-32]!
STP Q5, Q4, [SP, #-32]!
STP Q7, Q6, [SP, #-32]!
STP Q9, Q8, [SP, #-32]!
STP Q11, Q10, [SP, #-32]!
STP Q13, Q12, [SP, #-32]!
STP Q15, Q14, [SP, #-32]!
STP Q17, Q16, [SP, #-32]!
STP Q19, Q18, [SP, #-32]!
STP Q21, Q20, [SP, #-32]!
STP Q23, Q22, [SP, #-32]!
STP Q25, Q24, [SP, #-32]!
STP Q27, Q26, [SP, #-32]!
STP Q29, Q28, [SP, #-32]!
STP Q31, Q30, [SP, #-32]!
.endm

.macro restoreV31toV0
LDP Q31, Q30, [SP], #32
LDP Q29, Q28, [SP], #32
LDP Q27, Q26, [SP], #32
LDP Q25, Q24, [SP], #32
LDP Q23, Q22, [SP], #32
LDP Q21, Q20, [SP], #32
LDP Q19, Q18, [SP], #32
LDP Q17, Q16, [SP], #32
LDP Q15, Q14, [SP], #32
LDP Q13, Q12, [SP], #32
LDP Q11, Q10, [SP], #32
LDP Q9, Q8, [SP], #32
LDP Q7, Q6, [SP], #32
LDP Q5, Q4, [SP], #32
LDP Q3, Q2, [SP], #32
LDP Q1, Q0, [SP], #32
.endm

.macro restoreX29toX5
ldr X29, [SP], #8
LDP X28, X27, [SP], #16
LDP X26, X25, [SP], #16
LDP X24, X23, [SP], #16
LDP X22, X21, [SP], #16
LDP X20, X19, [SP], #16
LDP X18, X17, [SP], #16
LDP X16, X15, [SP], #16
LDP X14, X13, [SP], #16
LDP X12, X11, [SP], #16
LDP X10, X9, [SP], #16
LDP X8, X7, [SP], #16
LDP X6, X5, [SP], #16
.endm

.macro restoreX5toX0
LDP X4, X3, [SP], #16
LDP X2, X1, [SP], #16
ldr X0, [SP], #8
.endm

.macro callSavedLr, fct
msr SPSel, #0b0

str x30, [SP, #-8]!
bl \fct
ldr X30, [SP], #8

msr SPSel, #0b1
eret
.endm

svcCall:
    sub     sp,sp, #0x8     // pseudo push x0
    sub     sp,sp, #0x8     // pseudo push x1
    sub     sp,sp, #0x8     // pseudo push x2
    sub     sp,sp, #0x8     // pseudo push x3
    sub     sp,sp, #0x8     // pseudo push x4
    saveX5toX29
    saveV0toV31

    mrs x5, SPSR_EL1
    str x5, [sp, #-8]!

    //set w5 to svc number
    MRS     x5,ELR_EL1
    LDR     x5,[x5,#-4]
    bic     w5,w5,#0xff000001
    ror     x5,x5, 5

    msr SPSel, #0b1

    str x30, [SP, #-8]!
    bl svc_handler
    ldr X30, [SP], #8

    msr SPSel, #0b0

    ldr x5, [sp], #8
    msr SPSR_EL1, x5

    restoreV31toV0
    restoreX29toX5
    add     sp,sp, #0x8     // pseudo pop x4
    add     sp,sp, #0x8     // pseudo pop x3
    add     sp,sp, #0x8     // pseudo pop x2
    add     sp,sp, #0x8     // pseudo pop x1
    add     sp,sp, #0x8     // pseudo pop x0
    ret

irqCall:
    saveX0toX4       
    saveX5toX29
    saveV0toV31

    mrs x5, SPSR_EL1
    str x5, [sp, #-8]!

    msr SPSel, #0b1

    str x30, [SP, #-8]!
    bl irg_handler
    ldr X30, [SP], #8

    msr SPSel, #0b0

    ldr x5, [sp], #8
    msr SPSR_EL1, x5

    restoreV31toV0
    restoreX29toX5
    restoreX5toX0
    ret

// curr_el_sp0_     : exception from the current EL using SP0.
// curr_el_spx_     :  exception from the current EL using the current SP.
// lower_el_aarch64_: exception from a lower EL

// ...sync          : The exception handler for a synchronous (includes svc)
// ...irq           : The exception handler for an IRQ exception
// ...fiq           : The exception handler for an FIQ exception
// ...serror        : The exception handler for a System Error 

.balign 0x800
Vector_table_el1:

curr_el_sp0_sync:
b .

.balign 0x80
curr_el_sp0_irq:         
b .

.balign 0x80
curr_el_sp0_fiq:         
b . 

.balign 0x80
curr_el_sp0_serror:      
b .   

.balign 0x80
curr_el_spx_sync:
callSavedLr svcCall

.balign 0x80
curr_el_spx_irq: 
callSavedLr irqCall

.balign 0x80
curr_el_spx_fiq:         
b .    

.balign 0x80
curr_el_spx_serror:       
b .    

.balign 0x80
lower_el_aarch64_sync:    
callSavedLr svcCall

.balign 0x80
lower_el_aarch64_irq:    
callSavedLr irqCall

.balign 0x80
lower_el_aarch64_fiq:   
b .     

.balign 0x80
lower_el_aarch64_serror:  
b .                      

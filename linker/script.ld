ENTRY(_start)
SECTIONS {
    . = 0x40000000;
    . = ALIGN(8);
    .startup . : { KEEP(*(.text.boot)) }

    /* Include the list of initialization functions sorted. */
    .init_array :
    {
        liblib.a:crti.c.obj(.init_array)
        KEEP (*(SORT(EXCLUDE_FILE(liblib.a:crti.c.obj liblib.a:crtn.c.obj) .init_array.*)))
        KEEP (*(EXCLUDE_FILE(liblib.a:crti.c.obj liblib.a:crtn..c.obj) .init_array))
        liblib.a:crtn.obj(.init_array)
    }

    /* Include the list of termination functions sorted. */
    .fini_array :
    {
        liblib.a:crti.c.obj(.fini_array)
        KEEP (*(SORT(EXCLUDE_FILE(liblib.a:crti.c.obj liblib.a:crtn.c.obj) .fini_array.*)))
        KEEP (*(EXCLUDE_FILE(liblib.a:crti.c.obj liblib.a:crtn.c.obj) .fini_array))
        liblib.a:crtn.c.obj(.fini_array)
    }

    _idle_start = .;
    .boot.idle : { KEEP(*(.boot.idle))}
    .idle :  { liblib.a:idle.cpp.obj }
    . = ALIGN(8);
    . += 0x1000; /* 4kB of stack memory */
    _idle_stack_top = .;

    _console_start = .;
    .boot.console : { KEEP(*(.boot.console))}
    .console :  { liblib.a:console.cpp.obj }
    . = ALIGN(8);
    . += 0x1000; /* 4kB of stack memory */
    _console_stack_top = .;

    /* bsp section
    _<name>_start = .;
    .boot.<name> : { KEEP(*(.boot.<name>))}
    .<name> :  { liblib.a:<name>.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _<name>_stack_top = .;
    */

    _counterEndless_start = .;
    .boot.counterEndless : { KEEP(*(.boot.counterEndless))}
    .counterEndless :  { liblib.a:counterEndless.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _counterEndless_stack_top = .;

    _counter_start = .;
    .boot.counter : { KEEP(*(.boot.counter))}
    .counter :  { liblib.a:counter.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _counter_stack_top = .;

    _semaphoreDemo_start = .;
    .boot.semaphoreDemo : { KEEP(*(.boot.semaphoreDemo))}
    .semaphoreDemo :  { liblib.a:semaphoreDemo.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _semaphoreDemo_stack_top = .;

    _producer_start = .;
    .boot.producer : { KEEP(*(.boot.producer))}
    .producer :  { liblib.a:producer.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _producer_stack_top = .;

    _consumer_start = .;
    .boot.consumer : { KEEP(*(.boot.consumer))}
    .consumer :  { liblib.a:consumer.cpp.obj }
    . = ALIGN(8);
    . += 0x1000;
    _consumer_stack_top = .;
    
    /* kernel section */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . += 0x1000; /* 4kB of stack memory */
    stack_top = .;
    . = ALIGN(8);
    _heap_start = .;
    . += 0x200000; /* 2.097.152 of heap memory */
    . = ALIGN(8);
    _heap_end = .;
    _heap_size = _heap_end - _heap_start;
}

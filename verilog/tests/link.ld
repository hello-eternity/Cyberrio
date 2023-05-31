
MEMORY {

}

SECTIONS {
    .memory : {
        . = 0x80000000;
        *(.text.init);
        *(.*text*);
        *(.*data*);
        *(.*bss*);
    } > mem
}
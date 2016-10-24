.section .init
    pop %ebp
    ret
#.size _init, . - _init

.section .fini
    pop %ebp
    ret
#.size _fini,  . - _fini

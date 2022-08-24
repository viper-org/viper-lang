.globl SystemCall
SystemCall:
    push %rcx
    push %rdx
    push %rsi
    push %rdi

    pop %rax
    pop %rdi
    pop %rsi
    pop %rdx

    syscall
    ret
    .section	".note.GNU-stack","",@progbits

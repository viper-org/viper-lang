[section .text]
[global SystemCall]
SystemCall:
    push rdi
    push rsi
    push rdx
    push rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    ret
[section .text]
[global _Z11_SystemCall4qqqqEq]
_Z11_SystemCall4qqqqEq:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    syscall
    ret
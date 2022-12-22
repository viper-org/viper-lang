[section .text]
[global _Z11_SystemCall4qqqqEq]
; Base system call function
; Returns syscall return value
_Z11_SystemCall4qqqqEq:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    syscall
    ret
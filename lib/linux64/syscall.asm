[section .text]
[global _Z11_SystemCallA4qqqqEq]
; Base system call function
; Returns syscall return value
_Z11_SystemCallA4qqqqEq:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    syscall
    ret


[global _Z11_SystemCallA7qqqqqqqEq]
; Base system call function
; Returns syscall return value
_Z11_SystemCallA7qqqqqqqEq:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov r10, r8
    mov r8, r9
    mov r9, [rsp + 0x10]
    syscall
    ret
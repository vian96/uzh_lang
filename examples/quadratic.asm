; This file was autmatically compiled by Uzh lang compiler
; register bx is used for pointing to begin of local vars
; register cx is used for temporary using of var address
; register dx is used for returning values from func

; register ecx is for adresses
; registers edx, ebx and eax is for temp info
extern Print
extern Scan
extern ExitProcess

section .bss
    global_vars     resd 1024
    local_vars      resd 1024
global _start
section .text
_start:


jmp end_of_func_main
main:
    call Scan
    push eax
; getting var a
    lea ecx, [local_vars + 4*0]
    pop edx
    mov [ecx], edx
    call Scan
    push eax
; getting var b
    lea ecx, [local_vars + 4*1]
    pop edx
    mov [ecx], edx
    call Scan
    push eax
; getting var c
    lea ecx, [local_vars + 4*2]
    pop edx
    mov [ecx], edx
; starting if 0
; getting var a
    lea ecx, [local_vars + 4*0]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setne al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_0
; true branch of if
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
push dword 4
; getting var a
    lea ecx, [local_vars + 4*0]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
; getting var c
    lea ecx, [local_vars + 4*2]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
    pop edx
    pop eax
    sub eax, edx
    push eax
; getting var d
    lea ecx, [local_vars + 4*3]
    pop edx
    mov [ecx], edx
; starting if 1
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    sete al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_1
; true branch of if
push dword 1
    call Print
    add esp, 4
push dword 0
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
push dword 2
; getting var a
    lea ecx, [local_vars + 4*0]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
    xor edx, edx
    pop ebx
    pop eax
    div ebx
    push eax
    pop edx
    pop eax
    sub eax, edx
    push eax
    call Print
    add esp, 4
    jmp end_of_if_1
    false_branch_if_1:
    end_of_if_1:
; starting if 2
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_2
; true branch of if
push dword 0
    call Print
    add esp, 4
    jmp end_of_if_2
    false_branch_if_2:
    end_of_if_2:
; starting if 3
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setg al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_3
; true branch of if
push dword 2
    call Print
    add esp, 4
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
push dword 0
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    sub eax, edx
    push eax
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    sub eax, edx
    push eax
push dword 2
; getting var a
    lea ecx, [local_vars + 4*0]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
    xor edx, edx
    pop ebx
    pop eax
    div ebx
    push eax
    call Print
    add esp, 4
push dword 0
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    sub eax, edx
    push eax
; getting var d
    lea ecx, [local_vars + 4*3]
    mov edx, [ecx]
    push edx
    pop eax
    pop edx
    add eax, edx
    push eax
push dword 2
; getting var a
    lea ecx, [local_vars + 4*0]
    mov edx, [ecx]
    push edx
    pop edx
    pop eax
    mul edx
    push eax
    xor edx, edx
    pop ebx
    pop eax
    div ebx
    push eax
    call Print
    add esp, 4
    jmp end_of_if_3
    false_branch_if_3:
    end_of_if_3:
    jmp end_of_if_0
    false_branch_if_0:
; starting if 4
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    sete al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_4
; true branch of if
; starting if 5
; getting var c
    lea ecx, [local_vars + 4*2]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    sete al
    push eax
   pop eax
    cmp eax, 0
    je false_branch_if_5
; true branch of if
push dword 100500
    call Print
    add esp, 4
    jmp end_of_if_5
    false_branch_if_5:
push dword 0
    call Print
    add esp, 4
    end_of_if_5:
    jmp end_of_if_4
    false_branch_if_4:
push dword 1
    call Print
    add esp, 4
push dword 0
; getting var c
    lea ecx, [local_vars + 4*2]
    mov edx, [ecx]
    push edx
; getting var b
    lea ecx, [local_vars + 4*1]
    mov edx, [ecx]
    push edx
    xor edx, edx
    pop ebx
    pop eax
    div ebx
    push eax
    pop edx
    pop eax
    sub eax, edx
    push eax
    call Print
    add esp, 4
    end_of_if_4:
    end_of_if_0:
push dword 66
    call Print
    add esp, 4

jmp end_of_all_program

end_of_func_main:



jmp main
end_of_all_program:
; ExitProcess( 0 )
push    dword 0
call    ExitProcess

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


jmp end_of_func_A
A:
push ebp
mov  ebp, esp
sub esp, 4*0
; starting if 3
; getting var m
    lea ecx, [ebp - 4*-2]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
; getting var n
    lea ecx, [ebp - 4*-3]
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
    pop edx
    or eax, edx
 push eax
   pop eax
    cmp eax, 0
    je false_branch_if_3
; true branch of if
push dword 0
pop eax
mov esp, ebp
pop ebp
ret
    jmp end_of_if_3
    false_branch_if_3:
    end_of_if_3:
; starting if 4
; getting var m
    lea ecx, [ebp - 4*-2]
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
; getting var n
    lea ecx, [ebp - 4*-3]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
pop eax
mov esp, ebp
pop ebp
ret
    jmp end_of_if_4
    false_branch_if_4:
    end_of_if_4:
; starting if 5
; getting var n
    lea ecx, [ebp - 4*-3]
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

; Calcing params
push dword 1

; Calcing params
; getting var m
    lea ecx, [ebp - 4*-2]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
call A

add esp, 4*2
push eax
pop eax
mov esp, ebp
pop ebp
ret
    jmp end_of_if_5
    false_branch_if_5:
    end_of_if_5:

; Calcing params

; Calcing params
; getting var n
    lea ecx, [ebp - 4*-3]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax

; Calcing params
; getting var m
    lea ecx, [ebp - 4*-2]
    mov edx, [ecx]
    push edx
call A

add esp, 4*2
push eax

; Calcing params
; getting var m
    lea ecx, [ebp - 4*-2]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
call A

add esp, 4*2
push eax
pop eax
mov esp, ebp
pop ebp
ret

end_of_func_A:


jmp end_of_func_main
main:
push ebp
mov  ebp, esp
sub esp, 4*3
    call Scan
    push eax
; getting var m
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
    call Scan
    push eax
; getting var n
    lea ecx, [ebp - 4*2]
    pop edx
    mov [ecx], edx

; Calcing params
; getting var n
    lea ecx, [ebp - 4*2]
    mov edx, [ecx]
    push edx

; Calcing params
; getting var m
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
call A

add esp, 4*2
push eax
    call Print
    add esp, 4

jmp end_of_all_program

end_of_func_main:



jmp main
end_of_all_program:
; ExitProcess( 0 )
push    dword 0
call    ExitProcess

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
    global_vars     resd 8192
global _start
section .text
_start:

push dword 0
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 9
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx

jmp end_of_func_fib
fib:
push ebp
mov  ebp, esp
sub esp, 4*2
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 0
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 1
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
push dword 2
; getting var i
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
; starting while 1
    begin_of_while_1:
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 10
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je end_of_while_1
push dword 0
push dword 15
    pop edx
    pop eax
    sub eax, edx
    push eax
    call Print
    add esp, 4
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 2
    pop edx
    pop eax
    sub eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 2
    pop edx
    pop eax
    sub eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var i
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_1
    end_of_while_1:
push dword 0
pop eax
mov esp, ebp
pop ebp
ret

end_of_func_fib:


jmp end_of_func_printMas
printMas:
push ebp
mov  ebp, esp
sub esp, 4*2
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 0
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 1
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
push dword 0
; getting var i
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
; starting while 3
    begin_of_while_3:
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 10
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je end_of_while_3
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    call Print
    add esp, 4
; getting var i
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var i
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_3
    end_of_while_3:
push dword 0
pop eax
mov esp, ebp
pop ebp
ret

end_of_func_printMas:


jmp end_of_func_main
main:
push ebp
mov  ebp, esp
sub esp, 4*0
    call Scan
    push eax
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 0
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
    call Scan
    push eax
; getting var mas
    lea ecx, [global_vars + 4*1]
    push ecx
    ; index
push dword 1
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
call fib

add esp, 4*0
push eax
call printMas

add esp, 4*0
push eax

jmp end_of_all_program

end_of_func_main:



jmp main
end_of_all_program:
; ExitProcess( 0 )
push    dword 0
call    ExitProcess

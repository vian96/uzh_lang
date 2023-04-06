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
; getting var x
    lea ecx, [global_vars + 4*1]
    pop edx
    mov [ecx], edx
push dword 0
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
push dword 2048
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx

jmp end_of_func_main
main:
push ebp
mov  ebp, esp
sub esp, 4*7
    call Scan
    push eax
; getting var n
    lea ecx, [ebp - 4*1]
    pop edx
    mov [ecx], edx
; starting if 1
; getting var n
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 2000
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setg al
    push eax
; getting var n
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
push dword 2
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
    je false_branch_if_1
; true branch of if
push dword 0
push dword 1337
    pop edx
    pop eax
    sub eax, edx
    push eax
    call Print
    add esp, 4
; getting var return
    lea ecx, [ebp - 4*2]
    mov edx, [ecx]
    push edx
    jmp end_of_if_1
    false_branch_if_1:
    end_of_if_1:
push dword 0
; getting var i
    lea ecx, [ebp - 4*3]
    pop edx
    mov [ecx], edx
; starting while 4
    begin_of_while_4:
; getting var i
    lea ecx, [ebp - 4*3]
    mov edx, [ecx]
    push edx
; getting var n
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je end_of_while_4
    call Scan
    push eax
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*3]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
; getting var i
    lea ecx, [ebp - 4*3]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var i
    lea ecx, [ebp - 4*3]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_4
    end_of_while_4:
push dword 1
; getting var begin
    lea ecx, [ebp - 4*4]
    pop edx
    mov [ecx], edx
; starting while 5
    begin_of_while_5:
; getting var begin
    lea ecx, [ebp - 4*4]
    mov edx, [ecx]
    push edx
; getting var n
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je end_of_while_5
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var begin
    lea ecx, [ebp - 4*4]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
; getting var elem
    lea ecx, [ebp - 4*5]
    pop edx
    mov [ecx], edx
; getting var begin
    lea ecx, [ebp - 4*4]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
; getting var topl
    lea ecx, [ebp - 4*6]
    pop edx
    mov [ecx], edx
; starting while 6
    begin_of_while_6:
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
push dword 0
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setge al
    push eax
; getting var elem
    lea ecx, [ebp - 4*5]
    mov edx, [ecx]
    push edx
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
    pop eax
    pop edx
    and eax, edx
 push eax
   pop eax
    cmp eax, 0
    je end_of_while_6
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    mov edx, [ecx]
    push edx
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
push dword 1
    pop edx
    pop eax
    sub eax, edx
    push eax
; getting var topl
    lea ecx, [ebp - 4*6]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_6
    end_of_while_6:
; getting var elem
    lea ecx, [ebp - 4*5]
    mov edx, [ecx]
    push edx
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var topl
    lea ecx, [ebp - 4*6]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
    pop edx
    pop ecx
    lea ecx, [ecx + 4*edx]
    pop edx
    mov [ecx], edx
; getting var begin
    lea ecx, [ebp - 4*4]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var begin
    lea ecx, [ebp - 4*4]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_5
    end_of_while_5:
push dword 0
; getting var i
    lea ecx, [ebp - 4*3]
    pop edx
    mov [ecx], edx
; starting while 7
    begin_of_while_7:
; getting var i
    lea ecx, [ebp - 4*3]
    mov edx, [ecx]
    push edx
; getting var n
    lea ecx, [ebp - 4*1]
    mov edx, [ecx]
    push edx
    xor eax, eax
    pop ebx
    pop ecx
    cmp ecx, ebx
    setl al
    push eax
   pop eax
    cmp eax, 0
    je end_of_while_7
; getting var MEM
    lea ecx, [global_vars + 4*2]
    push ecx
    ; index
; getting var i
    lea ecx, [ebp - 4*3]
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
    lea ecx, [ebp - 4*3]
    mov edx, [ecx]
    push edx
push dword 1
    pop eax
    pop edx
    add eax, edx
    push eax
; getting var i
    lea ecx, [ebp - 4*3]
    pop edx
    mov [ecx], edx
    jmp begin_of_while_7
    end_of_while_7:

jmp end_of_all_program

end_of_func_main:



jmp main
end_of_all_program:
; ExitProcess( 0 )
push    dword 0
call    ExitProcess

const char begin_asm_file[] =
    "; This file was autmatically compiled by "
    "Uzh lang compiler\n"
    // TODO
    "; register bx is used for pointing to begin of local vars\n"
    "; register cx is used for temporary using of var address\n"
    "; register dx is used for returning values from func\n\n" 

    "; register ecx is for adresses\n"
    "; registers edx, ebx and eax is for temp info\n"
    "extern Print\n"
    "extern Scan\n"
    "extern ExitProcess\n\n"

    "section .bss\n"
    "    global_vars     resd 8192\n"

    "global _start\n"
    "section .text\n_start:\n\n";

const char end_asm_file[] = 
    "\n\njmp main\nend_of_all_program:\n"
    "; ExitProcess( 0 )\n"
    "push    dword 0\n"
    "call    ExitProcess\n";


gcc -m32 -c x86backend/asm_utils.c -o utils.obj

gcc -shared -m32 -o utils.dll utils.obj

nasm -f win32 %*.asm -o %*.obj

GoLink.exe  /console /nw /entry _start %*.obj kernel32.dll utils.dll


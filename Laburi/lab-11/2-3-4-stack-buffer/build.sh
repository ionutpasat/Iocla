#!/bin/bash

/c/Program\ Files\ \(x86\)/SASM/NASM/nasm.exe -f win32 stack_buffer.asm
/c/Program\ Files\ \(x86\)/SASM/MinGW/bin/gcc -Wall -g -o stack_buffer stack_buffer.obj

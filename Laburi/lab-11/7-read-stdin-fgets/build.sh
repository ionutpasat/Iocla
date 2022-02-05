#!/bin/bash

/c/Program\ Files\ \(x86\)/SASM/NASM/nasm.exe -f win32 read_stdin_fgets.asm
/c/Program\ Files\ \(x86\)/SASM/MinGW/bin/gcc -Wall -g -o read_stdin_fgets read_stdin_fgets.obj

#!/bin/bash
# Assemble a single assembly file

file="$1"
prog=`basename $0 | cut -d . -f 1`

no_file()
{
    echo -e "\e[33mFile $1 not found!\n\e[0m"
    exit 1
}

test -e "$file.asm" || no_file "$file.asm"

case "$2" in
    32 )
        yasm -g dwarf2 -f elf32 "$file.asm" -l "$file.lst32"
        ld -m elf_i386 -g -s -o "$file" "$file.o"
    ;;
    64 )
        yasm -g dwarf2 -f elf64 "$file.asm" -l "$file.lst64"
        ld -m elf_x86_64 -g -s -o "$file" "$file.o"
    ;;
    * )
        echo "Usage: " $prog "[ASM FILE] [MODE]"
        echo "e.g -> assm file 32"
    ;;
esac
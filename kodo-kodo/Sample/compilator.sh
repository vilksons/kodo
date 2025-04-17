#!/bin/bash

: ' get the directory of the current script '
shell_dir="$(dirname "$0")"

: ' define allowed include subdirectory '
shell_allow_sdir="/pawno/include"

: ' compiler options for pawncc '
amx_option="-;+ -(+ -d3"

: ' infinite loop to continuously compile files '
while true; do
    : ' get the directory of the current script again '
    bashdir=$(dirname "$0")

    : ' variable to store the path of pawncc compiler '
    bashpawncc=""
    
    : ' search for pawncc binary within the current script directory '
    while ifs= read -r -d '' pawncc; do
        if [ -f "$pawncc" ]; then
            bashpawncc="$pawncc"  : ' found pawncc, store the path '
            break
        fi
    done < <(find "$shell_dir" -name "pawncc" -print0)

    : ' if pawncc is not found, display error and wait for keypress '
    if [ -z "$bashpawncc" ]; then
        echo -e "\n# [$(date +%t)] pawncc not found in any subdirectories.\n"
        read -r -n 1 -s -p "press any key to continue..."  : ' wait for user input '
        continue  : ' restart the loop '
    fi

    : ' find and compile all .pwn files '
    find "$bashdir" -type f -name "*.pwn*" | while read -r __file; do
        : ' set output .amx file path based on input file name '
        amx_o="$(dirname "$__file")/$(basename "${__file%.pwn*}.amx")"
        echo "compiling $__file -> $amx_o"
        : ' run pawncc compiler with include path and options '
        "$bashpawncc" -i"$shell_dir$shell_allow_sdir" "$__file" -o"$amx_o" "$amx_option" || echo "compilation failed for $__file" >&2
    done

    : ' wait for user input before restarting the loop '
    read -r -n 1 -s -p "press any key to continue or ctrl+c to exit..."
    echo
done

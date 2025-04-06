#!/usr/bin/bash

shell_DIR="$(dirname "$0")"
shell_ALLOW_SDIR="/pawno/include"

AMX_OPTION="-;+ -(+ -d3"

while true; do
    BASHDIR=$(dirname "$0")

    BASHPAWNCC=""
    
    while IFS= read -r -d '' pawncc; do
    if [ -f "$pawncc" ]; then
        BASHPAWNCC="$pawncc"
        break
    fi
    done < <(find "$shell_DIR" -name "pawncc" -print0)

    if [ -z "$BASHPAWNCC" ]; then
        echo -e "\n# [$(date +%T)] pawncc not found in any subdirectories.\n"
        read -r -n 1 -s -p "Press any key to continue..."
        continue
    fi

    find "$BASHDIR" -type f -name "*.pwn*" | while read -r __file; do
        AMX_O="$(dirname "$__file")/$(basename "${__file%.pwn*}.amx")"
        echo "Compiling $__file -> $AMX_O"
        "$BASHPAWNCC" -i"$shell_DIR$shell_ALLOW_SDIR" "$__file" -o"$AMX_O" "$AMX_OPTION" || echo "Compilation failed for $__file" >&2
    done

    read -r -n 1 -s -p "Press any key to continue or Ctrl+C to exit..."
    echo
done

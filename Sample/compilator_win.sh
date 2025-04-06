#!/usr/bin/bash

shell_DIR="$(dirname "$0")"
shell_ALLOW_SDIR="/pawno/include"

AMX_OPTION="-;+ -(+ -d3"

COMPILERS() {
  BASHDIR=$(dirname "$0")
  BASHPAWNCC=""
  
  while IFS= read -r -d '' __file; do
    if [[ -f "$__file" ]]; then
      BASHPAWNCC="$__file"
      break
    fi
  done < <(find "$BASHDIR" -type f -name "pawncc.exe" -print0)

  if [[ -z "$BASHPAWNCC" ]]; then
    echo
    echo "# [$(date +%T)] pawncc not found in any subdirectories."
    echo
    read -r -p "Press any key to continue..."
    exit 1
  fi

  while IFS= read -r -d '' __file; do
    if [[ -f "$__file" ]]; then
      AMX_O="${__file%.pwn*}.amx"
      "$BASHPAWNCC" -i"$shell_DIR$shell_ALLOW_SDIR" "$__file" -o"$AMX_O" "$AMX_OPTION"
    fi
  done < <(find "$BASHDIR" -type f -name "*.pwn*" -print0)

  read -r -p "Press any key to continue..."
  COMPILERS
}

COMPILERS

What is Kodo?
Kodo is a toolchain for SA-MP and Open.MP development built on C Code by providing a pure Compiler from Compuphase with updates provided.
In Kodo Toolchain, users are given the freedom to update and even recompile after modifying.
Kodo utilizes a single Linux Program file built on C with the help of Make, Makefile..,
Linux Programs that allow users to run them in a Linux environment, such as WSL/WSL2/Docker or Termux and the like.

How to Compile from Source?
You need GCC https://gcc.gnu.org/ as default compilation, and Make https://www.gnu.org/software/make/. For some external libraries, GCC requires libm, libcurl, libarchive, readline, ncurses.

How to usage?
Everything is organized by the toml file, `kodo.toml`

| SECTION |      KEY      |                             VALUE                                 |
| general |      os       | filled with the type of operating system. windows|linux           |
| compiler|     option    | filled with the option Compiler of Pawn Compiler - PCC            |
| compiler|  include_path | filled with the path where ".inc" is located. example: `/includes`|
| compiler|     input     | filled with the pawn files name to be compiled.                   |
| compiler|     output    | filled with the output file name from the input.                  |

Compiler
Usage `compile .` to compile pawn files from kodo.toml.
Usage `compile example.pwn` to compile pawn files from args.
Runner
Usage `running .` to running server modes by server.cfg from `gamemode0`
Usage `running example` to running server modes by files .amx from /gamemodes/
Debugger
Usage `debug .` to debug server modes by server.cfg from `gamemode0`
Usage `debug example` to debug server modes by files .amx from /gamemodes/

Compiler Options
# Pawn Compiler 3.10.10
Copyright (c) 1997-2006, ITB CompuPhase

| Option          | Description                                                        |
|-----------------|--------------------------------------------------------------------|
| `-A<num>`       | Alignment in bytes of the data segment and the stack               |
| `-a`            | Output assembler code                                              |
| `-C[+/-]`       | Compact encoding for output file (default=+)                       |
| `-c<name>`      | Codepage name or number; e.g. 1252 for Windows Latin-1             |
| `-Dpath`        | Active directory path                                              |
| `-d<num>`       | Debugging level (default=-d1):                                     |
|                 | `0` no symbolic information, no run-time checks                    |
|                 | `1` run-time checks, no symbolic information                       |
|                 | `2` full debug information and dynamic checking                    |
|                 | `3` same as -d2, but implies `-O0`                                 |
| `-e<name>`      | Set name of error file (quiet compile)                             |
| `-H<hwnd>`      | Window handle to send a notification message on finish             |
| `-i<name>`      | Path for include files                                             |
| `-l`            | Create list file (preprocess only)                                 |
| `-o<name>`      | Set base name of (P-code) output file                              |
| `-O<num>`       | Optimization level (default=-O1):                                  |
|                 | `0` no optimization                                                |
|                 | `1` JIT-compatible optimizations only                              |
|                 | `2` full optimizations                                             |
| `-p<name>`      | Set name of "prefix" file                                          |
| `-R[+/-]`       | Add detailed recursion report with call chains (default=-)         |
| `-r[name]`      | Write cross reference report to console or to specified file       |
| `-S<num>`       | Stack/heap size in cells (default=4096)                            |
| `-s<num>`       | Skip lines from the input file                                     |
| `-t<num>`       | TAB indent size (in character positions, default=8)                |
| `-v<num>`       | Verbosity level; `0=quiet`, `1=normal`, `2=verbose` (default=1)    |
| `-w<num>`       | Disable a specific warning by its number                           |
| `-X<num>`       | Abstract machine size limit in bytes                               |
| `-XD<num>`      | Abstract machine data/stack size limit in bytes                    |
| `-Z[+/-]`       | Run in compatibility mode (default=-)                              |
| `-E[+/-]`       | Turn warnings into errors                                          |
| `-\`            | Use '\' for escape characters                                      |
| `-^`            | Use '^' for escape characters                                      |
| `-;[+/-]`       | Require a semicolon to end each statement (default=-)              |
| `-([+/-]`       | Require parentheses for function invocation (default=-)            |
| `sym=val`       | Define constant `sym` with value `val`                             |
| `sym=`          | Define constant `sym` with value 0                                 |

Options may start with a dash or a slash; the options `-d0` and `/d0` are equivalent.
Options with a value may optionally separate the value from the option letter with a colon (`:`) or an equal sign (`=`). That is, the options `-d0`, `-d=0`, and `-d:0` are all equivalent.
What is Kodo?
Kodo is a toolchain for SA-MP and Open.MP development built on C Code by providing a pure Compiler from Compuphase with updates provided.
In Kodo Toolchain, users are given the freedom to update and even recompile after modifying.
Kodo utilizes a single Linux Program file built on C with the help of Make, Makefile..,
Linux Programs that allow users to run them in a Linux environment, such as WSL/WSL2/Docker or Termux and the like.

How to compiler from Source Code?
You need GCC https://gcc.gnu.org/ as default compilation, and Make https://www.gnu.org/software/make/. For some external libraries, GCC requires libm, libcurl, libarchive, readline, ncurses.

How to usage?
Everything is organized by the toml file, `kodo.toml`

Compiler
Usage `compile .` to compile pawn files from kodo.toml.
Usage `compile example.pwn` to compile pawn files from args.
Runner
Usage `running .` to running server modes by server.cfg from `gamemode0`
Usage `running example` to running server modes by files .amx from /gamemodes/
Debugger
Usage `debug .` to debug server modes by server.cfg from `gamemode0`
Usage `debug example` to debug server modes by files .amx from /gamemodes/
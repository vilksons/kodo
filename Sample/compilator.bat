@echo off
setlocal enabledelayedexpansion

:: ' set compiler options '
set "amx_option=-;+ -(+ -d3"

:compilers
  :: ' get the current script directory '
  set "batchdir=%~dp0"
  
  :: ' search recursively for pawncc.exe '
  for /r "%batchdir%" %%p in (pawncc.exe) do (
      if exist "%%p" (
          set "batchpawncc=%%p"
          goto pawncc
      )
  )
  
:pawncc
  :: ' if compiler is not found, print error and exit '
  if not defined batchpawncc (
      echo.
      echo # [%time%] pawncc not found in any subdirectories.
      echo.
      pause
      exit /b
  )

  :: ' compile all .pwn files recursively '
  for /r "%batchdir%" %%f in (*.pwn*) do (
      if exist "%%f" (
          set "amx=%%~dpnf"
          set "amx=!amx:.pwn=!%.amx"
          "%batchpawncc%" "%%f" -o"!amx!" "!amx_option!"
      )
  )

pause
goto :end

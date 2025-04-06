@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

SET "AMX_OPTION=-;+ -(+ -d3"

:COMPILERS
  SET "BATCHDIR=%~dp0"
  
  FOR /r "%BATCHDIR%" %%P IN (pawncc.exe) DO (
      IF EXIST "%%P" (
          SET "BATCHPAWNCC=%%P"
          GOTO PAWNCC
      )
  )
  
  :PAWNCC
  IF NOT DEFINED BATCHPAWNCC (
      ECHO.
      ECHO # [%time%] pawncc not found in any subdirectories.
      ECHO.
      PAUSE
      EXIT /B
  )

  FOR /r "%BATCHDIR%" %%F IN (*.pwn*) DO (
      IF EXIST "%%F" (
          SET "AMX=%%~dpnF"
          SET "AMX=!AMX:.pwn=!%.amx"
          "%BATCHPAWNCC%" "%%F" -o"!AMX!" "!AMX_OPTION!"
      )
  )

PAUSE
GOTO :END

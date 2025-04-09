@ECHO OFF

setlocal EnableDelayedExpansion

color F

SET "ASM_OPTION_M=-o"
SET "ASM_OPTION_P=-C- -O0 -d2"

SET "BATCHDIR=%~dp0"
SET "BATCHTITLE="
SET "BATCHSTS=false"
SET "BATCHPAWNCC="
SET "BATCHNAME=kodo.cmd"
SET "SVRDEF=samp-server.exe"

:COMMAND_TYPEOF
FOR /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & ECHO on & FOR %%b in (1) do rem"') do (SET "DEL=%%a")

<nul SET /p="" 
    CALL :COLOURTEXT A "user" 
    <nul SET /p=":~$ " 
    SET /p KODOTYPEOF=" "
    GOTO NEXT

:COLOURTEXT
    <nul SET /p "=%DEL%" > "%~2"
    FINDSTR /v /a:%1 /R "+" "%~2" nul
    del "%~2" > nul
    GOTO :eof

:NEXT
SET "BATCHFILE=true"
SET "BATCHOPTION=kodo"

IF "%KODOTYPEOF%"=="%BATCHOPTION% -c" (

    TASKKILL /f /im "samp-server.exe" >nul 2>&1

    SET "BATCHTITLE=compilers"
    TITLE user:~/!BATCHTITLE!
    
    ECHO.

    SET "BATCHSTS=true"
    GOTO COMPILERS

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -r" (

    TASKKILL /f /im "samp-server.exe" >nul 2>&1
    
    SET "BATCHTITLE=running"
    TITLE user:~/!BATCHTITLE!

    GOTO SERVERS

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -t" (
    
:TESTSERVERS
    IF EXIST "%BATCHDIR%server_log.txt" ( DEL "%BATCHDIR%server_log.txt" /q >nul )
    
    TASKKILL /f /im "samp-server.exe" >nul 2>&1
    
    TIMEOUT /t 1 >nul
        START /min "" "!SVRDEF!"
    TIMEOUT /t 1 >nul
        TYPE server_log.txt
		ECHO.
	TASKKILL /f /im "samp-server.exe" >nul 2>&1
	
    GOTO BATCHEND

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -ci" (

    TASKKILL /f /im "samp-server.exe" >nul 2>&1

    SET "BATCHTITLE=compile running"
    TITLE user:~/!BATCHTITLE!

    SET "BATCHSTS=false"
    
    CALL :COMPILERS
    FINDSTR /i "error" %METADAT_FILE% >nul && ECHO. || CALL :OK_NEXT

:OK_NEXT
    ECHO # Press any key to running . . .
    PAUSE >nul

:SERVERS
    IF EXIST "%BATCHDIR%server_log.txt" ( DEL "%BATCHDIR%server_log.txt" /q >nul )
	
    START "" "!SVRDEF!"
	
    TIMEOUT /t 2 >nul
    TASKLIST | FIND /i "!SVRDEF!" >nul

    IF not EXIST !SVRDEF! (
        ECHO # !SVRDEF! not found..
        TIMEOUT /t 1 >nul
            START "" "https://sa-mp.app/"
        GOTO COMMAND_TYPEOF
    )

    IF ERRORLEVEL 1 (
        SET "BATCHTITLE=running - failed"
        TITLE user:~/!BATCHTITLE!

        ECHO.
        ECHO # [%time%] S?.. no
		
        IF EXIST "server_log.txt" (
            TIMEOUT /t 2
                TYPE server_log.txt
            ECHO.
        ) ELSE (
            ECHO # server_log.txt not found.
        )
        
        <nul SET /p=""
            CALL :COLOURTEXT a "# End."
            ECHO.
        GOTO COMMAND_TYPEOF
    ) ELSE (
        ECHO # [%time%] S?.. Done
        ECHO.
		
        TIMEOUT /t 2 >nul
        FINDSTR /i "error" server_log.txt >nul && CALL :START_TRUE2 || CALL :START_FALSE2

:START_TRUE2
        <nul SET /p=""
            CALL :COLOURTEXT 4X "~"
            ECHO    "Error"   .. Yes .. True
        CALL :ERROR_CACHE
        
        GOTO :eof

:START_FALSE2
        <nul SET /p=""
            CALL :COLOURTEXT a "~"
            ECHO    "Error"   .. No .. False
        
:CHECK2
        FINDSTR /i "failed" server_log.txt >nul && CALL :START_TRUE || CALL :START_FALSE

:START_TRUE
        <nul SET /p=""
            CALL :COLOURTEXT 4X "~"
            ECHO    "Failed"  .. Yes .. True
        CALL :FAILED_CACHE
        
        GOTO :eof

:START_FALSE
        <nul SET /p=""
            CALL :COLOURTEXT a "~"
            ECHO    "Failed"  .. No .. False

:CHECK3
        FINDSTR /i "invalid" server_log.txt >nul && CALL :START_TRUE3 || CALL :START_FALSE3

:START_TRUE3
        <nul SET /p=""
            CALL :COLOURTEXT 4X "~"
            ECHO    "Invalid" .. Yes .. True
        CALL :INVALID_CACHE

        GOTO :eof

:START_FALSE3
        <nul SET /p=""
            CALL :COLOURTEXT a "~"
            ECHO    "Invalid" .. No .. false

        ECHO.
        GOTO BATCHEND
    )

:ERROR_CACHE
    ECHO.
    FINDSTR /i "error" ^> server_log.txt
    ECHO.
    GOTO CHECK2
:FAILED_CACHE
    ECHO.
    FINDSTR /i "failed" ^> server_log.txt
    ECHO.
    GOTO CHECK3
:INVALID_CACHE
    ECHO.
    FINDSTR /i "invalid" ^> server_log.txt
    ECHO.

    GOTO COMMAND_TYPEOF

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -C" (

    SET "BATCHTITLE=clear screen"
    TITLE user:~/!BATCHTITLE!

    CLS
    GOTO COMMAND_TYPEOF

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -V" (

    SET "BATCHTITLE=vscode tasks"
    TITLE user:~/!BATCHTITLE!

    IF EXIST ".vscode" (
        RMDIR /s /q .vscode
    )
    MKDIR ".vscode"
    (
        ECHO {
        ECHO   "version": "2.0.0",
        ECHO   "tasks": [
        ECHO     {
        ECHO       "label": "Run Kodo",
        ECHO       "type": "process",
        ECHO       "command": "${workspaceFolder}/%BATCHNAME%",
        ECHO       "group": {
        ECHO           "kind": "build",
        ECHO           "isDefault": true
        ECHO       },
        ECHO       "problemMatcher": [],
        ECHO       "detail": "Task to run the batch file"
        ECHO     }
        ECHO   ]
        ECHO }
    ) > ".vscode\tasks.json"
    ECHO # [%time%] C? '.vscode\tasks.json'...: [yes]
    GOTO BATCHEND

) ELSE IF "%KODOTYPEOF:~0,2%"=="./" (

    SET RENEWTYPEOF=%KODOTYPEOF:~2%

    FOR /r "%BATCHDIR%" %%a in ("!RENEWTYPEOF!.*") do (
            ECHO %%~nxa | FINDSTR /i ".io" >nul
        IF not ERRORLEVEL 1 (
            ECHO E: File "%%~nxa" The file has become a samplex file....
            GOTO BATCHEND
        ) ELSE (
            ECHO %%~nxa | FINDSTR /i ".amx" >nul
            IF ERRORLEVEL 1 (
                ren "%%a" "!RENEWTYPEOF!.io.pwn"
            ) ELSE (
                GOTO BATCHEND
            )
        )
    )
    
    GOTO BATCHEND

)  ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -R" (

    SET /p NAMEFROMINPUT="~ "

    FOR /r "%BATCHDIR%" %%a in ("!NAMEFROMINPUT!.*") do (
        ECHO %%~nxa | FINDSTR /i ".io" >nul
        IF not ERRORLEVEL 1 (
            ECHO E: File "%%~nxa" The file has become a samplex file....
            GOTO BATCHEND
        ) ELSE (
            ECHO %%~nxa | FINDSTR /i ".amx" >nul
            IF ERRORLEVEL 1 (
                ren "%%a" "!NAMEFROMINPUT!.io.pwn"
            ) ELSE (
                GOTO BATCHEND
            )
        )
    )

    GOTO BATCHEND

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -F" (

    SET "BATCHTITLE=folder existence check"
    TITLE user:~/!BATCHTITLE!

    IF EXIST filterscripts (
        ECHO.
        ECHO # filterscripts is .. Ok ..
        ECHO  [A subdirectory or file filterscripts already exists.]
        ECHO -
        TIMEOUT /t 1 >nul
    ) ELSE (
        MKDIR filterscripts
        ECHO # [%time%] C? '%BATCHDIR%filterscripts'...: [yes]
        TIMEOUT /t 1 >nul
    )
    IF EXIST gamemodes (
        ECHO.
        ECHO # gamemodes is .. Ok ..
        ECHO  [A subdirectory or file gamemodes already exists.]
        ECHO -
        TIMEOUT /t 1 >nul
    ) ELSE (
        setlocal EnableDelayedExpansion
        MKDIR gamemodes
        (
            ECHO #include ^<a_samp^>
            ECHO.
            ECHO main^(^) {
            ECHO     printf "Hello, World!";
            ECHO }
        ) > "gamemodes\main.io.pwn"
        endlocal
        ECHO.
        ECHO # [%time%] C? '%BATCHDIR%gamemodes'...: [yes]
        ECHO.
        TIMEOUT /t 1 >nul
    )
    IF EXIST scriptfiles (
        ECHO.
        ECHO # scriptfiles is .. Ok ..
        ECHO  [A subdirectory or file scriptfiles already exists.]
        ECHO -
        TIMEOUT /t 1 >nul
    ) ELSE (
        MKDIR scriptfiles
        ECHO # [%time%] C? '%BATCHDIR%scriptfiles'...: [yes]
        TIMEOUT /t 1 >nul
    )
    FOR /r "%BATCHDIR%" %%F in (*.io*) DO (
        IF EXIST "%%F" (
            IF not "%%~xF"==".amx" (
                ECHO.
                ECHO # Kodo file is .. Ok ..
                ECHO  [A subdirectory or file %%F already exists.]
                ECHO -
                TIMEOUT /t 1 >nul
            )
        )
    )
    IF EXIST server.cfg (
        ECHO.
        ECHO # server.cfg is .. Ok ..
        ECHO  [A subdirectory or file server.cfg already exists.]
    ) ELSE (
        (
            ECHO ECHO Executing Server Config...
            ECHO lanmode 0
            ECHO rcon_password changename
            ECHO maxplayers 150
            ECHO port 7777
            ECHO hostname SA-MP 0.3
            ECHO gamemode0 main 1
            ECHO filterscripts 
            ECHO announce 0
            ECHO chatlogging 0
            ECHO weburl www.sa-mp.com
            ECHO onfoot_rate 40
            ECHO incar_rate 40
            ECHO weapon_rate 40
            ECHO stream_distance 300.0
            ECHO stream_rate 1000
            ECHO maxnpc 0
            ECHO logtimeformat [%H:%M:%S]
            ECHO language English
        ) > "server.cfg"
        ECHO # [%time%] C? '%BATCHDIR%server.cfg'...: [yes]
        ECHO.
        TYPE server.cfg
        ECHO.
        GOTO BATCHEND
    )

) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -G" (
    git init
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -X" (
    git clone https://github.com/laterium/sampKodo.git
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -P" (
    START "" "https://github.com/pawn-lang/compiler/releases"
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -D" (
    cmd /c dir
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -K" (
    START %BATCHNAME%
    EXIT
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -N" (
    cmd /c netstat -an
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -S" (
    cmd /c systeminfo
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% -I" (
    powershell -Command "ipconfig"
) ELSE IF "%KODOTYPEOF%"=="help" (

    SET "BATCHTITLE=help"
    TITLE user:~/!BATCHTITLE!
    
:HELPL
    ECHO usage: kodo [-c compile] [-r running] [-t test server] [-ci compile-running] 
    ECHO       	   [-F folder check] [-C clear screen] [-P pawncc release] [-V vscode tasks]
    ECHO       	   [-X clone sampKodo] [-R rename file] [-K kill batch] [-D directory]
    ECHO       	   [-N netstat] [-I ipconfig] [-S systeminfo] [-G git init]
    GOTO COMMAND_TYPEOF

) ELSE IF "%KODOTYPEOF%"=="kodo ." (

    GOTO TESTSERVERS

) ELSE IF "%KODOTYPEOF%"=="kodo" (

    SET "BATCHTITLE=kodo"
    TITLE user:~/!BATCHTITLE!
    GOTO HELPL

) ELSE IF "%KODOTYPEOF%"=="" (
    GOTO COMMAND_TYPEOF
) ELSE IF "%KODOTYPEOF%"==" " (
    GOTO COMMAND_TYPEOF
) ELSE IF "%KODOTYPEOF%"=="%BATCHOPTION% " (
    GOTO HELPL
) ELSE (
    ECHO '!KODOTYPEOF!' is not recognized as an internal or external command,
    ECHO operable program or batch file.
    ECHO.
    GOTO HELPL
    GOTO COMMAND_TYPEOF
)

:BATCHEND
<nul SET /p=""
    CALL :COLOURTEXT a "# Press any key to return"
    ECHO.
PAUSE >nul
GOTO COMMAND_TYPEOF

:COMPILERS
    <nul SET /p=""
        CALL :COLOURTEXT a "~"
        cmd /c time /t
        ECHO.

    FOR /r "%BATCHDIR%" %%P in (pawncc.exe) DO (
        IF EXIST "%%P" (
            SET "BATCHPAWNCC=%%P"
            GOTO PAWNCC
        )
    )
:PAWNCC
    IF not DEFINED BATCHPAWNCC (
        ECHO.
            ECHO # [%time%] pawncc not found in any subdirectories.
        ECHO.

        TIMEOUT /t 1 >nul
        START "" "https://github.com/pawn-lang/compiler/releases"
        GOTO COMMAND_TYPEOF
    )
    SET "BATCHFILE=false"
    FOR /r "%BATCHDIR%" %%F in (*.io*) DO (
        IF EXIST "%%F" (
            IF not "%%~xF"==".amx" (
            
            SET "BATCHFILE=true"
            
            TITLE %%F
            
            SET "AMX_O=%%~dpnF"
            SET "AMX_O=!AMX_O:.io=!%.amx"

            "!BATCHPAWNCC!" "%%F" %ASM_OPTION_M%"!AMX_O!" %ASM_OPTION_P% > %METADAT_FILE% 2>&1
            TYPE %METADAT_FILE%

            IF EXIST "!AMX_O!" (
                FOR %%A in ("!AMX_O!") DO (
                    ECHO.
                    <nul SET /p=""
                        CALL :COLOURTEXT a "~"
                        cmd /c time /t
                        ECHO.

                        CALL :COLOURTEXT a "[#]~"
						
                        IF "%BATCHSTS%"=="true" (
                            SET "BATCHTITLE=compilers "%ASM_OPTION_M% %ASM_OPTION_P%""
                            TITLE user:~/!BATCHTITLE!
                        ) ELSE IF "%BATCHSTS%"=="false" (
                            SET "BATCHTITLE=compiler - running "%ASM_OPTION_M% %ASM_OPTION_P%""
                            TITLE user:~/!BATCHTITLE!
                        )
                        ECHO Total Size [%%~zA / bytes] ^| "!AMX_O!" ^| "%ASM_OPTION_M% %ASM_OPTION_P%" 
                )
            ) ELSE (
                    setlocal DisableDelayedExpansion 
                        <nul SET /p=""
                            CALL :COLOURTEXT 4X "[#]~"
                            ECHO Compilation failed!. ERR? .. Yes
                    endlocal
                        IF "%BATCHSTS%"=="false" (
                            GOTO BATCHEND
                        )
                )
                ECHO .
            )
        )
    )
    IF not "%BATCHFILE%"=="true" (
        setlocal DisableDelayedExpansion 
            <nul SET /p=""
                    CALL :COLOURTEXT 4X "[#]~"
                    ECHO Compilation failed!.
        endlocal
        ECHO    ~ "!BATCHDIR!.io" no files found.
    )
    IF "%BATCHSTS%"=="true" (
        GOTO BATCHEND
    )

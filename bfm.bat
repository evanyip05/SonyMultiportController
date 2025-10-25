@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

idf.py --version
if %ERRORLEVEL% NEQ 0 (
    GOTO idf
)

if "%1"=="" (
    GOTO usage
) else (
    GOTO bfm
)

:usage
echo USAGE
echo    ./bfm [commands]
echo COMMANDS
echo    f compile program and flash
echo    m monitor usb output
echo    fm compile program, flash, and monitor usb output
GOTO end

:bfm
echo IDF detected!
set "args=%1"
if "!args:f=!" NEQ "!args!" (
    echo Building and flashing...
    set IDF_TARGET=esp32c6
    idf.py --ccache flash
) 

if "!args:m=!" NEQ "!args!" (
    echo Monitoring...
    idf.py monitor
)
GOTO end

:idf
echo IDF not detected, activating...
set parent_shell_type=%CMDCMDLINE%
set parent_shell_type=%parent_shell_type:/=%
set parent_shell_type=%parent_shell_type: =%

if %parent_shell_type% == "C:\Windows\system32\cmd.exe" (
    C:\Windows\system32\cmd.exe /k ""C:\Espressif\idf_cmd_init.bat" esp-idf-29323a3f5a0574597d6dbaa0af20c775
) ELSE (
    C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -ExecutionPolicy Bypass -NoExit -File "C:\Espressif/Initialize-Idf.ps1" -IdfId esp-idf-29323a3f5a0574597d6dbaa0af20c775
)
GOTO end

:end
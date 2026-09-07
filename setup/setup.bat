@echo off
setlocal

set "EXECUTABLE_DIR=%~dp0"
set "EXECUTABLE_DIR=%EXECUTABLE_DIR:~0,-1%"

set "MY_VARIABLE=%EXECUTABLE_DIR%"

set "PATH=%EXECUTABLE_DIR%;%PATH%"

setx EXECUTABLE_DIR "%EXECUTABLE_DIR%" >nul
setx MY_VARIABLE "%MY_VARIABLE%" >nul

setx PATH "%EXECUTABLE_DIR%;%PATH%" >nul

echo Available commands:

for %%E in ("%EXECUTABLE_DIR%\*") do (
    if /I not "%%~nxE"=="setup.bat" (
        echo   %%~nxE
    )
)

pause
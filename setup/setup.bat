@echo off

set "EXECUTABLE_DIR=%~dp0"
set "EXECUTABLE_DIR=%EXECUTABLE_DIR:~0,-1%"

set "PATH=%EXECUTABLE_DIR%;%PATH%"

echo Added to PATH:
echo   %EXECUTABLE_DIR%
echo.

echo Available commands:

for %%E in ("%EXECUTABLE_DIR%\*") do (
    echo   %%~nxE
)
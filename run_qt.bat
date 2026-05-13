@echo off
setlocal

set "MSYS2_UCRT64=C:\msys64\ucrt64"
set "QT_EXE=build-qt-msys\dbms_qt.exe"

if not exist "%MSYS2_UCRT64%\bin\Qt6Widgets.dll" (
    echo [FAIL] Qt6 Widgets runtime not found at "%MSYS2_UCRT64%\bin".
    echo        Install it with:
    echo        C:\msys64\usr\bin\pacman.exe -S --needed mingw-w64-ucrt-x86_64-qt6-base
    exit /b 1
)

set "PATH=%MSYS2_UCRT64%\bin;%PATH%"

if not exist "%QT_EXE%" (
    echo [INFO] Qt executable not found. Building dbms_qt...
    cmake --preset windows-msys2-ucrt64-qt
    if errorlevel 1 exit /b 1
    cmake --build --preset windows-msys2-ucrt64-qt
    if errorlevel 1 exit /b 1
)

echo [OK] Starting %QT_EXE%
start "DBMS Qt" "%QT_EXE%"
endlocal

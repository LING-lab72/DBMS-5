@echo off
setlocal

set "PRESET=windows-msys2-ucrt64"
set "DEFAULT_GXX=C:\msys64\ucrt64\bin\g++.exe"
if not defined DBMS_MINGW_GXX set "DBMS_MINGW_GXX=%DEFAULT_GXX%"
for %%I in ("%DBMS_MINGW_GXX%") do set "DBMS_MINGW_BIN=%%~dpI"
set "PATH=%DBMS_MINGW_BIN%;%PATH%"

:: ---- Check compiler used by the CMake preset wrapper ----
if not exist "%DBMS_MINGW_GXX%" (
    echo [FAIL] g++ not found at "%DBMS_MINGW_GXX%".
    echo        Install MSYS2 UCRT64, or set DBMS_MINGW_GXX to your g++.exe.
    exit /b 1
)
"%DBMS_MINGW_GXX%" --version > "%TEMP%\dbms_gxx_version.txt" 2>&1
set /p GXX_VERSION=<"%TEMP%\dbms_gxx_version.txt"
del /q "%TEMP%\dbms_gxx_version.txt" >nul 2>&1
echo [OK] %GXX_VERSION%

:: ---- Check C++17 ----
echo #include^<optional^> > "%TEMP%\dbms_test17.cpp"
echo int main(){} >> "%TEMP%\dbms_test17.cpp"
"%DBMS_MINGW_GXX%" -std=c++17 "%TEMP%\dbms_test17.cpp" -o "%TEMP%\dbms_test17.exe" >nul 2>&1
if errorlevel 1 (
    echo [FAIL] g++ does not support C++17. Please upgrade to GCC 8+.
    del /q "%TEMP%\dbms_test17.cpp" "%TEMP%\dbms_test17.exe" >nul 2>&1
    exit /b 1
)
echo [OK] C++17 supported
del /q "%TEMP%\dbms_test17.cpp" "%TEMP%\dbms_test17.exe" >nul 2>&1

:: ---- Check cmake ----
where cmake >nul 2>&1
if errorlevel 1 (
    echo [FAIL] cmake not found. Install: https://cmake.org/download/
    exit /b 1
)
cmake --version > "%TEMP%\dbms_cmake_version.txt" 2>nul
set /p CMAKE_VERSION=<"%TEMP%\dbms_cmake_version.txt"
del /q "%TEMP%\dbms_cmake_version.txt" >nul 2>&1
echo [OK] %CMAKE_VERSION%

:: ---- Check node for tools/mingw-gpp-wrapper.cmd ----
where node >nul 2>&1
if errorlevel 1 (
    echo [FAIL] node not found. The Windows CMake preset uses tools\mingw-gpp-wrapper.cmd.
    exit /b 1
)

:: ---- Build with the repository preset ----
echo.
cmake --preset %PRESET%
if errorlevel 1 ( echo [FAIL] cmake configure failed & exit /b 1 )

cmake --build --preset %PRESET%
if errorlevel 1 ( echo [FAIL] build failed & exit /b 1 )

echo.
echo [OK] Build successful. Run: build-windows\dbms.exe
exit /b 0

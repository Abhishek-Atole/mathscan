@echo off
REM MathScan Development Environment Setup
REM This script sets up the correct Qt and MinGW environment for building

echo Setting up MathScan Qt Development Environment...

REM Set Qt and MinGW paths
set QT_DIR=C:\Qt\6.9.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set CMAKE_DIR=C:\Qt\Tools\CMake_64

REM Add to PATH
set PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;%CMAKE_DIR%\bin;%PATH%

REM Set Qt environment variables
set QTDIR=%QT_DIR%
set Qt6_DIR=%QT_DIR%\lib\cmake\Qt6

echo.
echo Environment configured:
echo   Qt Directory: %QT_DIR%
echo   MinGW Directory: %MINGW_DIR%
echo   CMake Directory: %CMAKE_DIR%
echo.

REM Check if we should configure and build
if "%1"=="build" goto build
if "%1"=="configure" goto configure
if "%1"=="run" goto run
if "%1"=="run-ocr" goto run_ocr
if "%1"=="cleanup" goto run_cleanup
if "%1"=="check" goto check
goto end

:configure
echo Configuring CMake...
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%" -DCMAKE_CXX_COMPILER="%MINGW_DIR%\bin\g++.exe"
goto end

:build
echo Building project...
cmake --build build --config Debug
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    goto end
)
echo.
echo Build successful! Qt plugins and DLLs have been automatically copied.
echo.
echo Testing applications...
echo Running Qt environment check...
.\build\qt_checker.exe --check-qt
if %ERRORLEVEL% EQU 0 (
    echo ? Qt environment check PASSED
) else (
    echo ? Qt environment check FAILED
)
goto end

:run
echo Running MathScan application...
.\build\mathscan.exe
goto end

:run_ocr
echo Running OCR ^& PPT Automation Tool...
.\build\ocr_tool.exe
goto end

:run_cleanup
echo Running Project Cleanup Utility...
.\build\cleanup_tool.exe
goto end

:check
echo Running Qt environment check...
.\build\qt_checker.exe --check-qt
goto end

:end
echo.
echo Available commands:
echo   setup-env.bat configure  - Configure CMake
echo   setup-env.bat build      - Build the project
echo   setup-env.bat run        - Run MathScan GUI
echo   setup-env.bat run-ocr    - Run OCR ^& PPT Automation Tool
echo   setup-env.bat cleanup    - Run Project Cleanup Utility
echo   setup-env.bat check      - Run Qt environment checker
echo.
echo Or run commands manually in this environment:
echo   cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%"
echo   cmake --build build
echo   .\build\mathscan.exe       (Original MathScan calculator)
echo   .\build\ocr_tool.exe       (OCR ^& PPT Automation Tool)
echo   .\build\cleanup_tool.exe   (Project Cleanup Utility)
echo   .\build\qt_checker.exe --check-qt

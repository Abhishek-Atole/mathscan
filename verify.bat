@echo off
REM MathScan Application Verification Script
echo ========================================
echo MathScan Application Verification
echo ========================================
echo.

REM Check if executables exist
if not exist "build\mathscan.exe" (
    echo ❌ mathscan.exe not found in build directory
    echo Please run: setup-env.bat build
    goto end
)

if not exist "build\qt_checker.exe" (
    echo ❌ qt_checker.exe not found in build directory
    echo Please run: setup-env.bat build
    goto end
)

echo ✅ Executables found

REM Check if Qt DLLs exist
if not exist "build\Qt6Core.dll" (
    echo ❌ Qt6Core.dll not found
    echo Please rebuild the project
    goto end
)

echo ✅ Qt DLLs found

REM Check if platform plugins exist
if not exist "build\platforms\qwindows.dll" (
    echo ❌ Qt platform plugins not found
    echo Please rebuild the project
    goto end
)

echo ✅ Qt platform plugins found

echo.
echo Running Qt Environment Check...
echo ----------------------------------------
build\qt_checker.exe --check-qt

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✅ Qt environment check PASSED
    echo.
    echo 🚀 Applications are ready to run:
    echo    • Main GUI: .\build\mathscan.exe
    echo    • Qt Checker: .\build\qt_checker.exe --check-qt
    echo.
    echo Would you like to start the main application? (Y/N)
    set /p "choice=Enter choice: "
    if /i "%choice%"=="Y" (
        echo Starting MathScan...
        start "" "build\mathscan.exe"
    )
) else (
    echo.
    echo ❌ Qt environment check FAILED
    echo Please check your Qt installation
)

:end
echo.
pause

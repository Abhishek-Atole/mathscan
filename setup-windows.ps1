# MathScan Qt Development Environment Setup Script
# Run this script in PowerShell as Administrator

Write-Host "Setting up MathScan Qt Development Environment..." -ForegroundColor Green

# Function to check if a command exists
function Test-Command($command) {
    $null = Get-Command $command -ErrorAction SilentlyContinue
    return $?
}

# Check for Git
if (Test-Command "git") {
    Write-Host "✓ Git is installed" -ForegroundColor Green
} else {
    Write-Host "✗ Git is not installed. Please install Git from https://git-scm.com/" -ForegroundColor Red
    exit 1
}

# Check for CMake
if (Test-Command "cmake") {
    Write-Host "✓ CMake is installed" -ForegroundColor Green
    cmake --version
} else {
    Write-Host "✗ CMake is not installed. Please install CMake from https://cmake.org/" -ForegroundColor Red
    exit 1
}

# Check for Qt installation
$qtPaths = @(
    "C:\Qt\6.7.0\mingw_64\bin",
    "C:\Qt\6.6.0\mingw_64\bin",
    "C:\Qt\6.5.0\mingw_64\bin"
)

$qtFound = $false
$qtPath = ""

foreach ($path in $qtPaths) {
    if (Test-Path "$path\qmake.exe") {
        $qtFound = $true
        $qtPath = $path
        break
    }
}

if ($qtFound) {
    Write-Host "✓ Qt is installed at: $qtPath" -ForegroundColor Green
    
    # Add Qt to PATH if not already there
    $currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
    if ($currentPath -notlike "*$qtPath*") {
        Write-Host "Adding Qt to PATH..." -ForegroundColor Yellow
        [Environment]::SetEnvironmentVariable("PATH", "$currentPath;$qtPath", "User")
    }
} else {
    Write-Host "✗ Qt is not found. Please install Qt from https://www.qt.io/download" -ForegroundColor Red
    Write-Host "   Make sure to install Qt 6.5+ with MinGW compiler" -ForegroundColor Yellow
    exit 1
}

# Check for MinGW
$mingwPaths = @(
    "C:\Qt\Tools\mingw1120_64\bin",
    "C:\Qt\Tools\mingw900_64\bin",
    "C:\Qt\Tools\mingw810_64\bin"
)

$mingwFound = $false
$mingwPath = ""

foreach ($path in $mingwPaths) {
    if (Test-Path "$path\gcc.exe") {
        $mingwFound = $true
        $mingwPath = $path
        break
    }
}

if ($mingwFound) {
    Write-Host "✓ MinGW is installed at: $mingwPath" -ForegroundColor Green
    
    # Add MinGW to PATH if not already there
    $currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
    if ($currentPath -notlike "*$mingwPath*") {
        Write-Host "Adding MinGW to PATH..." -ForegroundColor Yellow
        [Environment]::SetEnvironmentVariable("PATH", "$currentPath;$mingwPath", "User")
    }
} else {
    Write-Host "✗ MinGW is not found. Please install Qt with MinGW compiler" -ForegroundColor Red
    exit 1
}

Write-Host "`n" -NoNewline
Write-Host "Environment setup complete!" -ForegroundColor Green
Write-Host "Please restart your terminal/VS Code to use the updated PATH" -ForegroundColor Yellow

# Test build
Write-Host "`nTesting build system..." -ForegroundColor Yellow
if (Test-Path "CMakeLists.txt") {
    Write-Host "Configuring CMake..." -ForegroundColor Yellow
    cmake -B build -S . -G "MinGW Makefiles"
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ CMake configuration successful!" -ForegroundColor Green
        
        Write-Host "Building project..." -ForegroundColor Yellow
        cmake --build build --config Debug
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ Build successful!" -ForegroundColor Green
            Write-Host "You can now run: .\build\mathscan.exe" -ForegroundColor Cyan
        } else {
            Write-Host "✗ Build failed" -ForegroundColor Red
        }
    } else {
        Write-Host "✗ CMake configuration failed" -ForegroundColor Red
    }
} else {
    Write-Host "CMakeLists.txt not found in current directory" -ForegroundColor Yellow
}

Write-Host "`nSetup complete! You can now:" -ForegroundColor Green
Write-Host "1. Open VS Code in this directory" -ForegroundColor White
Write-Host "2. Use Ctrl+Shift+P and run 'CMake: Configure'" -ForegroundColor White
Write-Host "3. Use Ctrl+Shift+P and run 'CMake: Build'" -ForegroundColor White
Write-Host "4. Press F5 to debug the application" -ForegroundColor White

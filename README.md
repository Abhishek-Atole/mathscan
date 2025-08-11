# MathScan - Cross-Platform C++ Qt Development Setup ✅

This project demonstrates a **successfully configured** cross-platform C++ development environment using Qt6 and VS Code, compatible with both Windows and Linux.

## ✅ **Setup Status: COMPLETE & VERIFIED**

- ✅ Qt 6.9.1 with MinGW 13.1.0 installed and configured
- ✅ CMake build system working
- ✅ VS Code integration functional
- ✅ Cross-platform compatibility ensured
- ✅ Sample Qt GUI application built and tested
- ✅ Qt environment checker utility working
- ✅ **Qt platform plugin issue RESOLVED** 🎉
- ✅ **Applications running successfully** 🚀
- ✅ **OCR & PPT Automation Tool implemented** ⭐

## Quick Start

### Option 1: Using the Setup Script (Recommended)

```batch
# Configure and build in one step
setup-env.bat configure
setup-env.bat build

# Run the applications
setup-env.bat run        # Run original MathScan calculator
setup-env.bat run-ocr    # Run OCR & PPT Automation Tool  
setup-env.bat cleanup    # Run Project Cleanup Utility
setup-env.bat check      # Run Qt environment checker
```

### Option 2: Using VS Code

1. Open this project in VS Code
2. Press `Ctrl+Shift+P` → "CMake: Configure"
3. Press `F7` to build
4. Press `F5` to run and debug

### Option 3: Manual Command Line 

```powershell
# Set environment (add to your PowerShell profile for persistence)
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.9.1\mingw_64\bin;" + $env:PATH

# Configure and build
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64"
cmake --build build

# Run applications
.\build\mathscan.exe        # Original calculator GUI
.\build\ocr_tool.exe        # OCR & PPT Automation Tool
.\build\cleanup_tool.exe    # Project Cleanup Utility
.\build\qt_checker.exe --check-qt  # Environment checker
.\build\ocr_tool.exe        # OCR & PPT Automation Tool
.\build\qt_checker.exe --check-qt  # Environment checker
```

## 🎯 **Applications Overview**

### 1. MathScan Calculator (`mathscan.exe`)
- Simple mathematical expression calculator
- Qt6 GUI demonstration
- Cross-platform compatibility test

### 2. OCR & PPT Automation Tool (`ocr_tool.exe`) ⭐ **NEW**
- **Professional desktop application** for OCR processing
- **Object-oriented design** with Qt6 best practices
- **Modern UI** with drag & drop, image preview, and organized panels
- **Robust error handling** and structured logging
- **Cross-platform compatibility** (Windows & Linux)
- **Ready for extension** with OCR engine and PowerPoint automation
- See [OCR_TOOL_README.md](OCR_TOOL_README.md) for detailed documentation

### 3. Project Cleanup Utility (`cleanup_tool.exe`) 🧹 **NEW**
- **Smart cleanup** of unwanted files and directories
- **Safe & conservative** - preserves all source code and important files
- **Dry-run mode** for safe preview before actual cleanup
- **C++17 filesystem-based** utility with detailed progress reporting
- **Cross-platform compatibility** with comprehensive error handling
- See [CLEANUP_UTILITY_README.md](CLEANUP_UTILITY_README.md) for detailed documentation

### 4. Qt Environment Checker (`qt_checker.exe`)
- Diagnostic utility for Qt installation
- Validates Qt modules and environment
- Useful for troubleshooting setup issues

## 🔧 **Troubleshooting**

### Common Issues and Solutions

#### "This application failed to start because no platform plugin could be initialized"

**Problem**: Qt applications can't find platform plugins (qwindows.dll, etc.)

**Solution**: ✅ **AUTOMATICALLY FIXED** - The CMakeLists.txt now automatically copies Qt plugins to the build directory.

**Manual Fix** (if needed):
```powershell
# Copy Qt platform plugins manually
mkdir build\platforms
copy "C:\Qt\6.9.1\mingw_64\plugins\platforms\qwindows.dll" build\platforms\
copy "C:\Qt\6.9.1\mingw_64\plugins\platforms\qminimal.dll" build\platforms\
```

#### CMake Configuration Issues

**Problem**: "Could not find a configuration file for package Qt6"

**Solution**:
```powershell
# Use the setup script
setup-env.bat configure

# Or specify Qt path manually
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64"
```

#### MinGW Compiler Issues

**Problem**: Wrong MinGW version or compiler not found

**Solution**:
```powershell
# Ensure correct MinGW is in PATH
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;" + $env:PATH
```

### Verification Commands

```powershell
# Test Qt environment
.\build\qt_checker.exe --check-qt

# Check if plugins exist
ls build\platforms

# Verify Qt DLLs
ls build\*.dll
```

### Windows Setup

1. **Install Qt6**
   - Download Qt Online Installer from https://www.qt.io/download
   - Install Qt 6.6+ with MinGW compiler
   - Default installation path: `C:\Qt\`
   - Components to install:
     - Qt 6.6.x for MinGW
     - MinGW compiler toolchain
     - Qt Creator (optional)
     - CMake

2. **Install Git**
   - Download from https://git-scm.com/download/win
   - Ensure Git is in your PATH

3. **Install CMake** (if not included with Qt)
   - Download from https://cmake.org/download/
   - Add to PATH during installation

### Linux Setup (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install Qt6 development packages
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools

# Install CMake and build tools
sudo apt install cmake build-essential gdb

# Install additional Qt6 modules (if needed)
sudo apt install qt6-multimedia-dev qt6-webengine-dev
```

### Linux Setup (Fedora/CentOS/RHEL)

```bash
# Install Qt6 development packages
sudo dnf install qt6-qtbase-devel qt6-qttools-devel

# Install CMake and build tools
sudo dnf install cmake gcc-c++ gdb

# Install additional Qt6 modules (if needed)
sudo dnf install qt6-qtmultimedia-devel qt6-qtwebengine-devel
```

## VS Code Extensions (Already Configured)

The following extensions are automatically installed:
- C/C++ Extension Pack (IntelliSense, debugging, code browsing)
- CMake Tools (CMake integration)
- Qt tools (Qt-specific support)

## Building the Project

### Using VS Code

1. Open the project folder in VS Code
2. Press `Ctrl+Shift+P` and run "CMake: Configure"
3. Press `Ctrl+Shift+P` and run "CMake: Build" or use `Ctrl+F7`
4. Or use the build task: `Ctrl+Shift+P` → "Tasks: Run Task" → "CMake: Build"

### Using Command Line

#### Windows (PowerShell)
```powershell
# Configure the build
cmake -B build -S . -G "MinGW Makefiles"

# Build the project
cmake --build build --config Debug

# Run the executable
.\build\mathscan.exe
```

#### Linux (Bash)
```bash
# Configure the build
cmake -B build -S .

# Build the project
cmake --build build --config Debug

# Run the executable
./build/mathscan
```

## Debugging

1. Set breakpoints in your code
2. Press `F5` to start debugging
3. The debugger will automatically build the project and start the application

## Project Structure

```
mathscan/
├── .vscode/                 # VS Code configuration
│   ├── c_cpp_properties.json  # IntelliSense configuration
│   ├── tasks.json             # Build tasks
│   ├── launch.json            # Debug configuration
│   └── settings.json          # Workspace settings
├── src/                     # Source files
│   └── main.cpp            # Main application
├── include/                 # Header files
├── build/                   # Build output (auto-generated)
├── CMakeLists.txt          # CMake configuration
└── README.md               # This file
```

## Features

- **Cross-Platform**: Works on Windows and Linux
- **Qt6 Integration**: Full Qt6 support with widgets
- **CMake Build System**: Modern build configuration
- **VS Code Integration**: IntelliSense, debugging, and tasks
- **MinGW on Windows**: No need for Visual Studio
- **GCC on Linux**: Standard Linux development

## Troubleshooting

### Windows Issues

1. **Qt not found**: Ensure Qt is installed in `C:\Qt\` and MinGW is in PATH
2. **CMake errors**: Verify CMake is installed and in PATH
3. **Compiler not found**: Check MinGW installation and PATH

### Linux Issues

1. **Qt6 not found**: Install qt6-base-dev package
2. **Missing headers**: Install qt6-tools-dev package
3. **Build errors**: Ensure build-essential is installed

### Common Issues

1. **IntelliSense errors**: Run "C/C++: Reset IntelliSense Database"
2. **CMake configuration fails**: Delete build folder and reconfigure
3. **Debug issues**: Check that the correct debugger path is set in launch.json

## Extending the Project

1. Add new source files to `src/` directory
2. Add headers to `include/` directory
3. CMake will automatically detect new files
4. For Qt-specific features, ensure proper Q_OBJECT macro usage

## Qt Resources

- [Qt Documentation](https://doc.qt.io/)
- [Qt Examples](https://doc.qt.io/qt-6/qtexamples.html)
- [CMake Qt Guide](https://doc.qt.io/qt-6/cmake-get-started.html)

This setup provides a solid foundation for cross-platform C++ development with Qt6!
#   m a t h s c a n 
 
 
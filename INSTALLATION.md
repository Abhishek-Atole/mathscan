# Complete Installation Guide for MathScan Qt Development

## 🚀 Quick Start

This guide will help you set up a complete cross-platform C++ development environment with Qt6 on Windows, ensuring compatibility with Linux.

## 📋 Prerequisites Installation

### Step 1: Install Qt6 Framework

#### Windows Installation

1. **Download Qt Online Installer**
   - Go to https://www.qt.io/download-open-source
   - Download the Qt Online Installer for Windows
   - Run the installer as Administrator

2. **Qt Installation Options**
   - Create a Qt account (free)
   - Choose installation directory: `C:\Qt` (recommended)
   - Select the following components:
     - ✅ Qt 6.6.x or 6.7.x
     - ✅ MinGW 11.2.0 64-bit (under Qt 6.x.x)
     - ✅ CMake (under Developer and Designer Tools)
     - ✅ MinGW 11.2.0 64-bit (under Developer and Designer Tools)
     - ✅ Qt Creator (optional but recommended)

3. **Verify Installation**
   - Qt should be installed in `C:\Qt\6.x.x\mingw_64`
   - MinGW should be in `C:\Qt\Tools\mingw1120_64`

#### Linux Installation (Ubuntu/Debian)

```bash
# Update package database
sudo apt update

# Install Qt6 development packages
sudo apt install -y qt6-base-dev qt6-tools-dev qt6-tools-dev-tools

# Install additional Qt6 modules
sudo apt install -y qt6-multimedia-dev qt6-webengine-dev qt6-svg-dev

# Install CMake and build tools
sudo apt install -y cmake build-essential gdb

# Verify installation
qmake6 --version
cmake --version
```

#### Linux Installation (Fedora/CentOS/RHEL)

```bash
# Install Qt6 development packages
sudo dnf install -y qt6-qtbase-devel qt6-qttools-devel cmake gcc-c++ gdb

# Install additional Qt6 modules
sudo dnf install -y qt6-qtmultimedia-devel qt6-qtwebengine-devel qt6-qtsvg-devel

# Verify installation
qmake6 --version
cmake --version
```

### Step 2: Install Additional Tools (Windows)

1. **Git for Windows**
   - Download from https://git-scm.com/download/win
   - Install with default options
   - Ensure "Git from the command line" is selected

2. **Windows PowerShell 5.1+** (usually pre-installed)
   - Verify: Open PowerShell and run `$PSVersionTable.PSVersion`

### Step 3: Configure Environment Variables (Windows)

#### Automatic Configuration (Recommended)

Run the provided PowerShell script:

```powershell
# Run as Administrator
.\setup-windows.ps1
```

#### Manual Configuration

1. **Add Qt to PATH**
   - Open System Properties → Advanced → Environment Variables
   - Add to User PATH:
     - `C:\Qt\6.x.x\mingw_64\bin`
     - `C:\Qt\Tools\mingw1120_64\bin`
     - `C:\Qt\Tools\CMake_64\bin`

2. **Set Qt Environment Variables**
   - `QTDIR` = `C:\Qt\6.x.x\mingw_64`
   - `Qt6_DIR` = `C:\Qt\6.x.x\mingw_64\lib\cmake\Qt6`

3. **Restart Command Prompt/PowerShell**

## 🔧 VS Code Configuration

### Step 4: Install VS Code Extensions

The project is pre-configured with the following extensions:

1. **C/C++ Extension Pack** - Provides IntelliSense, debugging, and code browsing
2. **CMake Tools** - CMake integration and build management  
3. **Qt tools** - Qt-specific development support

These will be automatically suggested when you open the project in VS Code.

### Step 5: Configure VS Code for Qt Development

The project includes pre-configured VS Code settings:

- **`.vscode/c_cpp_properties.json`** - IntelliSense configuration for Qt
- **`.vscode/tasks.json`** - Build tasks for CMake
- **`.vscode/launch.json`** - Debug configuration
- **`.vscode/settings.json`** - Workspace settings

## 🏗️ Building the Project

### Method 1: Using VS Code (Recommended)

1. **Open Project**
   ```bash
   code "E:\Project 01\mathscan"
   ```

2. **Configure CMake**
   - Press `Ctrl+Shift+P`
   - Type "CMake: Configure"
   - Select the MinGW kit when prompted

3. **Build Project**
   - Press `Ctrl+Shift+P`
   - Type "CMake: Build"
   - Or use the shortcut `F7`

4. **Run Application**
   - Press `F5` to run with debugging
   - Or press `Ctrl+F5` to run without debugging

### Method 2: Using Command Line

#### Windows (PowerShell)

```powershell
# Navigate to project directory
cd "E:\Project 01\mathscan"

# Configure CMake (first time only)
cmake -B build -S . -G "MinGW Makefiles"

# Build the project
cmake --build build --config Debug

# Run the application
.\build\mathscan.exe
```

#### Linux (Bash)

```bash
# Navigate to project directory
cd /path/to/mathscan

# Configure CMake (first time only)
cmake -B build -S .

# Build the project
cmake --build build --config Debug

# Run the application
./build/mathscan
```

## 🐛 Debugging

### VS Code Debugging

1. **Set Breakpoints** - Click in the gutter next to line numbers
2. **Start Debugging** - Press `F5`
3. **Debug Controls**:
   - `F5` - Continue
   - `F10` - Step Over
   - `F11` - Step Into
   - `Shift+F11` - Step Out
   - `Shift+F5` - Stop

### GDB Command Line (Linux)

```bash
# Build with debug symbols
cmake --build build --config Debug

# Run with GDB
gdb ./build/mathscan
```

## 🔍 Troubleshooting

### Common Windows Issues

**1. Qt6 not found during CMake configuration**
```
Error: Could not find a package configuration file provided by "Qt6"
```
**Solution:**
- Verify Qt installation path
- Check environment variables
- Run `setup-windows.ps1` script

**2. MinGW compiler not found**
```
Error: CMAKE_CXX_COMPILER not found
```
**Solution:**
- Ensure MinGW is in PATH
- Restart VS Code/Terminal after PATH changes

**3. Platform plugin initialization error** ✅ **FIXED**
```
Error: This application failed to start because no platform plugin could be initialized
```
**Solution:**
- ✅ **AUTOMATICALLY RESOLVED** - CMakeLists.txt now copies Qt plugins
- Qt platform plugins (qwindows.dll, etc.) are automatically copied to build directory
- No manual intervention required

**4. Permission denied errors**
```
Error: Access denied
```
**Solution:**
- Run PowerShell as Administrator
- Check antivirus software interference

### Common Linux Issues

**1. Qt6 development packages missing**
```
Error: qt6-base-dev not found
```
**Solution:**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev
```

**2. CMake version too old**
```
Error: CMake 3.16 or higher required
```
**Solution:**
```bash
# Ubuntu 20.04+
sudo apt install cmake

# Or install latest CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update && sudo apt install cmake
```

### VS Code Specific Issues

**1. IntelliSense not working**
- Press `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database"
- Ensure CMake Tools extension is installed
- Check `.vscode/c_cpp_properties.json` configuration

**2. Build tasks not found**
- Press `Ctrl+Shift+P` → "Tasks: Configure Task"
- Verify `.vscode/tasks.json` exists

**3. Debugging not working**
- Check `.vscode/launch.json` configuration
- Ensure GDB is installed and in PATH
- Verify executable exists in build directory

## 🎯 Next Steps

### Development Workflow

1. **Edit Code** - Modify `.cpp` and `.h` files in `src/` and `include/`
2. **Build** - Use `F7` or CMake: Build
3. **Test** - Run with `F5` (debug) or `Ctrl+F5` (run)
4. **Debug** - Set breakpoints and use debugging features

### Extending the Application

1. **Add New Qt Widgets** - Explore Qt documentation
2. **Implement Math Parser** - Add expression evaluation logic
3. **Add File I/O** - Load/save mathematical expressions
4. **Create Unit Tests** - Add testing framework

### Cross-Platform Considerations

- **File Paths** - Use `QDir` and `QFileInfo` for path operations
- **UI Scaling** - Test on different DPI settings
- **Fonts** - Use system fonts or bundle custom fonts
- **Build Scripts** - Test build process on both platforms

## 📚 Additional Resources

- [Qt Documentation](https://doc.qt.io/qt-6/)
- [CMake Qt Guide](https://doc.qt.io/qt-6/cmake-get-started.html)
- [VS Code C++ Documentation](https://code.visualstudio.com/docs/languages/cpp)
- [Git for Windows](https://gitforwindows.org/)

## 🆘 Getting Help

If you encounter issues:

1. Check this troubleshooting guide
2. Run the diagnostic script: `.\setup-windows.ps1`
3. Verify all prerequisites are installed
4. Check VS Code output panels for detailed error messages
5. Consult Qt and CMake documentation

Happy coding! 🚀

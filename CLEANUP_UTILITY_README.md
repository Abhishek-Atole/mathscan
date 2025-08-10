# Project Cleanup Utility

## Overview
The Project Cleanup Utility is a C++17 console application designed to safely remove unwanted files and directories from your project workspace while preserving all important source code and configuration files.

## Features

### ✅ **Safe & Conservative**
- Preserves all source files (*.cpp, *.h, *.hpp)
- Preserves project files (CMakeLists.txt, README.md, etc.)
- Preserves version control (.git) and IDE settings (.vscode)
- Graceful error handling with detailed logging

### 🗂️ **Removes Unwanted Content**
- **Temporary files:** *.tmp, *.bak, *~, *.swp, *.swo, *.log, *.cache
- **Build directories:** build, debug, release, cmake-build-*
- **IDE artifacts:** .vs, .idea (but preserves .vscode)
- **Language artifacts:** __pycache__, node_modules, *.pyc, *.class
- **Backup files:** *.orig, *.rej, *.patch, *.diff

### 🔍 **Dry-Run Mode**
- Preview what would be deleted without making changes
- Detailed size calculations and impact assessment
- Perfect for safety checks before actual cleanup

## Usage

### Command Line Interface
```bash
# Show help information
.\build\cleanup_tool.exe --help

# Preview what would be cleaned (recommended first)
.\build\cleanup_tool.exe --dry-run

# Perform actual cleanup
.\build\cleanup_tool.exe
```

### Integration with Setup Script
```bash
# Clean project through setup script
.\setup-env.bat cleanup
```

## Example Output

### Dry-Run Mode
```
╔══════════════════════════════════════════════════════════════╗
║                    Project Cleanup Utility                  ║
║                      MathScan Project                        ║
╚══════════════════════════════════════════════════════════════╝

Starting cleanup in project directory: "E:\Project 01\mathscan"
*** DRY RUN MODE - No files will be deleted ***
Scanning for unwanted files and directories...

◇ Would delete file: "test_file.tmp" (58.0 B)
◇ Would delete file: "main.cpp.bak" (50.0 B)
◇ Would delete directory: "build" (47.2 MB)

============================================================
CLEANUP SUMMARY:
============================================================
Files deleted:       2
Directories deleted: 1
Total space freed:   47.2 MB
============================================================
Cleanup completed in 127 ms.

✓ Run without --dry-run to actually delete these files.
```

### Actual Cleanup
```
╔══════════════════════════════════════════════════════════════╗
║                    Project Cleanup Utility                  ║
║                      MathScan Project                        ║
╚══════════════════════════════════════════════════════════════╝

Starting cleanup in project directory: "E:\Project 01\mathscan"
Scanning for unwanted files and directories...

✓ Deleted file: "test_file.tmp" (58.0 B)
✓ Deleted file: "main.cpp.bak" (50.0 B)
✓ Deleted directory: "build" (47.2 MB)

============================================================
CLEANUP SUMMARY:
============================================================
Files deleted:       2
Directories deleted: 1
Total space freed:   47.2 MB
============================================================
Cleanup completed in 156 ms.
```

## Safety Features

### 🔒 **Preserved Directories**
- `.git` - Git version control
- `.vscode` - VS Code workspace settings  
- `src/` - Source code directory
- `include/` - Header files directory
- Any directory containing important project files

### 🔒 **Preserved File Types**
- Source files: `*.cpp`, `*.c`, `*.h`, `*.hpp`, `*.cc`, `*.cxx`
- Project files: `CMakeLists.txt`, `*.cmake`, `Makefile`
- Documentation: `README.*`, `*.md`, `*.txt`
- Configuration: `*.json`, `*.xml`, `*.yml`, `*.yaml`

### ⚠️ **Smart Conflict Resolution**
- Cannot delete files currently in use (like running executables)
- Provides clear error messages for access-denied situations
- Continues cleaning other items even if some fail

## Technical Details

### Requirements
- **C++17 or later** - Uses `std::filesystem`
- **Cross-platform** - Works on Windows, Linux, and macOS
- **No external dependencies** - Pure C++ standard library

### Architecture
```cpp
class ProjectCleanup {
    // Core functionality
    bool isUnwantedFile(const fs::path& path);
    bool isUnwantedDirectory(const fs::path& path);
    void cleanDirectory(const fs::path& dir, bool isRoot);
    
    // Utility methods
    size_t getFileSize(const fs::path& path);
    std::string formatSize(size_t bytes);
    void printSummary();
    
    // Public interface
    void run(bool isDryRun = false);
};
```

### Build Integration
The cleanup utility is integrated into the CMake build system:

```cmake
# Project cleanup utility executable (C++17 standard library only, no Qt)
add_executable(cleanup_tool src/cleanup_main.cpp)
set_target_properties(cleanup_tool PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
```

## Best Practices

### 🎯 **Recommended Workflow**
1. **Always run dry-run first:** `.\build\cleanup_tool.exe --dry-run`
2. **Review the output** to ensure no important files are targeted
3. **Run actual cleanup:** `.\build\cleanup_tool.exe`
4. **Rebuild if needed:** `.\setup-env.bat build`

### 📋 **When to Use**
- Before committing code to remove temporary artifacts
- When switching between debug/release builds
- To free up disk space by removing build outputs
- During project maintenance and reorganization

### ⚠️ **Cautions**
- Always use dry-run mode first in new environments
- The utility cannot delete files that are currently open/locked
- Large build directories may take time to scan and delete
- Consider backing up important work before major cleanups

## Error Handling

The utility provides comprehensive error reporting:

```
✗ Error deleting "build\app.exe": filesystem error: Access is denied
✓ Deleted file: "temp.log" (1.2 KB)
✓ Deleted directory: "cache" (156.8 KB)
```

Common scenarios:
- **Access denied:** File is in use or requires administrator privileges
- **Network drives:** Some operations may fail on network-mounted directories
- **Symlinks:** Handled carefully to avoid deleting linked content

## Integration

The cleanup utility integrates seamlessly with the MathScan development environment:

```bash
# Available through setup script
.\setup-env.bat cleanup

# Manual execution with options
.\build\cleanup_tool.exe --help
.\build\cleanup_tool.exe --dry-run
.\build\cleanup_tool.exe
```

## Future Enhancements

Potential improvements for future versions:
- Configuration file support for custom patterns
- Selective cleanup modes (build-only, temp-only, etc.)
- Integration with Git to respect .gitignore patterns
- GUI version for visual file selection
- Backup creation before deletion
- Scheduled cleanup capabilities

---

**Note:** This utility is designed to be conservative and safe. When in doubt, it preserves files rather than deleting them. Always review the dry-run output before proceeding with actual cleanup operations.

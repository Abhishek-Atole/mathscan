# OCR & PPT Automation Tool

## Overview

The OCR & PPT Automation Tool is a professional desktop application designed for optical character recognition (OCR) processing and PowerPoint automation. Built with Qt6 and C++, it provides a modern, cross-platform solution for document processing and presentation automation.

## Features

### Current Implementation ✅
- **Modern Qt6 GUI**: Professional desktop interface with resizable panels
- **Cross-Platform Compatibility**: Works on Windows and Linux
- **Robust Error Handling**: Comprehensive exception handling and logging
- **Drag & Drop Support**: Easy file input via drag and drop
- **Image Preview**: Visual confirmation of loaded images
- **Settings Persistence**: Saves window layout and user preferences
- **Professional UI**: Menu bar, toolbar, status bar, and organized panels

### Architecture Highlights ✅
- **Object-Oriented Design**: Clean separation of concerns
- **RAII Resource Management**: Automatic memory management via Qt's parent-child system
- **Signal-Slot Architecture**: Decoupled event handling
- **Structured Logging**: Category-based logging for debugging
- **Exception Safety**: Robust error handling throughout

### Planned Features 🚧
- **OCR Engine Integration**: Text recognition from images and PDFs
- **Mathematical Expression Recognition**: Special handling for mathematical content
- **PowerPoint Automation**: Direct export to PowerPoint presentations
- **Batch Processing**: Multiple file processing capabilities
- **OCR Configuration**: Customizable recognition settings

## Application Structure

```
OCR Tool Layout:
┌─────────────────────────────────────────────────────────────┐
│ File  Edit  Tools  Help                              [_][□][×] │
├─────────────────────────────────────────────────────────────┤
│ [Open] [Save] [Export] [Clear]                    Toolbar     │
├──────────────────────┬──────────────────────────────────────┤
│ Input Section        │ OCR Results                          │
│ ┌──────────────────┐ │ ┌──────────────────────────────────┐ │
│ │ [Open Image]     │ │ │                                  │ │
│ │ [Configure]      │ │ │  OCR results will appear here    │ │
│ │                  │ │ │                                  │ │
│ │ File: image.png  │ │ │                                  │ │
│ │                  │ │ │                                  │ │
│ │ ┌──────────────┐ │ │ │                                  │ │
│ │ │              │ │ │ │                                  │ │
│ │ │ Image Preview│ │ │ │                                  │ │
│ │ │              │ │ │ │                                  │ │
│ │ └──────────────┘ │ │ │                                  │ │
│ └──────────────────┘ │ └──────────────────────────────────┘ │
│                      │                                      │ │
│ Processing Section   │ [Save Results] [Export to PPT]      │ │
│ ┌──────────────────┐ │                                      │ │
│ │ [Start OCR]      │ │                                      │ │
│ │ [Clear Results]  │ │                                      │ │
│ │ Progress: 75%    │ │                                      │ │
│ │ Status: Working..│ │                                      │ │
│ └──────────────────┘ │                                      │ │
├──────────────────────┴──────────────────────────────────────┤
│ Ready                                                Status  │
└─────────────────────────────────────────────────────────────┘
```

## Usage

### Basic Workflow

1. **Launch Application**
   ```bash
   .\build\ocr_tool.exe
   # or
   setup-env.bat run-ocr
   ```

2. **Load Image**
   - Click "Open Image File" button, or
   - Drag and drop image file onto the application, or
   - Use File → Open Image (Ctrl+O)

3. **Configure OCR** (when implemented)
   - Click "Configure OCR" for settings
   - Adjust recognition parameters

4. **Process Image**
   - Click "Start OCR Analysis"
   - Monitor progress in the progress bar
   - View results in the text area

5. **Save/Export Results**
   - Edit results if needed
   - Save to text file (Ctrl+S)
   - Export to PowerPoint (Ctrl+E)

### Supported File Formats

- **Images**: PNG, JPG, JPEG, BMP, GIF, TIFF
- **Documents**: PDF (planned)
- **Output**: TXT, PowerPoint presentations

## Technical Details

### Dependencies
- **Qt6**: Core, Widgets, Gui modules
- **C++17**: Modern C++ features
- **CMake**: Build system
- **MinGW/GCC**: Compiler

### Build Requirements
- Qt 6.5+ with MinGW compiler
- CMake 3.16+
- Windows 10+ or Linux with X11/Wayland

### Project Files
```
src/
├── ocr_main.cpp          # Application entry point with robust initialization
└── mainwindow.cpp        # Main window implementation with full UI logic

include/
└── mainwindow.h          # Main window class declaration

.vscode/
├── launch.json           # Debug configuration for OCR tool
├── tasks.json            # Build tasks
└── settings.json         # VS Code workspace settings
```

## Development Features

### Object-Oriented Design
- **MainWindow Class**: Inherits from QMainWindow
- **Signal-Slot Pattern**: Event-driven architecture
- **RAII**: Automatic resource management
- **Exception Safety**: Comprehensive error handling

### Logging and Debugging
```cpp
// Structured logging categories
Q_LOGGING_CATEGORY(startup, "app.startup")
Q_LOGGING_CATEGORY(gui, "app.gui")
Q_LOGGING_CATEGORY(error, "app.error")

// Usage examples
qCInfo(startup) << "Application started successfully";
qCInfo(gui) << "Main window initialized";
qCCritical(error) << "Failed to load image:" << filename;
```

### Cross-Platform Considerations
- **File Paths**: Uses QDir and QFileInfo for path operations
- **UI Scaling**: Handles different DPI settings
- **Platform Plugins**: Automatically copied for deployment
- **Settings Storage**: Platform-appropriate locations

## Debugging and Development

### VS Code Integration
```json
// Launch configuration available:
"(Windows) Debug OCR Tool" - Debug the OCR application
"(Linux) Debug OCR Tool"   - Linux debugging support
```

### Build Commands
```bash
# Quick build and run
setup-env.bat build
setup-env.bat run-ocr

# Development build
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64"
cmake --build build --config Debug

# Debug build
.\build\ocr_tool.exe
```

### Environment Variables
```bash
# Required paths (automatically set by setup-env.bat)
QT_DIR=C:\Qt\6.9.1\mingw_64
MINGW_DIR=C:\Qt\Tools\mingw1310_64
PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;%PATH%
```

## Future Development

### Planned OCR Integration
```cpp
// Future OCR engine interface
class OCREngine {
public:
    virtual QString recognizeText(const QImage& image) = 0;
    virtual QStringList recognizeMath(const QImage& image) = 0;
    virtual void configure(const OCRSettings& settings) = 0;
};
```

### PowerPoint Automation
```cpp
// Future PowerPoint integration
class PowerPointExporter {
public:
    void createPresentation(const OCRResults& results);
    void addSlide(const QString& title, const QString& content);
    void exportToFile(const QString& filename);
};
```

## Troubleshooting

### Common Issues

1. **Application won't start**
   - Ensure Qt DLLs and platform plugins are in place
   - Run `.\build\qt_checker.exe --check-qt` to verify environment

2. **Build failures**
   - Check Qt installation path in CMakeLists.txt
   - Verify MinGW compiler is in PATH
   - Ensure include directory is properly configured

3. **MOC compilation errors**
   - Q_OBJECT macro requires MOC processing
   - Header files must be in include/ directory
   - CMake automatically handles MOC generation

### Getting Help

- Check application logs for detailed error messages
- Use Qt Creator for advanced debugging
- Refer to Qt6 documentation for framework-specific issues
- Review CMake output for build-related problems

## Contributing

When extending the application:

1. **Follow Qt Conventions**: Use Qt's signal-slot pattern
2. **Maintain RAII**: Let Qt handle object lifecycles
3. **Add Logging**: Use appropriate logging categories
4. **Handle Exceptions**: Wrap risky operations in try-catch
5. **Update Documentation**: Keep this file current

---

**Status**: ✅ **Core Framework Complete and Tested**  
**Next Steps**: OCR engine integration and PowerPoint automation implementation

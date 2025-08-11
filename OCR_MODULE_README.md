# OCR Processor Module Documentation

## Overview

The OCR Processor module provides professional-grade Optical Character Recognition (OCR) capabilities to the MathScan application using the Tesseract OCR engine. It offers a Qt-friendly interface with support for text recognition, mathematical equation processing, and robust error handling.

## Features

### 🔍 **Advanced OCR Capabilities**
- **Multi-mode processing:** Auto-detect, Text-only, Mathematical equations, Mixed content
- **High accuracy:** Leverages Tesseract 4+ with LSTM neural networks
- **Confidence scoring:** Provides reliability metrics for extracted text
- **Image preprocessing:** Automatic image enhancement for better OCR results
- **Multi-language support:** Supports all Tesseract-trained languages

### 🖼️ **Image Processing**
- **Format support:** PNG, JPEG, TIFF, BMP, GIF, WebP
- **Automatic preprocessing:** Grayscale conversion, DPI scaling, noise reduction
- **Memory efficient:** Smart image data management with RAII principles
- **Qt integration:** Seamless QImage compatibility

### 🔧 **Professional Features**
- **Thread-safe design:** Safe for multi-threaded applications
- **Configuration management:** Persistent settings with QSettings integration
- **Comprehensive logging:** Detailed operation tracking with Qt logging framework
- **Error handling:** Robust exception handling with descriptive error messages
- **Performance metrics:** Processing time and confidence reporting

## Architecture

### Class Structure

```cpp
class OCRProcessor {
public:
    // Core OCR functionality
    OCRResult performOCR(const QString& imagePath);
    OCRResult performOCR(const QImage& image);
    OCRResult performOCR(const QString& imagePath, ProcessingMode mode);
    
    // Configuration management
    OCRConfig getConfig() const;
    bool setConfig(const OCRConfig& config);
    
    // Utility functions
    bool isInitialized() const;
    QStringList getAvailableLanguages() const;
    QString getTesseractVersion() const;
    
    // Static validation
    static bool validateImage(const QString& imagePath);
    static QStringList getSupportedFormats();
};
```

### Processing Modes

#### 1. **Auto Mode** (Default)
- Automatically detects content type
- Optimal for mixed content documents
- Uses Tesseract's automatic page segmentation

#### 2. **Text Mode**
- Optimized for regular text documents
- Best performance for books, articles, letters
- Uses standard text recognition patterns

#### 3. **Equations Mode**
- Specialized for mathematical content
- Supports mathematical symbols and notation
- Requires equation-specific training data

#### 4. **Mixed Mode**
- Handles documents with both text and equations
- Balanced approach for academic papers
- Comprehensive character whitelist

### Configuration Options

```cpp
struct OCRConfig {
    ProcessingMode mode = ProcessingMode::Auto;
    QString language = "eng";                    // Language code
    int dpi = 300;                              // Processing DPI
    bool preprocessImage = true;                // Enable preprocessing
    bool enableConfidenceScoring = true;       // Enable confidence
    int minimumConfidence = 60;                // Confidence threshold
};
```

### Result Structure

```cpp
struct OCRResult {
    QString text;                               // Extracted text
    float confidence = 0.0f;                   // Confidence score (0-100)
    bool success = false;                      // Success flag
    QString errorMessage;                      // Error description
    QSize imageSize;                           // Image dimensions
    int processingTimeMs = 0;                  // Processing time
};
```

## Installation Requirements

### Dependencies

#### Tesseract OCR Engine
- **Minimum version:** Tesseract 4.0+
- **Recommended:** Tesseract 5.0+ for best accuracy
- **Training data:** Language-specific .traineddata files

#### Leptonica Library
- **Purpose:** Image processing backend for Tesseract
- **Version:** Compatible with your Tesseract installation

### Platform-Specific Installation

#### Windows
```powershell
# Using pre-built binaries (recommended)
# Download from: https://github.com/UB-Mannheim/tesseract/wiki
# Install to: C:\Program Files\Tesseract-OCR

# Or using vcpkg
vcpkg install tesseract:x64-windows
vcpkg install leptonica:x64-windows
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install tesseract-ocr-dev
sudo apt-get install libtesseract-dev
sudo apt-get install libleptonica-dev

# Additional language packs
sudo apt-get install tesseract-ocr-eng
sudo apt-get install tesseract-ocr-equ  # For equations
```

#### macOS
```bash
# Using Homebrew
brew install tesseract
brew install leptonica

# Language data is included automatically
```

### Training Data

#### Required Files
- `eng.traineddata` - English text recognition (required)
- `equ.traineddata` - Mathematical equations (optional)
- Additional languages as needed

#### Installation Paths
- **Linux:** `/usr/share/tesseract-ocr/4.00/tessdata/`
- **Windows:** `C:\Program Files\Tesseract-OCR\tessdata\`
- **macOS:** `/usr/local/share/tessdata/`

## Integration Guide

### CMake Configuration

The module automatically integrates with your CMake build system:

```cmake
# Tesseract detection is automatic
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(TESSERACT tesseract)
    pkg_check_modules(LEPTONICA lept)
endif()

# Fallback manual detection for Windows
if(NOT TESSERACT_FOUND)
    find_path(TESSERACT_INCLUDE_DIR tesseract/baseapi.h)
    find_library(TESSERACT_LIBRARY tesseract)
    find_library(LEPTONICA_LIBRARY lept)
endif()

# Conditional compilation
if(TESSERACT_FOUND)
    add_definitions(-DTESSERACT_AVAILABLE)
    target_include_directories(ocr_tool PRIVATE ${TESSERACT_INCLUDE_DIRS})
    target_link_libraries(ocr_tool ${TESSERACT_LIBRARIES})
endif()
```

### Basic Usage Example

```cpp
#include "ocrprocessor.h"

try {
    // Create processor with default configuration
    OCRProcessor processor;
    
    // Process an image file
    auto result = processor.performOCR("document.png");
    
    if (result.success) {
        qDebug() << "Extracted text:" << result.text;
        qDebug() << "Confidence:" << result.confidence << "%";
        qDebug() << "Processing time:" << result.processingTimeMs << "ms";
    } else {
        qWarning() << "OCR failed:" << result.errorMessage;
    }
    
} catch (const std::exception& e) {
    qCritical() << "OCR initialization failed:" << e.what();
}
```

### Advanced Configuration

```cpp
// Create custom configuration
OCRProcessor::OCRConfig config;
config.mode = OCRProcessor::ProcessingMode::Equations;
config.language = "eng+equ";  // English + equations
config.dpi = 300;
config.preprocessImage = true;
config.minimumConfidence = 75;

// Initialize with custom config
OCRProcessor processor(config);

// Process with different mode temporarily
auto result = processor.performOCR("image.png", 
                                  OCRProcessor::ProcessingMode::Text);
```

### Qt Integration

```cpp
// Integrate with Qt UI
connect(startOCRButton, &QPushButton::clicked, [this]() {
    if (!imagePath.isEmpty() && ocrProcessor) {
        auto result = ocrProcessor->performOCR(imagePath);
        if (result.success) {
            textEdit->setPlainText(result.text);
            statusBar()->showMessage(
                QString("OCR completed - Confidence: %1%")
                .arg(result.confidence, 0, 'f', 1));
        }
    }
});
```

## Performance Optimization

### Image Preprocessing Tips
1. **Optimal DPI:** 300 DPI provides best balance of speed and accuracy
2. **Image format:** PNG and TIFF generally provide better results than JPEG
3. **Contrast:** High contrast images improve recognition accuracy
4. **Resolution:** Minimum text height of 20 pixels recommended

### Memory Management
- OCR processor uses RAII for automatic resource cleanup
- Large images are processed efficiently with streaming
- Memory usage scales with image size and complexity

### Threading Considerations
- OCR processor is thread-safe with internal mutex protection
- Consider using separate processor instances for parallel processing
- Background processing recommended for large documents

## Error Handling

### Common Error Scenarios

#### 1. **Initialization Failures**
```cpp
try {
    OCRProcessor processor;
} catch (const std::runtime_error& e) {
    // Handle Tesseract initialization failure
    qCritical() << "OCR not available:" << e.what();
    // Fall back to manual text input or show error dialog
}
```

#### 2. **Processing Failures**
```cpp
auto result = processor.performOCR("image.png");
if (!result.success) {
    if (result.confidence < 60) {
        // Low confidence - suggest image quality improvement
        showLowConfidenceWarning();
    } else {
        // Other error - check error message
        qWarning() << "OCR error:" << result.errorMessage;
    }
}
```

#### 3. **Language Availability**
```cpp
QStringList available = processor.getAvailableLanguages();
if (!available.contains("equ")) {
    qWarning() << "Equation recognition not available";
    // Disable equation mode in UI
}
```

### Troubleshooting Guide

#### Issue: "Failed to initialize Tesseract"
- **Cause:** Tesseract not installed or training data missing
- **Solution:** Install Tesseract and verify tessdata directory
- **Check:** Ensure `eng.traineddata` exists in tessdata path

#### Issue: Low OCR accuracy
- **Cause:** Poor image quality or incorrect settings
- **Solutions:**
  - Increase image DPI (try 300-600)
  - Enable image preprocessing
  - Use appropriate processing mode
  - Check image contrast and resolution

#### Issue: "Language not available"
- **Cause:** Language training data not installed
- **Solution:** Download and install required .traineddata files
- **Location:** Place in Tesseract tessdata directory

## Testing and Validation

### Unit Testing

```cpp
// Test OCR processor initialization
void testOCRProcessorInit() {
    try {
        OCRProcessor processor;
        QVERIFY(processor.isInitialized());
        QVERIFY(!processor.getAvailableLanguages().isEmpty());
    } catch (...) {
        QSKIP("Tesseract not available");
    }
}

// Test image validation
void testImageValidation() {
    QVERIFY(OCRProcessor::validateImage("test.png"));
    QVERIFY(!OCRProcessor::validateImage("nonexistent.png"));
    QVERIFY(!OCRProcessor::validateImage("test.txt"));
}
```

### Performance Benchmarks

Typical performance metrics on modern hardware:

| Image Size | Text Complexity | Processing Time | Memory Usage |
|------------|----------------|----------------|--------------|
| 1MP (Text) | Simple         | 200-500ms     | 50-100MB     |
| 1MP (Mixed)| Medium         | 500-1000ms    | 100-200MB    |
| 5MP (Text) | Complex        | 1-3s          | 200-500MB    |

## Future Enhancements

### Planned Features
1. **GPU acceleration** for faster processing
2. **Custom training data** support for domain-specific recognition
3. **Batch processing** for multiple files
4. **Real-time OCR** for camera input
5. **Layout analysis** for complex document structures

### Contributing

To contribute improvements to the OCR module:

1. **Performance optimizations** - Image preprocessing algorithms
2. **Language support** - Additional language configurations
3. **Error handling** - Better error recovery mechanisms
4. **Testing** - Additional test cases and benchmarks

---

This OCR module provides a solid foundation for text recognition in the MathScan application, with room for expansion and customization based on specific needs.

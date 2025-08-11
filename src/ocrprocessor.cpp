/*
 * Module: OCRProcessor Implementation
 *
 * This module provides a comprehensive Qt-friendly interface to Tesseract OCR,
 * with support for text and mathematical equation recognition, image preprocessing,
 * robust error handling, and professional logging.
 */

#include "ocrprocessor.h"

// Tesseract includes
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>

// Qt includes
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QMutexLocker>
#include <QStandardPaths>

// Standard library includes
#include <algorithm>
#include <cstring>
#include <stdexcept>

// Logging category definition
Q_LOGGING_CATEGORY(ocrProcessor, "ocr.processor")

// Static member initialization
QStringList OCRProcessor::s_supportedFormats;
bool OCRProcessor::s_supportedFormatsInitialized = false;

/**
 * @brief Default constructor with standard configuration
 */
OCRProcessor::OCRProcessor() : OCRProcessor(OCRConfig{}) {}

/**
 * @brief Constructor with custom configuration
 */
OCRProcessor::OCRProcessor(const OCRConfig& config)
    : m_tesseractAPI(std::make_unique<TessBaseAPI>()), m_config(config), m_initialized(false) {
    qCDebug(ocrProcessor) << "Initializing OCRProcessor with language:" << m_config.language;

    if (!initializeTesseract()) {
        throw std::runtime_error("Failed to initialize Tesseract OCR engine");
    }

    if (!applyConfiguration()) {
        throw std::runtime_error("Failed to apply OCR configuration");
    }

    m_initialized = true;
    qCInfo(ocrProcessor) << "OCRProcessor successfully initialized";
}

/**
 * @brief Destructor
 */
OCRProcessor::~OCRProcessor() {
    cleanupTesseract();
    qCDebug(ocrProcessor) << "OCRProcessor destroyed";
}

/**
 * @brief Move constructor
 */
OCRProcessor::OCRProcessor(OCRProcessor&& other) noexcept
    : m_tesseractAPI(std::move(other.m_tesseractAPI)),
      m_config(std::move(other.m_config)),
      m_initialized(other.m_initialized),
      m_tesseractDataPath(std::move(other.m_tesseractDataPath)) {
    other.m_initialized = false;
}

/**
 * @brief Move assignment operator
 */
OCRProcessor& OCRProcessor::operator=(OCRProcessor&& other) noexcept {
    if (this != &other) {
        cleanupTesseract();

        m_tesseractAPI = std::move(other.m_tesseractAPI);
        m_config = std::move(other.m_config);
        m_initialized = other.m_initialized;
        m_tesseractDataPath = std::move(other.m_tesseractDataPath);

        other.m_initialized = false;
    }
    return *this;
}

/**
 * @brief Perform OCR on image file
 */
OCRProcessor::OCRResult OCRProcessor::performOCR(const QString& imagePath) {
    QMutexLocker locker(&m_mutex);

    OCRResult result;
    QElapsedTimer timer;
    timer.start();

    qCDebug(ocrProcessor) << "Starting OCR processing for:" << imagePath;

    // Validate input
    if (!m_initialized) {
        result.errorMessage = "OCR processor not initialized";
        qCWarning(ocrProcessor) << result.errorMessage;
        return result;
    }

    if (!validateImage(imagePath)) {
        result.errorMessage = QString("Invalid or unsupported image file: %1").arg(imagePath);
        qCWarning(ocrProcessor) << result.errorMessage;
        return result;
    }

    // Load image
    QImage image(imagePath);
    if (image.isNull()) {
        result.errorMessage = QString("Failed to load image: %1").arg(imagePath);
        qCWarning(ocrProcessor) << result.errorMessage;
        return result;
    }

    result.imageSize = image.size();
    qCDebug(ocrProcessor) << "Loaded image size:" << result.imageSize;

    // Process the image
    result = performOCR(image);
    result.processingTimeMs = timer.elapsed();

    logOCROperation(QString("File: %1").arg(imagePath), result);
    return result;
}

/**
 * @brief Perform OCR on QImage
 */
OCRProcessor::OCRResult OCRProcessor::performOCR(const QImage& image) {
    QMutexLocker locker(&m_mutex);

    OCRResult result;
    QElapsedTimer timer;
    timer.start();

    qCDebug(ocrProcessor) << "Starting OCR processing for QImage";

    if (!m_initialized) {
        result.errorMessage = "OCR processor not initialized";
        qCWarning(ocrProcessor) << result.errorMessage;
        return result;
    }

    if (image.isNull()) {
        result.errorMessage = "Invalid image provided";
        qCWarning(ocrProcessor) << result.errorMessage;
        return result;
    }

    result.imageSize = image.size();

    try {
        // Preprocess image if enabled
        QImage processedImage = m_config.preprocessImage ? preprocessImage(image) : image;

        // Convert image for Tesseract
        ImageData imageData = convertImageForTesseract(processedImage);

        // Extract text
        result = extractText(imageData);
        result.processingTimeMs = timer.elapsed();

    } catch (const std::exception& e) {
        result.errorMessage = QString("OCR processing failed: %1").arg(e.what());
        qCWarning(ocrProcessor) << result.errorMessage;
    }

    logOCROperation("QImage processing", result);
    return result;
}

/**
 * @brief Perform OCR with specific processing mode
 */
OCRProcessor::OCRResult OCRProcessor::performOCR(const QString& imagePath, ProcessingMode mode) {
    // Temporarily change processing mode
    ProcessingMode originalMode = m_config.mode;
    m_config.mode = mode;

    // Apply new configuration
    applyConfiguration();

    // Perform OCR
    OCRResult result = performOCR(imagePath);

    // Restore original mode
    m_config.mode = originalMode;
    applyConfiguration();

    return result;
}

/**
 * @brief Get current configuration
 */
OCRProcessor::OCRConfig OCRProcessor::getConfig() const {
    QMutexLocker locker(&m_mutex);
    return m_config;
}

/**
 * @brief Set new configuration
 */
bool OCRProcessor::setConfig(const OCRConfig& config) {
    QMutexLocker locker(&m_mutex);

    qCDebug(ocrProcessor) << "Updating OCR configuration";
    m_config = config;

    return applyConfiguration();
}

/**
 * @brief Check if processor is initialized
 */
bool OCRProcessor::isInitialized() const {
    QMutexLocker locker(&m_mutex);
    return m_initialized;
}

/**
 * @brief Get available languages
 */
QStringList OCRProcessor::getAvailableLanguages() const {
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) {
        return QStringList();
    }

    QStringList languages;

    // Get available languages from Tesseract
    GenericVector<STRING> available_languages;
    m_tesseractAPI->GetAvailableLanguagesAsVector(&available_languages);

    for (int i = 0; i < available_languages.size(); ++i) {
        languages.append(QString::fromStdString(available_languages[i].c_str()));
    }

    qCDebug(ocrProcessor) << "Available languages:" << languages;
    return languages;
}

/**
 * @brief Get Tesseract version
 */
QString OCRProcessor::getTesseractVersion() const {
    return QString::fromStdString(TessBaseAPI::Version());
}

/**
 * @brief Validate image file
 */
bool OCRProcessor::validateImage(const QString& imagePath) {
    QFileInfo fileInfo(imagePath);

    // Check if file exists
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    // Check file extension
    QString suffix = fileInfo.suffix().toLower();
    QStringList supportedFormats = getSupportedFormats();

    return supportedFormats.contains(suffix);
}

/**
 * @brief Get supported image formats
 */
QStringList OCRProcessor::getSupportedFormats() {
    if (!s_supportedFormatsInitialized) {
        s_supportedFormats.clear();

        // Get supported formats from QImageReader
        QList<QByteArray> supportedByteArrays = QImageReader::supportedImageFormats();

        for (const QByteArray& format : supportedByteArrays) {
            QString formatString = QString::fromUtf8(format).toLower();

            // Filter common image formats suitable for OCR
            if (formatString == "png" || formatString == "jpg" || formatString == "jpeg" ||
                formatString == "tiff" || formatString == "tif" || formatString == "bmp" ||
                formatString == "gif" || formatString == "webp") {
                s_supportedFormats.append(formatString);
            }
        }

        s_supportedFormatsInitialized = true;
    }

    return s_supportedFormats;
}

/**
 * @brief Initialize Tesseract
 */
bool OCRProcessor::initializeTesseract() {
    qCDebug(ocrProcessor) << "Initializing Tesseract OCR engine";

    // Try to find Tesseract data path
    QStringList possiblePaths = {
        QCoreApplication::applicationDirPath() + "/tessdata",
        QDir::currentPath() + "/tessdata",
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/tessdata",
        "/usr/share/tesseract-ocr/4.00/tessdata",   // Linux
        "/usr/share/tesseract-ocr/tessdata",        // Linux alternative
        "C:/Program Files/Tesseract-OCR/tessdata",  // Windows
        "C:/tools/tesseract/tessdata"               // Windows alternative
    };

    for (const QString& path : possiblePaths) {
        QDir dir(path);
        if (dir.exists() && dir.exists("eng.traineddata")) {
            m_tesseractDataPath = path;
            qCDebug(ocrProcessor) << "Found Tesseract data at:" << m_tesseractDataPath;
            break;
        }
    }

    if (m_tesseractDataPath.isEmpty()) {
        qCWarning(ocrProcessor) << "Could not find Tesseract training data";
        // Try to initialize without explicit path
    }

    // Initialize Tesseract
    const char* dataPath =
        m_tesseractDataPath.isEmpty() ? nullptr : m_tesseractDataPath.toLocal8Bit().constData();

    int result = m_tesseractAPI->Init(dataPath, m_config.language.toLocal8Bit().constData());

    if (result != 0) {
        qCCritical(ocrProcessor) << "Failed to initialize Tesseract with language:"
                                 << m_config.language;
        return false;
    }

    qCInfo(ocrProcessor) << "Tesseract initialized successfully with language:"
                         << m_config.language;
    return true;
}

/**
 * @brief Cleanup Tesseract resources
 */
void OCRProcessor::cleanupTesseract() {
    if (m_tesseractAPI) {
        m_tesseractAPI->End();
        qCDebug(ocrProcessor) << "Tesseract resources cleaned up";
    }
    m_initialized = false;
}

/**
 * @brief Apply configuration to Tesseract
 */
bool OCRProcessor::applyConfiguration() {
    if (!m_tesseractAPI) {
        return false;
    }

    qCDebug(ocrProcessor) << "Applying OCR configuration";

    // Set page segmentation mode based on processing mode
    tesseract::PageSegMode psm;
    switch (m_config.mode) {
        case ProcessingMode::Text:
            psm = tesseract::PSM_AUTO;
            break;
        case ProcessingMode::Equations:
            psm = tesseract::PSM_SINGLE_BLOCK;
            break;
        case ProcessingMode::Mixed:
            psm = tesseract::PSM_AUTO;
            break;
        default:
            psm = tesseract::PSM_AUTO;
            break;
    }

    m_tesseractAPI->SetPageSegMode(psm);

    // Set OCR Engine Mode
    m_tesseractAPI->SetVariable("tessedit_ocr_engine_mode", "1");  // Use LSTM OCR engine

    // Set DPI if specified
    if (m_config.dpi > 0) {
        m_tesseractAPI->SetVariable("user_defined_dpi",
                                    QString::number(m_config.dpi).toLocal8Bit().constData());
    }

    // Configure equation recognition if needed
    if (m_config.mode == ProcessingMode::Equations || m_config.mode == ProcessingMode::Mixed) {
        m_tesseractAPI->SetVariable(
            "tessedit_char_whitelist",
            "0123456789+-*/=()[]{}^_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,");
    }

    qCDebug(ocrProcessor) << "Configuration applied successfully";
    return true;
}

/**
 * @brief Preprocess image for better OCR
 */
QImage OCRProcessor::preprocessImage(const QImage& image) const {
    qCDebug(ocrProcessor) << "Preprocessing image for OCR";

    QImage processed = image;

    // Convert to grayscale if not already
    if (processed.format() != QImage::Format_Grayscale8) {
        processed = processed.convertToFormat(QImage::Format_Grayscale8);
    }

    // Scale image if needed (optimal DPI for OCR is usually 300)
    if (m_config.dpi > 0 && m_config.dpi != 300) {
        double scaleFactor = m_config.dpi / 300.0;
        QSize newSize = processed.size() * scaleFactor;
        processed = processed.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    qCDebug(ocrProcessor) << "Image preprocessed, new size:" << processed.size();
    return processed;
}

/**
 * @brief Convert QImage to Tesseract format
 */
OCRProcessor::ImageData OCRProcessor::convertImageForTesseract(const QImage& image) const {
    qCDebug(ocrProcessor) << "Converting image for Tesseract processing";

    ImageData imageData;

    // Convert to RGB32 format for consistent processing
    QImage rgbImage = image.convertToFormat(QImage::Format_RGB32);

    imageData.width = rgbImage.width();
    imageData.height = rgbImage.height();
    imageData.bytesPerPixel = 4;  // RGB32 format
    imageData.bytesPerLine = rgbImage.bytesPerLine();

    // Allocate memory for image data
    size_t dataSize = imageData.height * imageData.bytesPerLine;
    imageData.data = std::make_unique<unsigned char[]>(dataSize);

    // Copy image data
    std::memcpy(imageData.data.get(), rgbImage.constBits(), dataSize);

    qCDebug(ocrProcessor) << "Image converted:" << imageData.width << "x" << imageData.height;
    return imageData;
}

/**
 * @brief Extract text using Tesseract
 */
OCRProcessor::OCRResult OCRProcessor::extractText(const ImageData& imageData) {
    qCDebug(ocrProcessor) << "Extracting text with Tesseract";

    OCRResult result;

    try {
        // Set image data in Tesseract
        m_tesseractAPI->SetImage(imageData.data.get(), imageData.width, imageData.height,
                                 imageData.bytesPerPixel, imageData.bytesPerLine);

        // Perform OCR
        char* ocrResult = m_tesseractAPI->GetUTF8Text();

        if (ocrResult) {
            result.text = QString::fromUtf8(ocrResult);
            delete[] ocrResult;

            // Get confidence score if enabled
            if (m_config.enableConfidenceScoring) {
                result.confidence = m_tesseractAPI->MeanTextConf();

                // Check if confidence meets minimum threshold
                if (result.confidence >= m_config.minimumConfidence) {
                    result.success = true;
                } else {
                    result.errorMessage = QString("OCR confidence (%1%) below threshold (%2%)")
                                              .arg(result.confidence)
                                              .arg(m_config.minimumConfidence);
                    qCWarning(ocrProcessor) << result.errorMessage;
                }
            } else {
                result.success = !result.text.trimmed().isEmpty();
            }

            qCDebug(ocrProcessor) << "OCR completed. Text length:" << result.text.length()
                                  << "Confidence:" << result.confidence;
        } else {
            result.errorMessage = "Tesseract failed to extract text";
            qCWarning(ocrProcessor) << result.errorMessage;
        }

    } catch (const std::exception& e) {
        result.errorMessage = QString("Exception during text extraction: %1").arg(e.what());
        qCCritical(ocrProcessor) << result.errorMessage;
    }

    return result;
}

/**
 * @brief Log OCR operation
 */
void OCRProcessor::logOCROperation(const QString& operation, const OCRResult& result) const {
    if (result.success) {
        qCInfo(ocrProcessor) << "OCR SUCCESS -" << operation
                             << "| Text length:" << result.text.length()
                             << "| Confidence:" << result.confidence
                             << "| Time:" << result.processingTimeMs << "ms";
    } else {
        qCWarning(ocrProcessor) << "OCR FAILED -" << operation << "| Error:" << result.errorMessage
                                << "| Time:" << result.processingTimeMs << "ms";
    }
}

/*
 * Module: OCRProcessor
 *
 * Objective:
 * - Integrate Tesseract OCR into the Qt C++ application.
 * - Load an image file from disk.
 * - Process the image with Tesseract to extract text.
 * - Support recognition of text and mathematical equations.
 * - Return extracted text as QString for further processing.
 * - Provide robust error handling and logging.
 *
 * Requirements:
 * - Use Tesseract OCR C++ API.
 * - Use Qt classes for image handling (QImage).
 * - Keep code modular and maintainable.
 * - Include detailed comments suitable for professional developers.
 */

#ifndef OCRPROCESSOR_H
#define OCRPROCESSOR_H

#include <QImage>
#include <QLoggingCategory>
#include <QMutex>
#include <QString>
#include <memory>

// Forward declarations to avoid exposing Tesseract headers in the interface
typedef struct TessBaseAPI TessBaseAPI;

// Logging category for OCR operations
Q_DECLARE_LOGGING_CATEGORY(ocrProcessor)

/**
 * @brief Professional OCR processing class using Tesseract OCR engine
 *
 * This class provides a Qt-friendly interface to the Tesseract OCR library,
 * handling image loading, preprocessing, text extraction, and mathematical
 * equation recognition. Designed for thread-safety and robust error handling.
 */
class OCRProcessor {
   public:
    /**
     * @brief OCR processing modes for different content types
     */
    enum class ProcessingMode {
        Auto,       ///< Automatic detection of content type
        Text,       ///< Standard text recognition
        Equations,  ///< Mathematical equations (using equation-specific training data)
        Mixed       ///< Mixed text and equations
    };

    /**
     * @brief OCR processing configuration
     */
    struct OCRConfig {
        ProcessingMode mode = ProcessingMode::Auto;
        QString language = "eng";             ///< Tesseract language code (eng, eng+equ, etc.)
        int dpi = 300;                        ///< Image DPI for processing
        bool preprocessImage = true;          ///< Enable image preprocessing
        bool enableConfidenceScoring = true;  ///< Enable confidence scoring
        int minimumConfidence = 60;           ///< Minimum confidence threshold (0-100)
    };

    /**
     * @brief OCR processing result with metadata
     */
    struct OCRResult {
        QString text;              ///< Extracted text
        float confidence = 0.0f;   ///< Overall confidence score (0-100)
        bool success = false;      ///< Processing success flag
        QString errorMessage;      ///< Error message if processing failed
        QSize imageSize;           ///< Size of processed image
        int processingTimeMs = 0;  ///< Processing time in milliseconds
    };

   public:
    /**
     * @brief Default constructor - initializes Tesseract with default configuration
     * @throws std::runtime_error if Tesseract initialization fails
     */
    OCRProcessor();

    /**
     * @brief Constructor with custom configuration
     * @param config OCR processing configuration
     * @throws std::runtime_error if Tesseract initialization fails
     */
    explicit OCRProcessor(const OCRConfig& config);

    /**
     * @brief Destructor - properly cleans up Tesseract resources
     */
    ~OCRProcessor();

    // Disable copy constructor and assignment operator to prevent issues with Tesseract API
    OCRProcessor(const OCRProcessor&) = delete;
    OCRProcessor& operator=(const OCRProcessor&) = delete;

    // Enable move constructor and assignment operator
    OCRProcessor(OCRProcessor&& other) noexcept;
    OCRProcessor& operator=(OCRProcessor&& other) noexcept;

    /**
     * @brief Perform OCR on the image at the given file path
     * @param imagePath Path to the image file to process
     * @return OCRResult containing extracted text and metadata
     */
    OCRResult performOCR(const QString& imagePath);

    /**
     * @brief Perform OCR on a QImage object
     * @param image QImage to process
     * @return OCRResult containing extracted text and metadata
     */
    OCRResult performOCR(const QImage& image);

    /**
     * @brief Perform OCR with custom processing mode
     * @param imagePath Path to the image file to process
     * @param mode Processing mode to use for this operation
     * @return OCRResult containing extracted text and metadata
     */
    OCRResult performOCR(const QString& imagePath, ProcessingMode mode);

    /**
     * @brief Get current OCR configuration
     * @return Current OCRConfig
     */
    OCRConfig getConfig() const;

    /**
     * @brief Update OCR configuration
     * @param config New configuration to apply
     * @return true if configuration was successfully applied, false otherwise
     */
    bool setConfig(const OCRConfig& config);

    /**
     * @brief Check if Tesseract is properly initialized
     * @return true if initialized and ready for processing
     */
    bool isInitialized() const;

    /**
     * @brief Get list of available languages for OCR
     * @return QStringList of available language codes
     */
    QStringList getAvailableLanguages() const;

    /**
     * @brief Get Tesseract version information
     * @return QString containing version information
     */
    QString getTesseractVersion() const;

    /**
     * @brief Validate image file for OCR processing
     * @param imagePath Path to image file
     * @return true if image is valid and can be processed
     */
    static bool validateImage(const QString& imagePath);

    /**
     * @brief Get supported image formats for OCR
     * @return QStringList of supported file extensions
     */
    static QStringList getSupportedFormats();

   private:
    /**
     * @brief Initialize Tesseract OCR engine
     * @return true if initialization successful
     */
    bool initializeTesseract();

    /**
     * @brief Cleanup Tesseract resources
     */
    void cleanupTesseract();

    /**
     * @brief Apply OCR configuration to Tesseract
     * @return true if configuration was successfully applied
     */
    bool applyConfiguration();

    /**
     * @brief Preprocess image for better OCR results
     * @param image Input image
     * @return Preprocessed image
     */
    QImage preprocessImage(const QImage& image) const;

    /**
     * @brief Convert QImage to format suitable for Tesseract
     * @param image Input QImage
     * @return Processed image data and metadata
     */
    struct ImageData {
        std::unique_ptr<unsigned char[]> data;
        int width;
        int height;
        int bytesPerPixel;
        int bytesPerLine;
    };
    ImageData convertImageForTesseract(const QImage& image) const;

    /**
     * @brief Extract text from image data using Tesseract
     * @param imageData Image data prepared for Tesseract
     * @return OCRResult with extracted text and metadata
     */
    OCRResult extractText(const ImageData& imageData);

    /**
     * @brief Log OCR operation details
     * @param operation Operation description
     * @param result OCR result to log
     */
    void logOCROperation(const QString& operation, const OCRResult& result) const;

   private:
    std::unique_ptr<TessBaseAPI> m_tesseractAPI;  ///< Tesseract API instance
    OCRConfig m_config;                           ///< Current OCR configuration
    mutable QMutex m_mutex;                       ///< Thread safety mutex
    bool m_initialized;                           ///< Initialization status flag
    QString m_tesseractDataPath;                  ///< Path to Tesseract training data

    // Static members for shared resources
    static QStringList s_supportedFormats;      ///< Cached list of supported formats
    static bool s_supportedFormatsInitialized;  ///< Flag for format initialization
};

#endif  // OCRPROCESSOR_H

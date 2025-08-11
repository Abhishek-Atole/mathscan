/*
 * MainWindow.h - Main application window for OCR & PPT Automation Tool
 *
 * This class implements the primary user interface for the OCR & PPT Automation Tool.
 * It follows Qt best practices and object-oriented design principles for maintainability
 * and extensibility.
 *
 * Key Features:
 * - Clean separation of concerns with dedicated UI setup methods
 * - Signal-slot architecture for event handling
 * - Resource management through Qt's parent-child system
 * - Cross-platform compatibility considerations
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenuBar>
#include <QMimeData>
#include <QProgressBar>

#ifdef TESSERACT_AVAILABLE
#include "ocrprocessor.h"
#endif
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

// Forward declarations for better compilation times
class QTimer;
class QSettings;

// Logging category
Q_DECLARE_LOGGING_CATEGORY(gui)

/**
 * @brief Main application window class
 *
 * This class implements the primary user interface for the OCR & PPT Automation Tool.
 * It provides a clean, intuitive interface for OCR operations and PowerPoint automation.
 *
 * Design Patterns Used:
 * - Observer pattern (Qt signals/slots)
 * - Template method pattern (virtual methods for extensibility)
 * - RAII for resource management
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    /**
     * @brief Construct the main window
     * @param parent Parent widget (typically nullptr for main window)
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor - Qt handles cleanup automatically via parent-child relationships
     */
    ~MainWindow() override;

   protected:
    /**
     * @brief Handle drag enter events for file drops
     * @param event Drag enter event
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief Handle drop events for file drops
     * @param event Drop event
     */
    void dropEvent(QDropEvent *event) override;

    /**
     * @brief Handle window close events
     * @param event Close event
     */
    void closeEvent(QCloseEvent *event) override;

   private slots:
    // File operations
    void onOpenFile();
    void onSaveResults();
    void onExportToPPT();
    void onExit();

    // OCR operations
    void onStartOCR();
    void onClearResults();
    void onConfigureOCR();
    void onOCRModeChanged();
    void performOCROnCurrentImage();
    void updateOCRProgress(int percentage);
    void displayOCRResults(const QString &text, float confidence);

    // Help and information
    void onAbout();
    void onShowHelp();
    void onCheckUpdates();

    // Internal operations
    void onProgressUpdate(int value);
    void onOperationCompleted();
    void onStatusUpdate(const QString &message);

   private:
    // UI setup methods
    void setupUserInterface();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupConnections();

    // Utility methods
    void loadSettings();
    void saveSettings();
    void updateProgress(int percentage, const QString &message = QString());
    void setOperationEnabled(bool enabled);
    void logMessage(const QString &message, bool isError = false);

    // Central widget and main layout
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;

    // Input section widgets
    QGroupBox *m_inputGroup;
    QVBoxLayout *m_inputLayout;
    QPushButton *m_openFileButton;
    QPushButton *m_configureButton;
    QLabel *m_filePathLabel;
    QLabel *m_imagePreviewLabel;

    // OCR processing section
    QGroupBox *m_processingGroup;
    QVBoxLayout *m_processingLayout;
    QPushButton *m_startOCRButton;
    QPushButton *m_clearButton;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;

    // Results section
    QGroupBox *m_resultsGroup;
    QVBoxLayout *m_resultsLayout;
    QTextEdit *m_resultsTextEdit;
    QPushButton *m_saveResultsButton;
    QPushButton *m_exportToPPTButton;

    // Menu items
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_toolsMenu;
    QMenu *m_helpMenu;

    // Actions
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    QAction *m_clearAction;
    QAction *m_configureAction;
    QAction *m_aboutAction;
    QAction *m_helpAction;

    // Toolbar
    QToolBar *m_mainToolBar;

    // Settings and state management
    QSettings *m_settings;
    QString m_currentFilePath;
    bool m_operationInProgress;

#ifdef TESSERACT_AVAILABLE
    // OCR processing
    std::unique_ptr<OCRProcessor> m_ocrProcessor;
    OCRProcessor::ProcessingMode m_currentOCRMode;
    QString m_lastOCRResult;
    float m_lastOCRConfidence;
#endif

    // Constants for UI layout
    static constexpr int DEFAULT_WINDOW_WIDTH = 900;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 700;
    static constexpr int MINIMUM_WINDOW_WIDTH = 600;
    static constexpr int MINIMUM_WINDOW_HEIGHT = 400;
    static constexpr int SPLITTER_HANDLE_WIDTH = 5;
};

#endif  // MAINWINDOW_H

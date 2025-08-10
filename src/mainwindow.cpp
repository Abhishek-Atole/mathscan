/*
 * MainWindow.cpp - Implementation of the main application window
 *
 * This file implements the MainWindow class with complete UI setup,
 * event handling, and application logic for the OCR & PPT Automation Tool.
 */

#include "../include/mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QPixmap>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QUrl>

// Note: Logging category is defined in ocr_main.cpp to avoid multiple definitions

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_centralWidget(nullptr),
      m_mainSplitter(nullptr),
      m_settings(nullptr),
      m_operationInProgress(false) {
    qCInfo(gui) << "Initializing MainWindow...";

    try {
        // Initialize settings
        m_settings = new QSettings(this);

        // Set up the user interface
        setupUserInterface();
        setupMenuBar();
        setupToolBar();
        setupStatusBar();
        setupCentralWidget();
        setupConnections();

        // Load saved settings
        loadSettings();

        // Enable drag and drop
        setAcceptDrops(true);

        qCInfo(gui) << "MainWindow initialization completed successfully";

    } catch (const std::exception &e) {
        qCCritical(gui) << "Error during MainWindow initialization:" << e.what();
        QMessageBox::critical(this, "Initialization Error",
                              QString("Failed to initialize main window: %1").arg(e.what()));
    }
}

MainWindow::~MainWindow() {
    qCInfo(gui) << "MainWindow destructor called";
    saveSettings();
}

void MainWindow::setupUserInterface() {
    // Set window properties
    setWindowTitle("OCR & PPT Automation Tool v1.0");
    setMinimumSize(MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // Set window icon (you can add an icon file later)
    // setWindowIcon(QIcon(":/icons/app_icon.png"));

    qCInfo(gui) << "Basic UI properties configured";
}

void MainWindow::setupMenuBar() {
    // File Menu
    m_fileMenu = menuBar()->addMenu("&File");

    m_openAction = new QAction("&Open Image...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open an image file for OCR processing");
    m_fileMenu->addAction(m_openAction);

    m_fileMenu->addSeparator();

    m_saveAction = new QAction("&Save Results...", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip("Save OCR results to a text file");
    m_saveAction->setEnabled(false);
    m_fileMenu->addAction(m_saveAction);

    m_exportAction = new QAction("&Export to PowerPoint...", this);
    m_exportAction->setShortcut(QKeySequence("Ctrl+E"));
    m_exportAction->setStatusTip("Export results to PowerPoint presentation");
    m_exportAction->setEnabled(false);
    m_fileMenu->addAction(m_exportAction);

    m_fileMenu->addSeparator();

    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit the application");
    m_fileMenu->addAction(m_exitAction);

    // Edit Menu
    m_editMenu = menuBar()->addMenu("&Edit");

    m_clearAction = new QAction("&Clear Results", this);
    m_clearAction->setShortcut(QKeySequence("Ctrl+L"));
    m_clearAction->setStatusTip("Clear all OCR results");
    m_clearAction->setEnabled(false);
    m_editMenu->addAction(m_clearAction);

    // Tools Menu
    m_toolsMenu = menuBar()->addMenu("&Tools");

    m_configureAction = new QAction("&Configure OCR...", this);
    m_configureAction->setStatusTip("Configure OCR engine settings");
    m_toolsMenu->addAction(m_configureAction);

    // Help Menu
    m_helpMenu = menuBar()->addMenu("&Help");

    m_helpAction = new QAction("&Help", this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    m_helpAction->setStatusTip("Show application help");
    m_helpMenu->addAction(m_helpAction);

    m_helpMenu->addSeparator();

    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setStatusTip("Show information about this application");
    m_helpMenu->addAction(m_aboutAction);

    qCInfo(gui) << "Menu bar configured";
}

void MainWindow::setupToolBar() {
    m_mainToolBar = addToolBar("Main");
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Add main actions to toolbar
    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_saveAction);
    m_mainToolBar->addAction(m_exportAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_clearAction);

    qCInfo(gui) << "Toolbar configured";
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage("Ready", 2000);
    qCInfo(gui) << "Status bar configured";
}

void MainWindow::setupCentralWidget() {
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // Create main splitter for resizable layout
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);

    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_mainSplitter);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Set up input section
    m_inputGroup = new QGroupBox("Input", this);
    m_inputLayout = new QVBoxLayout(m_inputGroup);

    m_openFileButton = new QPushButton("Open Image File", this);
    m_configureButton = new QPushButton("Configure OCR", this);
    m_filePathLabel = new QLabel("No file selected", this);
    m_filePathLabel->setWordWrap(true);
    m_filePathLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");

    m_imagePreviewLabel = new QLabel("Image preview will appear here", this);
    m_imagePreviewLabel->setAlignment(Qt::AlignCenter);
    m_imagePreviewLabel->setMinimumHeight(200);
    m_imagePreviewLabel->setStyleSheet(
        "QLabel { border: 2px dashed #ccc; background-color: #f9f9f9; }");
    m_imagePreviewLabel->setScaledContents(true);

    m_inputLayout->addWidget(m_openFileButton);
    m_inputLayout->addWidget(m_configureButton);
    m_inputLayout->addWidget(m_filePathLabel);
    m_inputLayout->addWidget(m_imagePreviewLabel);
    m_inputLayout->addStretch();

    // Set up processing section
    m_processingGroup = new QGroupBox("OCR Processing", this);
    m_processingLayout = new QVBoxLayout(m_processingGroup);

    m_startOCRButton = new QPushButton("Start OCR Analysis", this);
    m_startOCRButton->setEnabled(false);
    m_startOCRButton->setStyleSheet("QPushButton { font-weight: bold; padding: 10px; }");

    m_clearButton = new QPushButton("Clear Results", this);
    m_clearButton->setEnabled(false);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);

    m_statusLabel = new QLabel("Select an image file to begin", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);

    m_processingLayout->addWidget(m_startOCRButton);
    m_processingLayout->addWidget(m_clearButton);
    m_processingLayout->addWidget(m_progressBar);
    m_processingLayout->addWidget(m_statusLabel);
    m_processingLayout->addStretch();

    // Set up results section
    m_resultsGroup = new QGroupBox("OCR Results", this);
    m_resultsLayout = new QVBoxLayout(m_resultsGroup);

    m_resultsTextEdit = new QTextEdit(this);
    m_resultsTextEdit->setPlaceholderText("OCR results will appear here...");
    m_resultsTextEdit->setReadOnly(false);  // Allow editing of results

    m_saveResultsButton = new QPushButton("Save Results", this);
    m_saveResultsButton->setEnabled(false);

    m_exportToPPTButton = new QPushButton("Export to PowerPoint", this);
    m_exportToPPTButton->setEnabled(false);

    QHBoxLayout *resultsButtonLayout = new QHBoxLayout();
    resultsButtonLayout->addWidget(m_saveResultsButton);
    resultsButtonLayout->addWidget(m_exportToPPTButton);
    resultsButtonLayout->addStretch();

    m_resultsLayout->addWidget(m_resultsTextEdit);
    m_resultsLayout->addLayout(resultsButtonLayout);

    // Create left panel (input and processing)
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->addWidget(m_inputGroup);
    leftLayout->addWidget(m_processingGroup);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // Add widgets to splitter
    m_mainSplitter->addWidget(leftPanel);
    m_mainSplitter->addWidget(m_resultsGroup);

    // Set splitter proportions (1:2 ratio)
    m_mainSplitter->setSizes({300, 600});
    m_mainSplitter->setHandleWidth(SPLITTER_HANDLE_WIDTH);

    qCInfo(gui) << "Central widget configured";
}

void MainWindow::setupConnections() {
    // File operations
    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(m_openFileButton, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveResults);
    connect(m_saveResultsButton, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::onExportToPPT);
    connect(m_exportToPPTButton, &QPushButton::clicked, this, &MainWindow::onExportToPPT);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);

    // OCR operations
    connect(m_startOCRButton, &QPushButton::clicked, this, &MainWindow::onStartOCR);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::onClearResults);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearResults);
    connect(m_configureAction, &QAction::triggered, this, &MainWindow::onConfigureOCR);
    connect(m_configureButton, &QPushButton::clicked, this, &MainWindow::onConfigureOCR);

    // Help and information
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::onShowHelp);

    qCInfo(gui) << "Signal-slot connections established";
}

void MainWindow::loadSettings() {
    // Restore window geometry
    restoreGeometry(m_settings->value("geometry").toByteArray());
    restoreState(m_settings->value("windowState").toByteArray());

    // Restore splitter state
    m_mainSplitter->restoreState(m_settings->value("splitterState").toByteArray());

    qCInfo(gui) << "Settings loaded";
}

void MainWindow::saveSettings() {
    // Save window geometry
    m_settings->setValue("geometry", saveGeometry());
    m_settings->setValue("windowState", saveState());

    // Save splitter state
    m_settings->setValue("splitterState", m_mainSplitter->saveState());

    qCInfo(gui) << "Settings saved";
}

// Event handlers
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        // Extract the local path of the first file
        for (const QUrl &url : urlList) {
            if (url.isLocalFile()) {
                QString filePath = url.toLocalFile();
                QFileInfo fileInfo(filePath);

                // Check if it's an image file
                QStringList imageExtensions = {"png", "jpg", "jpeg", "bmp", "gif", "tiff", "pdf"};
                if (imageExtensions.contains(fileInfo.suffix().toLower())) {
                    m_currentFilePath = filePath;
                    m_filePathLabel->setText(filePath);
                    m_filePathLabel->setStyleSheet("QLabel { color: black; font-style: normal; }");

                    // Load and display image preview
                    QPixmap pixmap(filePath);
                    if (!pixmap.isNull()) {
                        m_imagePreviewLabel->setPixmap(pixmap.scaled(m_imagePreviewLabel->size(),
                                                                     Qt::KeepAspectRatio,
                                                                     Qt::SmoothTransformation));
                    }

                    m_startOCRButton->setEnabled(true);
                    m_statusLabel->setText("Image loaded. Ready for OCR processing.");

                    statusBar()->showMessage(QString("Loaded: %1").arg(fileInfo.fileName()), 3000);
                    break;
                }
            }
        }
        event->acceptProposedAction();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_operationInProgress) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Operation in Progress",
            "An OCR operation is currently in progress. Are you sure you want to exit?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            event->ignore();
            return;
        }
    }

    saveSettings();
    event->accept();
}

// Slot implementations (placeholder implementations for now)
void MainWindow::onOpenFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Image File", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Image Files (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;PDF Files (*.pdf);;All Files (*.*)");

    if (!fileName.isEmpty()) {
        m_currentFilePath = fileName;
        m_filePathLabel->setText(fileName);
        m_filePathLabel->setStyleSheet("QLabel { color: black; font-style: normal; }");

        // Load and display image preview
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            m_imagePreviewLabel->setPixmap(pixmap.scaled(
                m_imagePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        m_startOCRButton->setEnabled(true);
        m_statusLabel->setText("Image loaded. Ready for OCR processing.");

        QFileInfo fileInfo(fileName);
        statusBar()->showMessage(QString("Loaded: %1").arg(fileInfo.fileName()), 3000);
    }
}

void MainWindow::onSaveResults() {
    if (m_resultsTextEdit->toPlainText().isEmpty()) {
        QMessageBox::information(this, "No Results", "No OCR results to save.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, "Save OCR Results",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ocr_results.txt",
        "Text Files (*.txt);;All Files (*.*)");

    if (!fileName.isEmpty()) {
        // TODO: Implement file saving
        QMessageBox::information(this, "Save Results",
                                 "Save functionality will be implemented here.");
        statusBar()->showMessage("Results saved", 2000);
    }
}

void MainWindow::onExportToPPT() {
    QMessageBox::information(this, "Export to PowerPoint",
                             "PowerPoint export functionality will be implemented here.");
}

void MainWindow::onExit() { close(); }

void MainWindow::onStartOCR() {
    if (m_currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select an image file first.");
        return;
    }

    // Simulate OCR processing
    m_operationInProgress = true;
    setOperationEnabled(false);
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_statusLabel->setText("Processing OCR...");

    // TODO: Implement actual OCR processing
    // For now, simulate with a timer
    QTimer::singleShot(2000, this, [this]() {
        m_resultsTextEdit->setPlainText(
            "Sample OCR Result:\n\nThis is placeholder text that would be replaced with actual OCR "
            "results from the processed image.\n\nMathematical expressions, text content, and "
            "other recognized elements would appear here.");

        m_progressBar->setValue(100);
        m_statusLabel->setText("OCR processing completed");

        // Enable result-related actions
        m_saveAction->setEnabled(true);
        m_exportAction->setEnabled(true);
        m_saveResultsButton->setEnabled(true);
        m_exportToPPTButton->setEnabled(true);
        m_clearAction->setEnabled(true);
        m_clearButton->setEnabled(true);

        setOperationEnabled(true);
        m_operationInProgress = false;

        statusBar()->showMessage("OCR processing completed successfully", 3000);

        QTimer::singleShot(2000, this, [this]() { m_progressBar->setVisible(false); });
    });

    statusBar()->showMessage("Starting OCR processing...", 2000);
}

void MainWindow::onClearResults() {
    m_resultsTextEdit->clear();
    m_progressBar->setVisible(false);
    m_progressBar->setValue(0);
    m_statusLabel->setText("Results cleared");

    // Disable result-related actions
    m_saveAction->setEnabled(false);
    m_exportAction->setEnabled(false);
    m_saveResultsButton->setEnabled(false);
    m_exportToPPTButton->setEnabled(false);
    m_clearAction->setEnabled(false);
    m_clearButton->setEnabled(false);

    statusBar()->showMessage("Results cleared", 2000);
}

void MainWindow::onConfigureOCR() {
    QMessageBox::information(this, "Configure OCR",
                             "OCR configuration dialog will be implemented here.");
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this, "About OCR & PPT Automation Tool",
        "<h3>OCR & PPT Automation Tool v1.0</h3>"
        "<p>A professional desktop application for OCR processing and PowerPoint automation.</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Advanced OCR text recognition</li>"
        "<li>Mathematical expression processing</li>"
        "<li>PowerPoint automation</li>"
        "<li>Cross-platform compatibility</li>"
        "</ul>"
        "<p><b>Built with:</b> Qt 6 Framework</p>"
        "<p><b>Platform:</b> Windows & Linux</p>");
}

void MainWindow::onShowHelp() {
    QDesktopServices::openUrl(QUrl("https://github.com/your-repo/mathscan/wiki"));
}

void MainWindow::onCheckUpdates() {
    QMessageBox::information(this, "Check Updates",
                             "Update checking functionality will be implemented here.");
}

void MainWindow::onProgressUpdate(int value) { m_progressBar->setValue(value); }

void MainWindow::onOperationCompleted() {
    m_operationInProgress = false;
    setOperationEnabled(true);
    m_progressBar->setVisible(false);
}

void MainWindow::onStatusUpdate(const QString &message) {
    m_statusLabel->setText(message);
    statusBar()->showMessage(message, 2000);
}

void MainWindow::setOperationEnabled(bool enabled) {
    m_openFileButton->setEnabled(enabled);
    m_startOCRButton->setEnabled(enabled && !m_currentFilePath.isEmpty());
    m_configureButton->setEnabled(enabled);
    m_openAction->setEnabled(enabled);
    m_configureAction->setEnabled(enabled);
}

void MainWindow::updateProgress(int percentage, const QString &message) {
    m_progressBar->setValue(percentage);
    if (!message.isEmpty()) {
        m_statusLabel->setText(message);
    }
}

void MainWindow::logMessage(const QString &message, bool isError) {
    if (isError) {
        qCWarning(gui) << "UI Error:" << message;
    } else {
        qCInfo(gui) << "UI Info:" << message;
    }
}

/*
 * Project: OCR & PPT Automation Tool - Initial Setup
 *
 * Objective:
 * - Create a basic Qt C++ desktop application with a main window.
 * - Use object-oriented programming best practices.
 * - Set up a clean project structure for easy extension.
 * - Include robust error handling during startup.
 * - Add comments suitable for experienced developers.
 * - Ensure cross-platform compatibility (Windows and Linux).
 *
 * Requirements:
 * - Use Qt 6 framework.
 * - Create a MainWindow class inheriting from QMainWindow.
 * - Initialize and display the main window in main().
 * - Implement basic logging of application startup status.
 */

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStyleFactory>
#include <iostream>

#include "../include/mainwindow.h"

// Logging categories for structured debugging
Q_LOGGING_CATEGORY(startup, "app.startup")
Q_LOGGING_CATEGORY(gui, "app.gui")
Q_LOGGING_CATEGORY(error, "app.error")

/**
 * @brief Configure application-wide settings and logging
 * @param app Reference to QApplication instance
 */
void configureApplication(QApplication &app) {
    // Set application metadata
    app.setApplicationName("OCR & PPT Automation Tool");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("MathScan Development");
    app.setOrganizationDomain("mathscan.dev");

    qCInfo(startup) << "Application metadata configured";
    qCInfo(startup) << "Application name:" << app.applicationName();
    qCInfo(startup) << "Version:" << app.applicationVersion();

    // Configure application style for cross-platform consistency
    QStringList availableStyles = QStyleFactory::keys();
    qCInfo(startup) << "Available GUI styles:" << availableStyles;

    // Use Fusion style for consistent cross-platform appearance
    if (availableStyles.contains("Fusion")) {
        app.setStyle("Fusion");
        qCInfo(startup) << "Using Fusion style for cross-platform consistency";
    }
}

/**
 * @brief Verify application directories and permissions
 * @return true if all directories are accessible, false otherwise
 */
bool verifyApplicationEnvironment() {
    qCInfo(startup) << "Verifying application environment...";

    // Check writable locations for application data
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    qCInfo(startup) << "App data path:" << appDataPath;
    qCInfo(startup) << "Temp path:" << tempPath;
    qCInfo(startup) << "Documents path:" << documentsPath;

    // Create application data directory if it doesn't exist
    QDir appDataDir(appDataPath);
    if (!appDataDir.exists()) {
        if (appDataDir.mkpath(".")) {
            qCInfo(startup) << "Created application data directory:" << appDataPath;
        } else {
            qCCritical(error) << "Failed to create application data directory:" << appDataPath;
            return false;
        }
    }

    // Verify write permissions
    QDir tempDir(tempPath);
    if (!tempDir.exists() || !QFileInfo(tempPath).isWritable()) {
        qCCritical(error) << "Temp directory not writable:" << tempPath;
        return false;
    }

    qCInfo(startup) << "Application environment verification completed successfully";
    return true;
}

/**
 * @brief Main application entry point
 * @param argc Command line argument count
 * @param argv Command line arguments
 * @return Application exit code
 */
int main(int argc, char *argv[]) {
    // Create QApplication instance with error handling
    QApplication app(argc, argv);

    try {
        qCInfo(startup) << "=== OCR & PPT Automation Tool Starting ===";
        qCInfo(startup) << "Qt version:" << QT_VERSION_STR;
        qCInfo(startup) << "Platform:" << QGuiApplication::platformName();
        qCInfo(startup) << "Arguments:" << app.arguments();

        // Configure application settings
        configureApplication(app);

        // Verify environment and permissions
        if (!verifyApplicationEnvironment()) {
            qCCritical(error) << "Environment verification failed";
            QMessageBox::critical(nullptr, "Startup Error",
                                  "Failed to verify application environment.\n"
                                  "Please check permissions and try again.");
            return 1;
        }

        // Initialize and display main window
        qCInfo(startup) << "Initializing main window...";
        MainWindow mainWindow;

        // Connect to application-wide signals for cleanup
        QObject::connect(&app, &QApplication::aboutToQuit,
                         [&]() { qCInfo(startup) << "Application shutting down..."; });

        // Show main window
        mainWindow.show();
        qCInfo(startup) << "Main window displayed successfully";
        qCInfo(startup) << "=== Application startup completed ===";

        // Start event loop
        int exitCode = app.exec();
        qCInfo(startup) << "Application exited with code:" << exitCode;
        return exitCode;

    } catch (const std::exception &e) {
        qCCritical(error) << "Unhandled exception during startup:" << e.what();
        std::cerr << "Fatal error: " << e.what() << std::endl;

        QMessageBox::critical(
            nullptr, "Fatal Error",
            QString("An unexpected error occurred during startup:\n%1").arg(e.what()));
        return 2;

    } catch (...) {
        qCCritical(error) << "Unknown exception during startup";
        std::cerr << "Fatal error: Unknown exception" << std::endl;

        QMessageBox::critical(nullptr, "Fatal Error", "An unknown error occurred during startup.");
        return 3;
    }
}

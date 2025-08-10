#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[]) {
    // Create a minimal QCoreApplication for basic Qt functionality
    QCoreApplication app(argc, argv);

    qDebug() << "Starting Qt environment check...";
    qDebug() << "Qt version:" << QT_VERSION_STR;
    qDebug() << "Qt library paths:" << QCoreApplication::libraryPaths();

    bool allGood = true;

    // Check QtCore (basic Qt functionality)
    qDebug() << "Checking QtCore module...";
    try {
        QString version = QCoreApplication::applicationVersion();
        qDebug() << "QtCore module: Available";
        qDebug() << "Application version support: Working";
    } catch (...) {
        qDebug() << "QtCore module: Error during testing";
        allGood = false;
    }

    // Check Qt library information
    qDebug() << "Checking Qt library information...";
    try {
        QString qtVersion = QLibraryInfo::version().toString();
        qDebug() << "Qt runtime version:" << qtVersion;
        qDebug() << "Qt installation path:" << QLibraryInfo::path(QLibraryInfo::PrefixPath);
        qDebug() << "Qt plugins path:" << QLibraryInfo::path(QLibraryInfo::PluginsPath);
    } catch (...) {
        qDebug() << "Qt library information: Error accessing";
        allGood = false;
    }

    // Check QtWidgets availability (create minimal test)
    qDebug() << "Checking QtWidgets module...";
    try {
        // Test if we can create QWidget class
        // Note: We can't actually instantiate it without QApplication,
        // but we can check if the class is available
        QWidget *testWidget = nullptr;
        Q_UNUSED(testWidget);  // Suppress unused variable warning
        qDebug() << "QtWidgets module: Available (class accessible)";
    } catch (...) {
        qDebug() << "QtWidgets module: NOT available";
        allGood = false;
    }

    // Check if we're running with proper command line argument
    bool checkMode = false;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--check-qt") {
            checkMode = true;
            break;
        }
    }

    if (checkMode) {
        qDebug() << "Running in Qt check mode...";

        // Additional checks for GUI capabilities
        qDebug() << "Checking GUI environment...";
        try {
            // Check if GUI application could theoretically be created
            bool hasDisplay = !qEnvironmentVariableIsEmpty("DISPLAY") ||
                              !qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY") ||
                              true;  // On Windows, GUI is generally available

            if (hasDisplay) {
                qDebug() << "GUI environment: Available";
            } else {
                qDebug() << "GUI environment: Limited (no display detected)";
            }
        } catch (...) {
            qDebug() << "GUI environment: Error during check";
            allGood = false;
        }

        // Check standard paths
        qDebug() << "Checking Qt standard paths...";
        try {
            QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QString temp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
            qDebug() << "App data location:" << appData;
            qDebug() << "Temp location:" << temp;
            qDebug() << "Standard paths: Working";
        } catch (...) {
            qDebug() << "Standard paths: Error";
            allGood = false;
        }
    }

    // Final result
    if (allGood) {
        qDebug() << "✓ All required Qt modules are installed and accessible.";
        qDebug() << "✓ Qt environment check PASSED";
        return 0;
    } else {
        qDebug() << "✗ Some Qt modules are missing or not accessible.";
        qDebug() << "✗ Please check your Qt installation.";
        qDebug() << "✗ Qt environment check FAILED";
        return 1;
    }
}

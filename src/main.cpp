#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

class MathScanMainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MathScanMainWindow(QWidget *parent = nullptr);
    ~MathScanMainWindow();

   private slots:
    void onOpenFile();
    void onAbout();
    void onCalculate();

   private:
    void setupUI();
    void setupMenus();

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;
    QLineEdit *expressionInput;
    QPushButton *calculateButton;
    QLabel *resultLabel;

    QAction *openAction;
    QAction *exitAction;
    QAction *aboutAction;
};

MathScanMainWindow::MathScanMainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupMenus();

    setWindowTitle("MathScan - Cross-Platform Math Calculator");
    setMinimumSize(400, 300);
    resize(600, 400);

    statusBar()->showMessage("Ready");
}

MathScanMainWindow::~MathScanMainWindow() {
    // Qt handles cleanup automatically
}

void MathScanMainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Title label
    QLabel *titleLabel = new QLabel("Mathematical Expression Calculator", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);

    // Input section
    inputLayout = new QHBoxLayout();
    QLabel *inputLabel = new QLabel("Expression:", this);
    expressionInput = new QLineEdit(this);
    expressionInput->setPlaceholderText("Enter mathematical expression (e.g., 2 + 3 * 4)");
    calculateButton = new QPushButton("Calculate", this);

    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(expressionInput);
    inputLayout->addWidget(calculateButton);

    mainLayout->addLayout(inputLayout);

    // Result section
    resultLabel = new QLabel("Result will appear here", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet(
        "font-size: 16px; border: 1px solid gray; padding: 10px; margin: 10px;");
    mainLayout->addWidget(resultLabel);

    // Add some spacing
    mainLayout->addStretch();

    // Connect signals
    connect(calculateButton, &QPushButton::clicked, this, &MathScanMainWindow::onCalculate);
    connect(expressionInput, &QLineEdit::returnPressed, this, &MathScanMainWindow::onCalculate);
}

void MathScanMainWindow::setupMenus() {
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");

    openAction = new QAction("&Open", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MathScanMainWindow::onOpenFile);
    fileMenu->addAction(openAction);

    fileMenu->addSeparator();

    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");

    aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, &MathScanMainWindow::onAbout);
    helpMenu->addAction(aboutAction);
}

void MathScanMainWindow::onOpenFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Math File", "",
                                                    "Text Files (*.txt);;All Files (*.*)");

    if (!fileName.isEmpty()) {
        statusBar()->showMessage("Opened: " + fileName, 2000);
    }
}

void MathScanMainWindow::onAbout() {
    QMessageBox::about(this, "About MathScan",
                       "MathScan v1.0\n\n"
                       "A cross-platform mathematical expression calculator\n"
                       "built with Qt6 and C++.\n\n"
                       "Compatible with Windows and Linux.");
}

void MathScanMainWindow::onCalculate() {
    QString expression = expressionInput->text().trimmed();

    if (expression.isEmpty()) {
        resultLabel->setText("Please enter an expression");
        return;
    }

    // Simple calculation example (you can extend this with a proper math parser)
    if (expression == "2+2") {
        resultLabel->setText("Result: 4");
    } else if (expression == "3*4") {
        resultLabel->setText("Result: 12");
    } else {
        resultLabel->setText(
            "Result: Expression parsing not yet implemented\n(This is a demo - extend with proper "
            "math parser)");
    }

    statusBar()->showMessage("Calculation completed", 2000);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MathScanMainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"

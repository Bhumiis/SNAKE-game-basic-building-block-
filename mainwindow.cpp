#include "mainwindow.h"
#include "gameboard.h"
#include <QMessageBox>
#include <QFont>  // For font styling

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Central widget and layout setup (original way: layout on widget)
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);  // Implicitly sets layout
    setCentralWidget(centralWidget);

    // Game board
    gameBoard = new GameBoard(this);
    mainLayout->addWidget(gameBoard, 1);  // Stretch to fill

    // Controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    startButton = new QPushButton("Start", this);
    pauseButton = new QPushButton("Pause", this);
    restartButton = new QPushButton("Restart", this);
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(restartButton);
    mainLayout->addLayout(controlsLayout);

    // Score label (add after controls)
    scoreLabel = new QLabel("Score: 0", this);
    mainLayout->addWidget(scoreLabel);

    // NEW: Add title label at the top (insert at position 0)
    QLabel *titleLabel = new QLabel("Snake Game", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->insertWidget(0, titleLabel);  // Insert at beginning of layout

    // Style the score label: Larger, colored, centered
    scoreLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: darkGreen; padding: 5px; }");
    scoreLabel->setAlignment(Qt::AlignCenter);

    // Style buttons: Green theme with hover/pressed effects
    QString buttonStyle = R"(
        QPushButton {
            background-color: #4CAF50;
            border: 2px solid #45a049;
            color: white;
            padding: 8px 16px;
            font-size: 12px;
            border-radius: 4px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #45a049;
            border-color: #3d8b40;
        }
        QPushButton:pressed {
            background-color: #3d8b40;
            border-color: #2e7d32;
        }
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
    )";
    startButton->setStyleSheet(buttonStyle);
    pauseButton->setStyleSheet(buttonStyle);
    restartButton->setStyleSheet(buttonStyle);

    // Optional: Style the whole window for consistency
    setStyleSheet("QMainWindow { background-color: #f0f0f0; }");  // Light background

    // Connections (unchanged)
    connect(startButton, &QPushButton::clicked, gameBoard, &GameBoard::startGame);
    connect(pauseButton, &QPushButton::clicked, gameBoard, &GameBoard::pauseGame);
    connect(restartButton, &QPushButton::clicked, gameBoard, &GameBoard::restartGame);
    connect(gameBoard, &GameBoard::scoreChanged, this, [=](int score) {
        scoreLabel->setText(QString("Score: %1").arg(score));
    });

    setWindowTitle("Snake Game - Enhanced");
    resize(450, 550);  // Adjusted for title
}

MainWindow::~MainWindow() {}

#include "gameboard.h"
#include <QDebug>

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent), direction(1), timerId(0) {  // Start facing right
    setFixedSize(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    setFocusPolicy(Qt::StrongFocus);  // Enable key events
    initGame();
}

void GameBoard::initGame() {
    snake.clear();
    snake.append(QPoint(BOARD_SIZE / 2, BOARD_SIZE / 2));  // Start in center
    direction = 1;  // Right
    score = 0;
    gameOver = false;
    isPaused = false;
    generateFood();
    update();  // Repaint
    emit scoreChanged(score);
}

void GameBoard::startGame() {
    if (timerId == 0) {
        timerId = startTimer(200);  // Initial speed
    }
    isPaused = false;
}

void GameBoard::pauseGame() {
    isPaused = !isPaused;
}

void GameBoard::restartGame() {
    killTimer(timerId);
    timerId = 0;
    initGame();
}

void GameBoard::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Smooth edges

    // Background: Light gray for contrast
    painter.fillRect(rect(), Qt::lightGray);

    // Border around the entire boardt
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // Optional grid lines (faint)
    painter.setPen(QPen(Qt::gray, 1));
    for (int x = 0; x <= width(); x += CELL_SIZE) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y <= height(); y += CELL_SIZE) {
        painter.drawLine(0, y, width(), y);
    }

    if (!gameOver) {
        // Draw snake: Head (dark green), body (light green)
        QPen snakePen(Qt::black, 1);
        painter.setPen(snakePen);

        // Body segments (except head)
        painter.setBrush(QBrush(QColor(0, 255, 0, 180)));  // Semi-transparent green
        for (int i = 1; i < snake.size(); ++i) {  // Skip head
            const QPoint &segment = snake[i];
            QRect cellRect(segment.x() * CELL_SIZE + 1, segment.y() * CELL_SIZE + 1,
                           CELL_SIZE - 2, CELL_SIZE - 2);  // Inset for rounding
            painter.drawRoundedRect(cellRect, 4, 4);  // Rounded corners
        }

        // Head (darker, solid)
        if (!snake.isEmpty()) {
            const QPoint &head = snake.first();
            painter.setBrush(Qt::darkGreen);
            QRect headRect(head.x() * CELL_SIZE + 1, head.y() * CELL_SIZE + 1,
                           CELL_SIZE - 2, CELL_SIZE - 2);
            painter.drawRoundedRect(headRect, 4, 4);
        }

        // Draw food: Red circle with inner glow
        painter.setPen(QPen(Qt::darkRed, 2));
        painter.setBrush(QBrush(QColor(255, 100, 100)));  // Light red fill
        painter.drawEllipse(food.x() * CELL_SIZE + 2, food.y() * CELL_SIZE + 2,
                            CELL_SIZE - 4, CELL_SIZE - 4);  // Circle inset
    } else {
        // Game Over Overlay: Semi-transparent background
        painter.setBrush(QBrush(QColor(0, 0, 0, 128)));  // Dark semi-transparent
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());

        // Text: Centered, bold
        QFont font = painter.font();
        font.setPointSize(24);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Game Over!");

        // Score recap
        font.setPointSize(16);
        painter.setFont(font);
        QString scoreText = QString("Final Score: %1").arg(score);
        painter.drawText(rect(), Qt::AlignCenter | Qt::AlignBottom, scoreText);

        // Hint
        font.setPointSize(12);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignTop , "Press Restart to Play Again");
    }
}


void GameBoard::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        if (direction != 2) direction = 0;  // Not opposite (down)
        break;
    case Qt::Key_Right:
        if (direction != 3) direction = 1;
        break;
    case Qt::Key_Down:
        if (direction != 0) direction = 2;
        break;
    case Qt::Key_Left:
        if (direction != 1) direction = 3;
        break;
    default:
        break;
    }
}

void GameBoard::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId && !isPaused && !gameOver) {
        moveSnake();
    }
}

void GameBoard::moveSnake() {
    QPoint head = snake.first();
    QPoint newHead;

    switch (direction) {
    case 0: newHead = QPoint(head.x(), head.y() - 1); break;  // Up
    case 1: newHead = QPoint(head.x() + 1, head.y()); break;  // Right
    case 2: newHead = QPoint(head.x(), head.y() + 1); break;  // Down
    case 3: newHead = QPoint(head.x() - 1, head.y()); break;  // Left
    }

    // Check bounds/wall collision
    if (newHead.x() < 0 || newHead.x() >= BOARD_SIZE ||
        newHead.y() < 0 || newHead.y() >= BOARD_SIZE) {
        gameOver = true;
        update();
        return;
    }

    snake.prepend(newHead);  // Add new head

    // Check self collision
    if (snake.count(newHead) > 1) {
        gameOver = true;
        update();
        return;
    }

    // Check food
    if (newHead == food) {
        score += 10;
        emit scoreChanged(score);
        generateFood();
        growSnake();  // Don't remove tail
        // Increase speed every 5 foods
        if (score % 50 == 0) {
            killTimer(timerId);
            timerId = startTimer(qMax(50, 200 - (score / 5)));  // Faster
        }
    } else {
        snake.removeLast();  // Normal move: remove tail
    }

    update();
}

void GameBoard::generateFood() {
    do {
        food = QPoint(QRandomGenerator::global()->bounded(BOARD_SIZE),
                      QRandomGenerator::global()->bounded(BOARD_SIZE));
    } while (snake.contains(food));  // Avoid snake body
}

void GameBoard::growSnake() {
    // Snake already grew by not removing tail in moveSnake()
}

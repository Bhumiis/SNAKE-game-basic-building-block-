#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QList>
#include <QPoint>
#include <QPainter>
#include <QRandomGenerator>

class GameBoard : public QWidget {
    Q_OBJECT

public:
    GameBoard(QWidget *parent = nullptr);
    void startGame();
    void pauseGame();
    void restartGame();
    int getScore() const { return score; }

signals:
    void scoreChanged(int score);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    void initGame();
    void generateFood();
    bool checkCollision(const QPoint &head);
    void moveSnake();
    void growSnake();

    static const int BOARD_SIZE = 20;  // 20x20 grid
    static const int CELL_SIZE = 20;   // Pixels per cell
    QList<QPoint> snake;
    QPoint food;
    int direction;  // 0: up, 1: right, 2: down, 3: left
    int timerId;
    int score = 0;
    bool isPaused = false;
    bool gameOver = false;
};

#endif // GAMEBOARD_H

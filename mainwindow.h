#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
class GameBoard;  // Forward declaration
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GameBoard *gameBoard;
    QLabel *scoreLabel;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *restartButton;
};

#endif // MAINWINDOW_H

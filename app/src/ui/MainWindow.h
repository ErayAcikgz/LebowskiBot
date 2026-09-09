#ifndef LEBOWSKI_MAINWINDOW_H
#define LEBOWSKI_MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *pageStack_ = nullptr;
};

#endif // LEBOWSKI_MAINWINDOW_H

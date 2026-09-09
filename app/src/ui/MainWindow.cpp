#include "MainWindow.h"

#include "pages/OverviewPage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Lebowski"));
    resize(1280, 800);
    setCentralWidget(new OverviewPage(this));
}

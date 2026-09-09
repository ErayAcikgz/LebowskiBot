#include "MainWindow.h"

#include "BottomStatusBar.h"
#include "NavigationPanel.h"
#include "TopBar.h"
#include "pages/AIPage.h"
#include "pages/ControlPage.h"
#include "pages/MonitorPage.h"
#include "pages/OverviewPage.h"
#include "pages/ProgramPage.h"
#include "pages/SettingsPage.h"
#include "pages/TeachPage.h"

#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QIODevice>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Lebowski | Robotic Arm Control"));
    resize(1280, 800);

    auto *shell = new QWidget(this);
    auto *shellLayout = new QVBoxLayout(shell);
    shellLayout->setContentsMargins(0, 0, 0, 0);
    shellLayout->setSpacing(0);

    shellLayout->addWidget(new TopBar(shell));

    auto *mainRow = new QWidget(shell);
    auto *mainLayout = new QHBoxLayout(mainRow);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *navigation = new NavigationPanel(mainRow);
    mainLayout->addWidget(navigation);

    pageStack_ = new QStackedWidget(mainRow);
    pageStack_->setObjectName(QStringLiteral("pageStack"));
    pageStack_->addWidget(new OverviewPage(pageStack_));
    pageStack_->addWidget(new ControlPage(pageStack_));
    pageStack_->addWidget(new ProgramPage(pageStack_));
    pageStack_->addWidget(new TeachPage(pageStack_));
    pageStack_->addWidget(new AIPage(pageStack_));
    pageStack_->addWidget(new MonitorPage(pageStack_));
    pageStack_->addWidget(new SettingsPage(pageStack_));
    mainLayout->addWidget(pageStack_, 1);
    shellLayout->addWidget(mainRow, 1);

    shellLayout->addWidget(new BottomStatusBar(shell));
    setCentralWidget(shell);

    connect(navigation, &NavigationPanel::pageSelected,
            pageStack_, &QStackedWidget::setCurrentIndex);

    QFile styleSheetFile(QStringLiteral(":/styles/app.qss"));
    if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(styleSheetFile.readAll()));
    }
}

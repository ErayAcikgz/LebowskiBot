#include "TopBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>

TopBar::TopBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("topBar"));
    setFixedHeight(72);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(28, 0, 28, 0);
    layout->setSpacing(12);

    auto *applicationName = new QLabel(QStringLiteral("LEBOWSKI"), this);
    applicationName->setObjectName(QStringLiteral("applicationName"));
    layout->addWidget(applicationName);
    layout->addStretch();

    auto *connectionIndicator = new QLabel(this);
    connectionIndicator->setObjectName(QStringLiteral("connectionIndicator"));
    connectionIndicator->setFixedSize(9, 9);
    layout->addWidget(connectionIndicator);

    auto *connectionStatus = new QLabel(QStringLiteral("DISCONNECTED"), this);
    connectionStatus->setObjectName(QStringLiteral("connectionStatus"));
    layout->addWidget(connectionStatus);

    auto *estop = new QLabel(QStringLiteral("E-STOP"), this);
    estop->setObjectName(QStringLiteral("estopStatus"));
    estop->setAlignment(Qt::AlignCenter);
    estop->setMinimumWidth(88);
    layout->addWidget(estop);
}

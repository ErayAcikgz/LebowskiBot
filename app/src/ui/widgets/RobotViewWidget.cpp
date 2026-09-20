#include "RobotViewWidget.h"

#include <QLabel>
#include <QVBoxLayout>

RobotViewWidget::RobotViewWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("robotView"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("3D Visualization"), this);
    title->setObjectName(QStringLiteral("robotViewTitle"));
    title->setAlignment(Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(title);

    auto *availability = new QLabel(QStringLiteral("Not available"), this);
    availability->setObjectName(QStringLiteral("robotViewStatus"));
    availability->setAlignment(Qt::AlignCenter);
    layout->addWidget(availability);
    layout->addStretch();
}

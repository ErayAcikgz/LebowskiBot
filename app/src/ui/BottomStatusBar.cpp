#include "BottomStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>

BottomStatusBar::BottomStatusBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("bottomStatusBar"));
    setFixedHeight(48);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(28, 0, 28, 0);
    layout->setSpacing(20);

    auto *robot = new QLabel(QStringLiteral("Robot: Disconnected"), this);
    robot->setObjectName(QStringLiteral("robotStatus"));
    layout->addWidget(robot);
    layout->addSpacing(12);

    const QStringList joints = {QStringLiteral("J1  --"), QStringLiteral("J2  --"),
                                QStringLiteral("J3  --"), QStringLiteral("J4  --"),
                                QStringLiteral("J5  --"), QStringLiteral("J6  --")};
    for (const QString &joint : joints) {
        auto *label = new QLabel(joint, this);
        label->setObjectName(QStringLiteral("jointStatus"));
        layout->addWidget(label);
    }

    layout->addStretch();
    layout->addWidget(new QLabel(QStringLiteral("Battery: --"), this));
    layout->addWidget(new QLabel(QStringLiteral("Temperature: --"), this));
}

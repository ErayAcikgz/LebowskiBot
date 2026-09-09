#include "JointStatusWidget.h"

#include <QGridLayout>
#include <QLabel>

JointStatusWidget::JointStatusWidget(int jointCount, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("jointStatusWidget"));

    auto *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(30);
    layout->setVerticalSpacing(14);

    for (int index = 0; index < jointCount; ++index) {
        const int row = index % 3;
        const int column = index / 3;

        auto *jointLabel = new QLabel(QStringLiteral("J%1").arg(index + 1), this);
        jointLabel->setObjectName(QStringLiteral("jointLabel"));
        layout->addWidget(jointLabel, row, column * 2);

        auto *positionLabel = new QLabel(QStringLiteral("--°"), this);
        positionLabel->setObjectName(QStringLiteral("jointValue"));
        positionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        layout->addWidget(positionLabel, row, column * 2 + 1);
    }

    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(3, 1);
}

#include "JointControlWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

JointControlWidget::JointControlWidget(int jointCount, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("jointControlWidget"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    auto *stepRow = new QWidget(this);
    auto *stepLayout = new QHBoxLayout(stepRow);
    stepLayout->setContentsMargins(0, 0, 0, 0);
    stepLayout->setSpacing(8);

    auto *stepLabel = new QLabel(QStringLiteral("Step"), stepRow);
    stepLabel->setObjectName(QStringLiteral("valueLabel"));
    stepLayout->addWidget(stepLabel);
    stepLayout->addStretch();

    auto *stepSelector = new QComboBox(stepRow);
    stepSelector->setObjectName(QStringLiteral("stepSelector"));
    stepSelector->addItem(QStringLiteral("0.1°"), 0.1);
    stepSelector->addItem(QStringLiteral("0.5°"), 0.5);
    stepSelector->addItem(QStringLiteral("1.0°"), 1.0);
    stepSelector->addItem(QStringLiteral("5.0°"), 5.0);
    stepSelector->addItem(QStringLiteral("10.0°"), 10.0);
    stepSelector->setCurrentText(QStringLiteral("1.0°"));
    stepSelector->setMinimumWidth(78);
    stepLayout->addWidget(stepSelector);
    layout->addWidget(stepRow);

    for (int index = 0; index < jointCount; ++index) {
        auto *row = new QWidget(this);
        auto *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(10);

        auto *jointLabel = new QLabel(QStringLiteral("J%1").arg(index + 1), row);
        jointLabel->setObjectName(QStringLiteral("jointLabel"));
        jointLabel->setMinimumWidth(24);
        rowLayout->addWidget(jointLabel);

        auto *angleInput = new QDoubleSpinBox(row);
        angleInput->setObjectName(QStringLiteral("jointInput"));
        // Temporary generic bounds; replace with configured joint limits later.
        angleInput->setRange(-360.0, 360.0);
        angleInput->setDecimals(1);
        angleInput->setSingleStep(0.1);
        angleInput->setSuffix(QStringLiteral("°"));
        angleInput->setValue(0.0);
        angleInput->setMinimumWidth(82);
        angleInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
        rowLayout->addWidget(angleInput);

        rowLayout->addStretch();

        auto *decreaseButton = new QPushButton(QStringLiteral("-"), row);
        decreaseButton->setObjectName(QStringLiteral("jogButton"));
        decreaseButton->setAccessibleName(QStringLiteral("Decrease J%1").arg(index + 1));
        decreaseButton->setFixedSize(28, 26);
        rowLayout->addWidget(decreaseButton);

        auto *increaseButton = new QPushButton(QStringLiteral("+"), row);
        increaseButton->setObjectName(QStringLiteral("jogButton"));
        increaseButton->setAccessibleName(QStringLiteral("Increase J%1").arg(index + 1));
        increaseButton->setFixedSize(28, 26);
        rowLayout->addWidget(increaseButton);
        layout->addWidget(row);

        connect(decreaseButton, &QPushButton::clicked, this,
                [angleInput, stepSelector] {
                    angleInput->setValue(angleInput->value() - stepSelector->currentData().toDouble());
                });
        connect(increaseButton, &QPushButton::clicked, this,
                [angleInput, stepSelector] {
                    angleInput->setValue(angleInput->value() + stepSelector->currentData().toDouble());
                });
    }
}

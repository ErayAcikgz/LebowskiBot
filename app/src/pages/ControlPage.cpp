#include "ControlPage.h"

#include "ui/widgets/JointControlWidget.h"
#include "ui/widgets/Panel.h"
#include "ui/widgets/RobotViewWidget.h"
#include "ui/widgets/ValueRow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QSlider>
#include <QVBoxLayout>

ControlPage::ControlPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 4, 48, 4);
    layout->setSpacing(4);

    auto *titleLabel = new QLabel(QStringLiteral("Control"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(
        QStringLiteral("Manually control and monitor robot motion."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(4);

    auto *mainGrid = new QGridLayout;
    mainGrid->setObjectName(QStringLiteral("controlGrid"));
    mainGrid->setHorizontalSpacing(18);
    mainGrid->setVerticalSpacing(0);
    mainGrid->setColumnStretch(0, 3);
    mainGrid->setColumnStretch(1, 7);
    mainGrid->setRowStretch(0, 1);
    layout->addLayout(mainGrid, 1);

    auto *leftColumn = new QWidget(this);
    leftColumn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    auto *leftLayout = new QVBoxLayout(leftColumn);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(4);

    auto *jointPanel = new Panel(QStringLiteral("JOINT CONTROL"), this);
    jointPanel->setCompact(true);
    jointPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    jointPanel->contentLayout()->addWidget(new JointControlWidget(6, jointPanel));
    leftLayout->addWidget(jointPanel);
    leftLayout->addStretch(1);

    auto *robotViewPanel = new Panel(QStringLiteral("ROBOT VIEW"), this);
    robotViewPanel->contentLayout()->addWidget(new RobotViewWidget(robotViewPanel));

    auto *motionPanel = new Panel(QStringLiteral("MOTION"), this);
    motionPanel->setCompact(true);
    motionPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto *speedRow = new QWidget(motionPanel);
    auto *speedLayout = new QHBoxLayout(speedRow);
    speedLayout->setContentsMargins(0, 0, 0, 0);
    speedLayout->setSpacing(14);

    auto *speedLabel = new QLabel(QStringLiteral("Speed"), speedRow);
    speedLabel->setObjectName(QStringLiteral("valueLabel"));
    speedLayout->addWidget(speedLabel);

    auto *speedSlider = new QSlider(Qt::Horizontal, speedRow);
    speedSlider->setObjectName(QStringLiteral("controlSlider"));
    speedSlider->setRange(0, 100);
    speedSlider->setValue(50);
    speedSlider->setMinimumWidth(120);
    speedLayout->addWidget(speedSlider, 1);

    auto *speedValue = new QLabel(QStringLiteral("50 %"), speedRow);
    speedValue->setObjectName(QStringLiteral("controlValue"));
    speedValue->setMinimumWidth(42);
    speedValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    speedLayout->addWidget(speedValue);
    motionPanel->contentLayout()->addWidget(speedRow);

    auto *buttonRow = new QHBoxLayout;
    buttonRow->setContentsMargins(0, 6, 0, 0);
    buttonRow->setSpacing(12);
    auto *homeButton = new QPushButton(QStringLiteral("HOME"), motionPanel);
    homeButton->setObjectName(QStringLiteral("secondaryButton"));
    homeButton->setMinimumHeight(30);
    buttonRow->addWidget(homeButton);

    auto *stopButton = new QPushButton(QStringLiteral("STOP"), motionPanel);
    stopButton->setObjectName(QStringLiteral("stopButton"));
    stopButton->setMinimumHeight(30);
    buttonRow->addWidget(stopButton);
    buttonRow->addStretch();
    motionPanel->contentLayout()->addLayout(buttonRow);
    leftLayout->addWidget(motionPanel);
    leftLayout->addStretch(1);

    auto *positionPanel = new Panel(QStringLiteral("ROBOT POSITION"), this);
    positionPanel->setCompact(true);
    positionPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto *positionGrid = new QGridLayout;
    positionGrid->setContentsMargins(0, 0, 0, 0);
    positionGrid->setHorizontalSpacing(18);
    positionGrid->setVerticalSpacing(2);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("X"), QStringLiteral("-- mm"), positionPanel), 0, 0);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("Rx"), QStringLiteral("-- °"), positionPanel), 0, 1);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("Y"), QStringLiteral("-- mm"), positionPanel), 1, 0);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("Ry"), QStringLiteral("-- °"), positionPanel), 1, 1);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("Z"), QStringLiteral("-- mm"), positionPanel), 2, 0);
    positionGrid->addWidget(
        new ValueRow(QStringLiteral("Rz"), QStringLiteral("-- °"), positionPanel), 2, 1);
    positionPanel->contentLayout()->addLayout(positionGrid);
    leftLayout->addWidget(positionPanel);
    leftLayout->addStretch(1);

    auto *statePanel = new Panel(QStringLiteral("ROBOT STATE"), this);
    statePanel->setCompact(true);
    statePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto *stateGrid = new QGridLayout;
    stateGrid->setContentsMargins(0, 0, 0, 0);
    stateGrid->setHorizontalSpacing(18);
    stateGrid->setVerticalSpacing(2);
    stateGrid->addWidget(
        new ValueRow(QStringLiteral("Mode"), QStringLiteral("Manual"), statePanel), 0, 0);
    stateGrid->addWidget(
        new ValueRow(QStringLiteral("Servo"), QStringLiteral("Disabled"), statePanel), 0, 1);
    stateGrid->addWidget(
        new ValueRow(QStringLiteral("Safety"), QStringLiteral("--"), statePanel), 1, 0);
    stateGrid->addWidget(
        new ValueRow(QStringLiteral("Controller"), QStringLiteral("--"), statePanel), 1, 1);
    statePanel->contentLayout()->addLayout(stateGrid);
    leftLayout->addWidget(statePanel);

    mainGrid->addWidget(leftColumn, 0, 0);
    mainGrid->addWidget(robotViewPanel, 0, 1);

    connect(speedSlider, &QSlider::valueChanged, this, [speedValue](int value) {
        speedValue->setText(QStringLiteral("%1 %").arg(value));
    });
}

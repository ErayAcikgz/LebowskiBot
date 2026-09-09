#include "OverviewPage.h"

#include "ui/widgets/JointStatusWidget.h"
#include "ui/widgets/Panel.h"
#include "ui/widgets/StatusIndicator.h"
#include "ui/widgets/ValueRow.h"

#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

OverviewPage::OverviewPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 38, 48, 40);
    layout->setSpacing(12);

    auto *titleLabel = new QLabel(QStringLiteral("Overview"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(QStringLiteral("Robot system overview and status."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(14);

    auto *grid = new QGridLayout;
    grid->setObjectName(QStringLiteral("overviewGrid"));
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(18);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    grid->setRowMinimumHeight(0, 168);
    grid->setRowMinimumHeight(1, 188);
    layout->addLayout(grid);
    layout->addStretch(1);

    auto *robotPanel = new Panel(QStringLiteral("ROBOT STATUS"), this);
    robotPanel->contentLayout()->addWidget(
        new StatusIndicator(QStringLiteral("DISCONNECTED"), StatusIndicator::Tone::Neutral,
                            robotPanel));

    auto *estopLabel = new QLabel(QStringLiteral("E-STOP"), robotPanel);
    estopLabel->setObjectName(QStringLiteral("safetyLabel"));
    robotPanel->contentLayout()->addWidget(estopLabel);
    robotPanel->contentLayout()->addWidget(
        new StatusIndicator(QStringLiteral("NOT ACTIVE"), StatusIndicator::Tone::Safety,
                            robotPanel));
    grid->addWidget(robotPanel, 0, 0);

    auto *jointPanel = new Panel(QStringLiteral("JOINT STATUS"), this);
    jointPanel->contentLayout()->addWidget(new JointStatusWidget(6, jointPanel));
    grid->addWidget(jointPanel, 0, 1);

    auto *powerPanel = new Panel(QStringLiteral("POWER"), this);
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Battery"),
                                                        QStringLiteral("-- V"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Current"),
                                                        QStringLiteral("-- A"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Power"),
                                                        QStringLiteral("-- W"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Temperature"),
                                                        QStringLiteral("-- °C"), powerPanel));
    grid->addWidget(powerPanel, 1, 0);

    auto *systemPanel = new Panel(QStringLiteral("SYSTEM"), this);
    systemPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Connection"),
                                                         QStringLiteral("Disconnected"), systemPanel));
    systemPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Controller"),
                                                         QStringLiteral("--"), systemPanel));
    systemPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Errors"),
                                                         QStringLiteral("0"), systemPanel));
    systemPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Warnings"),
                                                         QStringLiteral("0"), systemPanel));
    grid->addWidget(systemPanel, 1, 1);
}

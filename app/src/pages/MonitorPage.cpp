#include "MonitorPage.h"

#include "ui/widgets/Panel.h"
#include "ui/widgets/StatusIndicator.h"
#include "ui/widgets/ValueRow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QSizePolicy>
#include <QStringList>
#include <QVBoxLayout>

MonitorPage::MonitorPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 38, 48, 40);
    layout->setSpacing(12);

    auto *titleLabel = new QLabel(QStringLiteral("Monitor"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(QStringLiteral("Robot telemetry and system diagnostics."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(14);

    auto *topGrid = new QGridLayout;
    topGrid->setHorizontalSpacing(18);
    topGrid->setVerticalSpacing(18);
    topGrid->setColumnStretch(0, 1);
    topGrid->setColumnStretch(1, 1);
    layout->addLayout(topGrid, 3);

    auto *jointPanel = new Panel(QStringLiteral("JOINT TELEMETRY"), this);
    jointPanel->setCompact(true);
    auto *jointGrid = new QGridLayout;
    jointGrid->setContentsMargins(0, 0, 0, 0);
    jointGrid->setHorizontalSpacing(16);
    jointGrid->setVerticalSpacing(6);
    const QStringList jointHeaders{QStringLiteral("Joint"), QStringLiteral("Position"),
                                   QStringLiteral("Velocity"), QStringLiteral("Current")};
    for (int column = 0; column < jointHeaders.size(); ++column) {
        auto *header = new QLabel(jointHeaders.at(column), jointPanel);
        header->setObjectName(QStringLiteral("valueLabel"));
        jointGrid->addWidget(header, 0, column);
    }
    jointGrid->setColumnStretch(0, 1);
    jointGrid->setColumnStretch(1, 1);
    jointGrid->setColumnStretch(2, 1);
    jointGrid->setColumnStretch(3, 1);
    for (int joint = 0; joint < 6; ++joint) {
        auto *name = new QLabel(QStringLiteral("J%1").arg(joint + 1), jointPanel);
        name->setObjectName(QStringLiteral("jointLabel"));
        jointGrid->addWidget(name, joint + 1, 0);

        const QStringList values{QStringLiteral("--.-°"), QStringLiteral("--.-°/s"),
                                 QStringLiteral("-- A")};
        for (int column = 0; column < values.size(); ++column) {
            auto *value = new QLabel(values.at(column), jointPanel);
            value->setObjectName(QStringLiteral("jointValue"));
            value->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            jointGrid->addWidget(value, joint + 1, column + 1);
        }
    }
    jointPanel->contentLayout()->addLayout(jointGrid);
    topGrid->addWidget(jointPanel, 0, 0);

    auto *statusPanel = new Panel(QStringLiteral("SYSTEM STATUS"), this);
    statusPanel->setCompact(true);
    auto *connectionRow = new QWidget(statusPanel);
    auto *connectionLayout = new QHBoxLayout(connectionRow);
    connectionLayout->setContentsMargins(0, 0, 0, 0);
    connectionLayout->setSpacing(12);
    auto *connectionLabel = new QLabel(QStringLiteral("Connection"), connectionRow);
    connectionLabel->setObjectName(QStringLiteral("valueLabel"));
    connectionLayout->addWidget(connectionLabel);
    connectionLayout->addStretch();
    connectionLayout->addWidget(new StatusIndicator(QStringLiteral("DISCONNECTED"),
                                                    StatusIndicator::Tone::Neutral, connectionRow));
    statusPanel->contentLayout()->addWidget(connectionRow);
    statusPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Controller"),
                                                         QStringLiteral("--"), statusPanel));
    statusPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Communication"),
                                                         QStringLiteral("--"), statusPanel));
    statusPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Errors"),
                                                         QStringLiteral("0"), statusPanel));
    statusPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Warnings"),
                                                         QStringLiteral("0"), statusPanel));
    topGrid->addWidget(statusPanel, 0, 1);

    auto *middleGrid = new QGridLayout;
    middleGrid->setHorizontalSpacing(18);
    middleGrid->setVerticalSpacing(18);
    middleGrid->setColumnStretch(0, 1);
    middleGrid->setColumnStretch(1, 1);
    layout->addLayout(middleGrid, 2);

    auto *powerPanel = new Panel(QStringLiteral("POWER"), this);
    powerPanel->setCompact(true);
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Battery Voltage"),
                                                        QStringLiteral("-- V"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("System Current"),
                                                        QStringLiteral("-- A"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Power Consumption"),
                                                        QStringLiteral("-- W"), powerPanel));
    powerPanel->contentLayout()->addWidget(new ValueRow(QStringLiteral("Battery Level"),
                                                        QStringLiteral("-- %"), powerPanel));
    middleGrid->addWidget(powerPanel, 0, 0);

    auto *temperaturePanel = new Panel(QStringLiteral("TEMPERATURE"), this);
    temperaturePanel->setCompact(true);
    const QStringList temperatureLabels{QStringLiteral("System"), QStringLiteral("J1"),
                                        QStringLiteral("J2"), QStringLiteral("J3"),
                                        QStringLiteral("J4"), QStringLiteral("J5"),
                                        QStringLiteral("J6")};
    for (const QString &label : temperatureLabels) {
        temperaturePanel->contentLayout()->addWidget(new ValueRow(label, QStringLiteral("-- °C"),
                                                                  temperaturePanel));
    }
    middleGrid->addWidget(temperaturePanel, 0, 1);

    auto *eventsPanel = new Panel(QStringLiteral("EVENTS / DIAGNOSTICS"), this);
    eventsPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *eventsList = new QListWidget(eventsPanel);
    eventsList->setObjectName(QStringLiteral("eventsList"));
    eventsList->setFocusPolicy(Qt::NoFocus);
    eventsList->setSelectionMode(QAbstractItemView::NoSelection);
    eventsList->addItem(QStringLiteral("No events available"));
    eventsPanel->contentLayout()->addWidget(eventsList);
    layout->addWidget(eventsPanel, 3);
}

#include "SettingsPage.h"

#include "ui/widgets/Panel.h"
#include "ui/widgets/StatusIndicator.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 38, 48, 40);
    layout->setSpacing(12);

    auto *titleLabel = new QLabel(QStringLiteral("Settings"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(QStringLiteral("Application and connection settings."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(14);

    auto *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    auto *content = new QWidget(this);
    content->setObjectName(QStringLiteral("settingsContent"));
    content->setMaximumWidth(760);
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto *contentColumn = new QVBoxLayout(content);
    contentColumn->setContentsMargins(0, 0, 0, 0);
    contentColumn->setSpacing(18);
    contentLayout->addWidget(content);
    contentLayout->addStretch();
    layout->addLayout(contentLayout);
    layout->addStretch();

    auto *connectionPanel = new Panel(QStringLiteral("CONNECTION"), content);
    auto *connectionForm = new QFormLayout;
    connectionForm->setContentsMargins(0, 0, 0, 0);
    connectionForm->setHorizontalSpacing(22);
    connectionForm->setVerticalSpacing(12);
    connectionForm->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    connectionForm->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    auto *interfaceSelector = new QComboBox(connectionPanel);
    interfaceSelector->setObjectName(QStringLiteral("settingsCombo"));
    interfaceSelector->addItem(QStringLiteral("Serial"));
    connectionForm->addRow(QStringLiteral("Interface"), interfaceSelector);

    auto *portInput = new QLineEdit(QStringLiteral("/dev/ttyUSB0"), connectionPanel);
    portInput->setObjectName(QStringLiteral("settingsInput"));
    connectionForm->addRow(QStringLiteral("Port"), portInput);

    auto *baudSelector = new QComboBox(connectionPanel);
    baudSelector->setObjectName(QStringLiteral("settingsCombo"));
    baudSelector->addItems({QStringLiteral("9600"), QStringLiteral("19200"),
                            QStringLiteral("38400"), QStringLiteral("57600"),
                            QStringLiteral("115200"), QStringLiteral("230400")});
    baudSelector->setCurrentText(QStringLiteral("115200"));
    connectionForm->addRow(QStringLiteral("Baud Rate"), baudSelector);

    auto *statusRow = new QWidget(connectionPanel);
    auto *statusLayout = new QHBoxLayout(statusRow);
    statusLayout->setContentsMargins(0, 0, 0, 0);
    statusLayout->addWidget(new StatusIndicator(QStringLiteral("Disconnected"),
                                                StatusIndicator::Tone::Neutral, statusRow));
    statusLayout->addStretch();
    connectionForm->addRow(QStringLiteral("Status"), statusRow);

    auto *testButton = new QPushButton(QStringLiteral("TEST CONNECTION"), connectionPanel);
    testButton->setObjectName(QStringLiteral("secondaryButton"));
    testButton->setMinimumHeight(32);
    testButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    connectionPanel->contentLayout()->addLayout(connectionForm);
    connectionPanel->contentLayout()->addWidget(testButton, 0, Qt::AlignLeft);
    contentColumn->addWidget(connectionPanel);

    auto *applicationPanel = new Panel(QStringLiteral("APPLICATION"), content);
    auto *applicationForm = new QFormLayout;
    applicationForm->setContentsMargins(0, 0, 0, 0);
    applicationForm->setHorizontalSpacing(22);
    applicationForm->setVerticalSpacing(12);
    applicationForm->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    applicationForm->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    auto *loggingSelector = new QComboBox(applicationPanel);
    loggingSelector->setObjectName(QStringLiteral("settingsCombo"));
    loggingSelector->addItems({QStringLiteral("Error"), QStringLiteral("Warning"),
                               QStringLiteral("Info"), QStringLiteral("Debug")});
    loggingSelector->setCurrentText(QStringLiteral("Info"));
    applicationForm->addRow(QStringLiteral("Logging Level"), loggingSelector);

    auto *debugLogging = new QCheckBox(QStringLiteral("Debug Logging"), applicationPanel);
    debugLogging->setObjectName(QStringLiteral("settingsCheckBox"));
    applicationForm->addRow(QString(), debugLogging);

    auto *logDirectory = new QLineEdit(QStringLiteral("~/.local/share/lebowski/logs"),
                                        applicationPanel);
    logDirectory->setObjectName(QStringLiteral("settingsInput"));
    logDirectory->setReadOnly(true);
    applicationForm->addRow(QStringLiteral("Log Directory"), logDirectory);

    applicationPanel->contentLayout()->addLayout(applicationForm);
    contentColumn->addWidget(applicationPanel);

    auto *information = new QLabel(applicationPanel);
    information->setObjectName(QStringLiteral("settingsInformation"));
    information->setText(QStringLiteral("Lebowski\nVersion: 0.1.0"));
    contentColumn->addWidget(information);
}

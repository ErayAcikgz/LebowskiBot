#include "NavigationPanel.h"

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

NavigationPanel::NavigationPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("navigationPanel"));
    setMinimumWidth(216);
    setMaximumWidth(280);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 24, 16, 20);
    layout->setSpacing(6);

    auto *sectionLabel = new QLabel(QStringLiteral("WORKSPACE"), this);
    sectionLabel->setObjectName(QStringLiteral("navigationSection"));
    layout->addWidget(sectionLabel);
    layout->addSpacing(8);

    const QStringList pages = {
        QStringLiteral("Overview"), QStringLiteral("Control"), QStringLiteral("Program"),
        QStringLiteral("Teach"), QStringLiteral("AI"), QStringLiteral("Monitor"),
        QStringLiteral("Settings")};

    auto *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    for (int index = 0; index < pages.size(); ++index) {
        auto *button = new QPushButton(pages.at(index), this);
        button->setObjectName(QStringLiteral("navigationButton"));
        button->setCheckable(true);
        button->setAutoExclusive(true);
        button->setMinimumHeight(42);
        buttonGroup->addButton(button, index);
        layout->addWidget(button);
    }

    layout->addStretch();
    buttonGroup->button(0)->setChecked(true);

    connect(buttonGroup, &QButtonGroup::idClicked, this, &NavigationPanel::pageSelected);
}

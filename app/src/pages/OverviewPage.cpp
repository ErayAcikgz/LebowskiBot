#include "OverviewPage.h"

#include <QLabel>
#include <QVBoxLayout>

OverviewPage::OverviewPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    auto *placeholder = new QLabel(QStringLiteral("Lebowski"), this);
    placeholder->setAlignment(Qt::AlignCenter);

    layout->addWidget(placeholder);
}

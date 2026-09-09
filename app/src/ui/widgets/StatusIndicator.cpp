#include "StatusIndicator.h"

#include <QHBoxLayout>
#include <QLabel>

StatusIndicator::StatusIndicator(const QString &text, Tone tone, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("statusIndicator"));
    setProperty("tone", tone == Tone::Safety ? QStringLiteral("safety")
                                               : QStringLiteral("neutral"));

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(9);

    auto *dot = new QLabel(this);
    dot->setObjectName(QStringLiteral("statusDot"));
    dot->setFixedSize(8, 8);
    layout->addWidget(dot);

    auto *label = new QLabel(text, this);
    label->setObjectName(QStringLiteral("statusText"));
    layout->addWidget(label);
    layout->addStretch();
}

#include "ValueRow.h"

#include <QHBoxLayout>
#include <QLabel>

ValueRow::ValueRow(const QString &label, const QString &value, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("valueRow"));

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    auto *labelWidget = new QLabel(label, this);
    labelWidget->setObjectName(QStringLiteral("valueLabel"));
    layout->addWidget(labelWidget);
    layout->addStretch();

    auto *valueWidget = new QLabel(value, this);
    valueWidget->setObjectName(QStringLiteral("valueText"));
    valueWidget->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(valueWidget);
}

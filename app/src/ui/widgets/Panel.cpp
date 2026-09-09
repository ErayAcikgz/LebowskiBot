#include "Panel.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

Panel::Panel(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("overviewPanel"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 16, 18, 18);
    layout->setSpacing(16);

    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("panelTitle"));
    layout->addWidget(titleLabel);

    auto *divider = new QFrame(this);
    divider->setObjectName(QStringLiteral("panelDivider"));
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    layout->addWidget(divider);

    contentLayout_ = new QVBoxLayout;
    contentLayout_->setContentsMargins(0, 0, 0, 0);
    contentLayout_->setSpacing(12);
    layout->addLayout(contentLayout_);
    layout->addStretch();
}

QVBoxLayout *Panel::contentLayout() const
{
    return contentLayout_;
}

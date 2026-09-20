#include "Panel.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

Panel::Panel(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("overviewPanel"));

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(18, 16, 18, 18);
    layout_->setSpacing(16);

    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("panelTitle"));
    layout_->addWidget(titleLabel);

    auto *divider = new QFrame(this);
    divider->setObjectName(QStringLiteral("panelDivider"));
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    layout_->addWidget(divider);

    contentLayout_ = new QVBoxLayout;
    contentLayout_->setContentsMargins(0, 0, 0, 0);
    contentLayout_->setSpacing(12);
    layout_->addLayout(contentLayout_);
    layout_->addStretch();
}

QVBoxLayout *Panel::contentLayout() const
{
    return contentLayout_;
}

void Panel::setCompact(bool compact)
{
    if (compact) {
        layout_->setContentsMargins(12, 10, 12, 10);
        layout_->setSpacing(8);
        contentLayout_->setSpacing(6);
        return;
    }

    layout_->setContentsMargins(18, 16, 18, 18);
    layout_->setSpacing(16);
    contentLayout_->setSpacing(12);
}

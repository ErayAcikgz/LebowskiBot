#include "PlaceholderPage.h"

#include <QLabel>
#include <QVBoxLayout>

PlaceholderPage::PlaceholderPage(const QString &title, const QString &description, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 44, 48, 44);
    layout->setSpacing(12);

    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(description, this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    descriptionLabel->setWordWrap(true);
    layout->addWidget(descriptionLabel);
    layout->addStretch();
}

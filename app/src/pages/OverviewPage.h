#ifndef LEBOWSKI_OVERVIEWPAGE_H
#define LEBOWSKI_OVERVIEWPAGE_H

#include "PlaceholderPage.h"

class OverviewPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit OverviewPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_OVERVIEWPAGE_H

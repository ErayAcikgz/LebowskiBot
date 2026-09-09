#ifndef LEBOWSKI_AIPAGE_H
#define LEBOWSKI_AIPAGE_H

#include "PlaceholderPage.h"

class AIPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit AIPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_AIPAGE_H

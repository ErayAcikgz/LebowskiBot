#ifndef LEBOWSKI_CONTROLPAGE_H
#define LEBOWSKI_CONTROLPAGE_H

#include "PlaceholderPage.h"

class ControlPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit ControlPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_CONTROLPAGE_H

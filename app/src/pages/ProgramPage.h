#ifndef LEBOWSKI_PROGRAMPAGE_H
#define LEBOWSKI_PROGRAMPAGE_H

#include "PlaceholderPage.h"

class ProgramPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit ProgramPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_PROGRAMPAGE_H

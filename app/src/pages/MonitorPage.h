#ifndef LEBOWSKI_MONITORPAGE_H
#define LEBOWSKI_MONITORPAGE_H

#include "PlaceholderPage.h"

class MonitorPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit MonitorPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_MONITORPAGE_H

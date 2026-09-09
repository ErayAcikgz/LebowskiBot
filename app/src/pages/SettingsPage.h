#ifndef LEBOWSKI_SETTINGSPAGE_H
#define LEBOWSKI_SETTINGSPAGE_H

#include "PlaceholderPage.h"

class SettingsPage final : public PlaceholderPage
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_SETTINGSPAGE_H

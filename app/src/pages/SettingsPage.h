#ifndef LEBOWSKI_SETTINGSPAGE_H
#define LEBOWSKI_SETTINGSPAGE_H

#include <QWidget>

class SettingsPage final : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_SETTINGSPAGE_H

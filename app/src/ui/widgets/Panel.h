#ifndef LEBOWSKI_PANEL_H
#define LEBOWSKI_PANEL_H

#include <QWidget>

class QVBoxLayout;

class Panel final : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(const QString &title, QWidget *parent = nullptr);

    QVBoxLayout *contentLayout() const;

private:
    QVBoxLayout *contentLayout_ = nullptr;
};

#endif // LEBOWSKI_PANEL_H

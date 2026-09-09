#ifndef LEBOWSKI_TOPBAR_H
#define LEBOWSKI_TOPBAR_H

#include <QWidget>

class TopBar final : public QWidget
{
    Q_OBJECT

public:
    explicit TopBar(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_TOPBAR_H

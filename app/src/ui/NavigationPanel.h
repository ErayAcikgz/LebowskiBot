#ifndef LEBOWSKI_NAVIGATIONPANEL_H
#define LEBOWSKI_NAVIGATIONPANEL_H

#include <QWidget>

class NavigationPanel final : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationPanel(QWidget *parent = nullptr);

signals:
    void pageSelected(int index);
};

#endif // LEBOWSKI_NAVIGATIONPANEL_H

#ifndef LEBOWSKI_BOTTOMSTATUSBAR_H
#define LEBOWSKI_BOTTOMSTATUSBAR_H

#include <QWidget>

class BottomStatusBar final : public QWidget
{
    Q_OBJECT

public:
    explicit BottomStatusBar(QWidget *parent = nullptr);
};

#endif // LEBOWSKI_BOTTOMSTATUSBAR_H

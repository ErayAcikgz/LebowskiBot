#ifndef LEBOWSKI_JOINTSTATUSWIDGET_H
#define LEBOWSKI_JOINTSTATUSWIDGET_H

#include <QWidget>

class JointStatusWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit JointStatusWidget(int jointCount = 6, QWidget *parent = nullptr);
};

#endif // LEBOWSKI_JOINTSTATUSWIDGET_H

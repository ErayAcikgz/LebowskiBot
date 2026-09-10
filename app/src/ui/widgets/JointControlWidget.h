#ifndef LEBOWSKI_JOINTCONTROLWIDGET_H
#define LEBOWSKI_JOINTCONTROLWIDGET_H

#include <QWidget>

class JointControlWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit JointControlWidget(int jointCount = 6, QWidget *parent = nullptr);
};

#endif // LEBOWSKI_JOINTCONTROLWIDGET_H

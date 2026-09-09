#ifndef LEBOWSKI_VALUEROW_H
#define LEBOWSKI_VALUEROW_H

#include <QWidget>

class ValueRow final : public QWidget
{
    Q_OBJECT

public:
    explicit ValueRow(const QString &label, const QString &value, QWidget *parent = nullptr);
};

#endif // LEBOWSKI_VALUEROW_H

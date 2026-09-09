#ifndef LEBOWSKI_PLACEHOLDERPAGE_H
#define LEBOWSKI_PLACEHOLDERPAGE_H

#include <QWidget>

class PlaceholderPage : public QWidget
{
    Q_OBJECT

public:
    PlaceholderPage(const QString &title, const QString &description, QWidget *parent = nullptr);
};

#endif // LEBOWSKI_PLACEHOLDERPAGE_H

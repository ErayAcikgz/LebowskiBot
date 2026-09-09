#ifndef LEBOWSKI_STATUSINDICATOR_H
#define LEBOWSKI_STATUSINDICATOR_H

#include <QWidget>

class StatusIndicator final : public QWidget
{
    Q_OBJECT

public:
    enum class Tone {
        Neutral,
        Safety
    };

    explicit StatusIndicator(const QString &text, Tone tone = Tone::Neutral,
                             QWidget *parent = nullptr);
};

#endif // LEBOWSKI_STATUSINDICATOR_H

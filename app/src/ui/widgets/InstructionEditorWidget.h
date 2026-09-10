#ifndef LEBOWSKI_INSTRUCTIONEDITORWIDGET_H
#define LEBOWSKI_INSTRUCTIONEDITORWIDGET_H

#include <QVector>
#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QStackedWidget;

struct InstructionParameters
{
    QVector<double> numericValues;
    QString output = QStringLiteral("DO1");
    QString state = QStringLiteral("OFF");
    QString text;
    QString program = QStringLiteral("Pick & Place");
};

class InstructionEditorWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit InstructionEditorWidget(QWidget *parent = nullptr);

    void setInstruction(const QString &type, const InstructionParameters &parameters);

signals:
    void parametersChanged(const InstructionParameters &parameters, const QString &summary);

private:
    QDoubleSpinBox *addNumberField(QGridLayout *layout, int row, int column,
                                   const QString &label, const QString &suffix,
                                   double minimum, double maximum, int decimals);
    void emitChanged();
    InstructionParameters currentParameters() const;
    QString currentSummary(const InstructionParameters &parameters) const;

    QString currentType_;
    bool updating_ = false;
    QLabel *typeLabel_ = nullptr;
    QStackedWidget *pages_ = nullptr;
    QVector<QDoubleSpinBox *> moveJInputs_;
    QVector<QDoubleSpinBox *> moveLInputs_;
    QVector<QDoubleSpinBox *> waitInputs_;
    QComboBox *outputSelector_ = nullptr;
    QComboBox *stateSelector_ = nullptr;
    QLineEdit *commentInput_ = nullptr;
    QComboBox *programSelector_ = nullptr;
};

#endif // LEBOWSKI_INSTRUCTIONEDITORWIDGET_H

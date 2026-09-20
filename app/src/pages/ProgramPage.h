#ifndef LEBOWSKI_PROGRAMPAGE_H
#define LEBOWSKI_PROGRAMPAGE_H

#include "ui/widgets/InstructionEditorWidget.h"

#include <QVector>
#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;
class InstructionEditorWidget;

class ProgramPage final : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramPage(QWidget *parent = nullptr);

private:
    struct Instruction {
        QString type;
        QString detail;
        InstructionParameters parameters;
    };

    struct Program {
        QString name;
        QVector<Instruction> instructions;
    };

    void selectProgram(int index);
    void refreshProgramList();
    void refreshInstructionList();
    void showInstructionEditor(int index);
    void addInstruction(const QString &type, const QString &detail);

    QVector<Program> programs_;
    int selectedProgram_ = 0;
    int selectedInstruction_ = -1;
    QListWidget *programList_ = nullptr;
    QListWidget *instructionList_ = nullptr;
    QLabel *programNameLabel_ = nullptr;
    QLabel *programStatus_ = nullptr;
    InstructionEditorWidget *instructionEditor_ = nullptr;
};

#endif // LEBOWSKI_PROGRAMPAGE_H

#include "ProgramPage.h"

#include "ui/widgets/InstructionEditorWidget.h"
#include "ui/widgets/Panel.h"
#include "ui/widgets/StatusIndicator.h"

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QSizePolicy>
#include <QToolButton>
#include <QVBoxLayout>

namespace {

InstructionParameters defaultParameters(const QString &type)
{
    InstructionParameters parameters;
    if (type == QStringLiteral("MOVEJ")) {
        parameters.numericValues = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0};
    } else if (type == QStringLiteral("MOVEL")) {
        parameters.numericValues = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0};
    } else if (type == QStringLiteral("WAIT")) {
        parameters.numericValues = {1.0};
    }
    return parameters;
}

QString instructionMenuLabel(const QString &type)
{
    if (type == QStringLiteral("MOVEJ")) {
        return QStringLiteral("Move Joint");
    }
    if (type == QStringLiteral("MOVEL")) {
        return QStringLiteral("Move Linear");
    }
    if (type == QStringLiteral("WAIT")) {
        return QStringLiteral("Wait");
    }
    if (type == QStringLiteral("SET_IO")) {
        return QStringLiteral("Set I/O");
    }
    if (type == QStringLiteral("COMMENT")) {
        return QStringLiteral("Comment");
    }
    return QStringLiteral("Call Program");
}

} // namespace

ProgramPage::ProgramPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    programs_ = {
        {QStringLiteral("Pick & Place"), {
             {QStringLiteral("MOVEJ"), QStringLiteral("Home"), defaultParameters(QStringLiteral("MOVEJ"))},
             {QStringLiteral("MOVEJ"), QStringLiteral("Pick Approach"), defaultParameters(QStringLiteral("MOVEJ"))},
             {QStringLiteral("MOVEL"), QStringLiteral("Pick"), defaultParameters(QStringLiteral("MOVEL"))},
             {QStringLiteral("WAIT"), QStringLiteral("1.0 s"), defaultParameters(QStringLiteral("WAIT"))},
             {QStringLiteral("MOVEL"), QStringLiteral("Place"), defaultParameters(QStringLiteral("MOVEL"))},
             {QStringLiteral("MOVEJ"), QStringLiteral("Home"), defaultParameters(QStringLiteral("MOVEJ"))}}},
        {QStringLiteral("Test Motion"), {
             {QStringLiteral("MOVEJ"), QStringLiteral("Home"), defaultParameters(QStringLiteral("MOVEJ"))},
             {QStringLiteral("MOVEL"), QStringLiteral("Test Point"), defaultParameters(QStringLiteral("MOVEL"))}}},
        {QStringLiteral("Demo Program"), {
             {QStringLiteral("COMMENT"), QStringLiteral("Demo sequence"), defaultParameters(QStringLiteral("COMMENT"))},
             {QStringLiteral("WAIT"), QStringLiteral("1.0 s"), defaultParameters(QStringLiteral("WAIT"))}}}
    };

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 30, 48, 28);
    layout->setSpacing(8);

    auto *titleLabel = new QLabel(QStringLiteral("Program"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(
        QStringLiteral("Create, edit and execute robot programs."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(12);

    auto *workspace = new QHBoxLayout;
    workspace->setSpacing(14);
    layout->addLayout(workspace, 1);

    auto *programPanel = new Panel(QStringLiteral("PROGRAMS"), this);
    programPanel->setCompact(true);
    programPanel->setMinimumWidth(190);
    programPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    auto *newProgramButton = new QPushButton(QStringLiteral("+  NEW PROGRAM"), programPanel);
    newProgramButton->setObjectName(QStringLiteral("secondaryButton"));
    newProgramButton->setMinimumHeight(30);
    programPanel->contentLayout()->addWidget(newProgramButton);

    programList_ = new QListWidget(programPanel);
    programList_->setObjectName(QStringLiteral("programList"));
    programList_->setSelectionMode(QAbstractItemView::SingleSelection);
    programList_->setMinimumHeight(140);
    programPanel->contentLayout()->addWidget(programList_, 1);
    workspace->addWidget(programPanel, 1);

    auto *editorColumn = new QVBoxLayout;
    editorColumn->setContentsMargins(0, 0, 0, 0);
    editorColumn->setSpacing(10);
    workspace->addLayout(editorColumn, 4);

    auto *editorPanel = new Panel(QStringLiteral("PROGRAM"), this);
    editorPanel->setCompact(true);
    editorPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    auto *editorHeader = new QHBoxLayout;
    editorHeader->setContentsMargins(0, 0, 0, 2);
    programNameLabel_ = new QLabel(editorPanel);
    programNameLabel_->setObjectName(QStringLiteral("programName"));
    editorHeader->addWidget(programNameLabel_);
    editorHeader->addStretch();

    auto *addButton = new QToolButton(editorPanel);
    addButton->setObjectName(QStringLiteral("secondaryButton"));
    addButton->setText(QStringLiteral("+  ADD INSTRUCTION"));
    addButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    addButton->setPopupMode(QToolButton::InstantPopup);
    auto *addMenu = new QMenu(addButton);
    const QVector<QString> instructionTypes = {
        QStringLiteral("MOVEJ"), QStringLiteral("MOVEL"), QStringLiteral("WAIT"),
        QStringLiteral("SET_IO"), QStringLiteral("COMMENT"), QStringLiteral("CALL")};
    for (const QString &type : instructionTypes) {
        auto *action = addMenu->addAction(instructionMenuLabel(type));
        connect(action, &QAction::triggered, this, [this, type] {
            addInstruction(type, QString());
        });
    }
    addButton->setMenu(addMenu);
    editorHeader->addWidget(addButton);
    editorPanel->contentLayout()->addLayout(editorHeader);

    instructionList_ = new QListWidget(editorPanel);
    instructionList_->setObjectName(QStringLiteral("instructionList"));
    instructionList_->setSelectionMode(QAbstractItemView::SingleSelection);
    instructionList_->setUniformItemSizes(true);
    editorPanel->contentLayout()->addWidget(instructionList_, 1);
    editorColumn->addWidget(editorPanel, 1);

    auto *propertiesPanel = new Panel(QStringLiteral("INSTRUCTION"), this);
    propertiesPanel->setCompact(true);
    propertiesPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    instructionEditor_ = new InstructionEditorWidget(propertiesPanel);
    propertiesPanel->contentLayout()->addWidget(instructionEditor_);
    editorColumn->addWidget(propertiesPanel);

    auto *executionPanel = new Panel(QStringLiteral("PROGRAM CONTROL"), this);
    executionPanel->setCompact(true);
    executionPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto *executionRow = new QHBoxLayout;
    executionRow->setContentsMargins(0, 0, 0, 0);
    executionRow->setSpacing(8);
    auto *runButton = new QPushButton(QStringLiteral("RUN"), executionPanel);
    runButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *pauseButton = new QPushButton(QStringLiteral("PAUSE"), executionPanel);
    pauseButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *stopButton = new QPushButton(QStringLiteral("STOP"), executionPanel);
    stopButton->setObjectName(QStringLiteral("stopButton"));
    executionRow->addWidget(runButton);
    executionRow->addWidget(pauseButton);
    executionRow->addWidget(stopButton);
    executionRow->addSpacing(18);
    auto *statusIndicator = new StatusIndicator(QStringLiteral("IDLE"), StatusIndicator::Tone::Neutral,
                                                 executionPanel);
    executionRow->addWidget(statusIndicator);
    executionRow->addStretch();
    programStatus_ = new QLabel(executionPanel);
    programStatus_->setObjectName(QStringLiteral("programStatus"));
    executionRow->addWidget(programStatus_);
    executionPanel->contentLayout()->addLayout(executionRow);
    editorColumn->addWidget(executionPanel);

    connect(programList_, &QListWidget::currentRowChanged, this, &ProgramPage::selectProgram);
    connect(instructionList_, &QListWidget::currentRowChanged, this,
            &ProgramPage::showInstructionEditor);
    connect(newProgramButton, &QPushButton::clicked, this, [this] {
        const QString name = QStringLiteral("New Program %1").arg(programs_.size() + 1);
        programs_.append({name, {}});
        refreshProgramList();
        programList_->setCurrentRow(programs_.size() - 1);
    });
    connect(instructionEditor_, &InstructionEditorWidget::parametersChanged,
            this, [this](const InstructionParameters &parameters, const QString &summary) {
        if (selectedProgram_ < 0 || selectedProgram_ >= programs_.size()
            || selectedInstruction_ < 0
            || selectedInstruction_ >= programs_[selectedProgram_].instructions.size()) {
            return;
        }
        auto &instruction = programs_[selectedProgram_].instructions[selectedInstruction_];
        instruction.parameters = parameters;
        instruction.detail = summary;
        refreshInstructionList();
        instructionList_->setCurrentRow(selectedInstruction_);
    });
    connect(runButton, &QPushButton::clicked, this, [this] {
        programStatus_->setText(QStringLiteral("Program: local UI placeholder  |  Status: RUN REQUEST"));
    });
    connect(pauseButton, &QPushButton::clicked, this, [this] {
        programStatus_->setText(QStringLiteral("Program: local UI placeholder  |  Status: PAUSE REQUEST"));
    });
    connect(stopButton, &QPushButton::clicked, this, [this] {
        programStatus_->setText(QStringLiteral("Program: local UI placeholder  |  Status: STOP REQUEST"));
    });

    programStatus_->setText(QStringLiteral("Program: Pick & Place  |  Status: Idle  |  Instruction: --"));
    refreshProgramList();
    programList_->setCurrentRow(0);
}

void ProgramPage::refreshProgramList()
{
    programList_->clear();
    for (const Program &program : programs_) {
        programList_->addItem(program.name);
    }
}

void ProgramPage::selectProgram(int index)
{
    if (index < 0 || index >= programs_.size()) {
        return;
    }
    selectedProgram_ = index;
    selectedInstruction_ = -1;
    programNameLabel_->setText(programs_[index].name);
    programStatus_->setText(QStringLiteral("Program: %1  |  Status: Idle  |  Instruction: --")
                                .arg(programs_[index].name));
    refreshInstructionList();
}

void ProgramPage::refreshInstructionList()
{
    const int instructionToSelect = selectedInstruction_;
    instructionList_->clear();
    if (selectedProgram_ < 0 || selectedProgram_ >= programs_.size()) {
        return;
    }

    const Program &program = programs_[selectedProgram_];
    for (int index = 0; index < program.instructions.size(); ++index) {
        const Instruction &instruction = program.instructions[index];
        auto *item = new QListWidgetItem(instructionList_);
        item->setText(QStringLiteral("%1    %2    %3")
                          .arg(index + 1, 2, 10, QLatin1Char('0'))
                          .arg(instruction.type, -8)
                          .arg(instruction.detail));
        item->setData(Qt::UserRole, index);
        item->setSizeHint(QSize(0, 30));
    }
    instructionList_->setCurrentRow(instructionToSelect >= 0 ? instructionToSelect : -1);
    if (instructionToSelect < 0) {
        showInstructionEditor(-1);
    }
}

void ProgramPage::showInstructionEditor(int index)
{
    selectedInstruction_ = index;
    if (selectedProgram_ < 0 || selectedProgram_ >= programs_.size()
        || index < 0 || index >= programs_[selectedProgram_].instructions.size()) {
        instructionEditor_->setInstruction(QString(), {});
        return;
    }

    const Instruction &instruction = programs_[selectedProgram_].instructions[index];
    instructionEditor_->setInstruction(instruction.type, instruction.parameters);
    programStatus_->setText(QStringLiteral("Program: %1  |  Status: Idle  |  Instruction: %2")
                                .arg(programs_[selectedProgram_].name, instruction.type));
}

void ProgramPage::addInstruction(const QString &type, const QString &detail)
{
    if (selectedProgram_ < 0 || selectedProgram_ >= programs_.size()) {
        return;
    }
    programs_[selectedProgram_].instructions.append(
        {type, detail.isEmpty() ? QStringLiteral("Target 01") : detail, defaultParameters(type)});
    selectedInstruction_ = programs_[selectedProgram_].instructions.size() - 1;
    refreshInstructionList();
    instructionList_->setCurrentRow(selectedInstruction_);
}

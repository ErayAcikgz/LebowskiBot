#include "InstructionEditorWidget.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace {

enum PageIndex {
    EmptyPage,
    MoveJPage,
    MoveLPage,
    WaitPage,
    SetIoPage,
    CommentPage,
    CallPage
};

QLabel *fieldLabel(const QString &text, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setObjectName(QStringLiteral("valueLabel"));
    return label;
}

QLabel *sectionLabel(const QString &text, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setObjectName(QStringLiteral("instructionSection"));
    return label;
}

QDoubleSpinBox *configureSpinBox(const QString &suffix, double minimum, double maximum,
                                 int decimals, QWidget *parent)
{
    auto *spinBox = new QDoubleSpinBox(parent);
    spinBox->setObjectName(QStringLiteral("instructionSpin"));
    spinBox->setRange(minimum, maximum);
    spinBox->setDecimals(decimals);
    spinBox->setSingleStep(decimals == 0 ? 1.0 : 0.1);
    spinBox->setSuffix(suffix);
    spinBox->setMinimumWidth(94);
    return spinBox;
}

} // namespace

InstructionEditorWidget::InstructionEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("instructionEditor"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto *header = new QHBoxLayout;
    header->setContentsMargins(0, 0, 0, 0);
    typeLabel_ = new QLabel(QStringLiteral("Select an instruction"), this);
    typeLabel_->setObjectName(QStringLiteral("instructionHeading"));
    header->addWidget(typeLabel_);
    header->addStretch();
    auto *applyButton = new QPushButton(QStringLiteral("APPLY"), this);
    applyButton->setObjectName(QStringLiteral("secondaryButton"));
    applyButton->setMinimumHeight(26);
    header->addWidget(applyButton);
    layout->addLayout(header);

    pages_ = new QStackedWidget(this);
    pages_->addWidget(new QLabel(QStringLiteral("Select an instruction to edit its parameters."), this));

    auto *moveJPage = new QWidget(this);
    auto *moveJLayout = new QVBoxLayout(moveJPage);
    moveJLayout->setContentsMargins(0, 0, 0, 0);
    moveJLayout->setSpacing(5);
    moveJLayout->addWidget(sectionLabel(QStringLiteral("Joint Position"), moveJPage));
    auto *moveJGrid = new QGridLayout;
    moveJGrid->setContentsMargins(0, 0, 0, 0);
    moveJGrid->setHorizontalSpacing(12);
    moveJGrid->setVerticalSpacing(4);
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 2; ++column) {
            const int joint = row * 2 + column;
            moveJInputs_.append(addNumberField(moveJGrid, row, column * 2,
                                                QStringLiteral("J%1").arg(joint + 1),
                                                QStringLiteral(" °"), -360.0, 360.0, 1));
        }
    }
    moveJLayout->addLayout(moveJGrid);
    auto *moveJSpeed = new QHBoxLayout;
    moveJSpeed->setContentsMargins(0, 3, 0, 0);
    moveJSpeed->addWidget(fieldLabel(QStringLiteral("Speed"), moveJPage));
    auto *moveJSpeedInput = configureSpinBox(QStringLiteral(" %"), 0.0, 100.0, 0, moveJPage);
    moveJInputs_.append(moveJSpeedInput);
    moveJSpeed->addWidget(moveJSpeedInput);
    moveJSpeed->addStretch();
    moveJLayout->addLayout(moveJSpeed);
    pages_->addWidget(moveJPage);

    auto *moveLPage = new QWidget(this);
    auto *moveLLayout = new QVBoxLayout(moveLPage);
    moveLLayout->setContentsMargins(0, 0, 0, 0);
    moveLLayout->setSpacing(5);
    moveLLayout->addWidget(sectionLabel(QStringLiteral("Cartesian Position"), moveLPage));
    auto *moveLGrid = new QGridLayout;
    moveLGrid->setContentsMargins(0, 0, 0, 0);
    moveLGrid->setHorizontalSpacing(12);
    moveLGrid->setVerticalSpacing(4);
    const QStringList cartesianLabels = {QStringLiteral("X"), QStringLiteral("Y"), QStringLiteral("Z"),
                                         QStringLiteral("Rx"), QStringLiteral("Ry"), QStringLiteral("Rz")};
    for (int row = 0; row < 3; ++row) {
        moveLInputs_.append(addNumberField(moveLGrid, row, 0, cartesianLabels[row],
                                            QStringLiteral(" mm"), -10000.0, 10000.0, 1));
        moveLInputs_.append(addNumberField(moveLGrid, row, 2, cartesianLabels[row + 3],
                                            QStringLiteral(" °"), -360.0, 360.0, 1));
    }
    moveLLayout->addLayout(moveLGrid);
    auto *moveLSpeed = new QHBoxLayout;
    moveLSpeed->setContentsMargins(0, 3, 0, 0);
    moveLSpeed->addWidget(fieldLabel(QStringLiteral("Speed"), moveLPage));
    auto *moveLSpeedInput = configureSpinBox(QStringLiteral(" %"), 0.0, 100.0, 0, moveLPage);
    moveLInputs_.append(moveLSpeedInput);
    moveLSpeed->addWidget(moveLSpeedInput);
    moveLSpeed->addStretch();
    moveLLayout->addLayout(moveLSpeed);
    pages_->addWidget(moveLPage);

    auto *waitPage = new QWidget(this);
    auto *waitLayout = new QHBoxLayout(waitPage);
    waitLayout->setContentsMargins(0, 0, 0, 0);
    waitLayout->addWidget(fieldLabel(QStringLiteral("Duration"), waitPage));
    auto *waitInput = configureSpinBox(QStringLiteral(" s"), 0.0, 3600.0, 1, waitPage);
    waitInputs_.append(waitInput);
    waitLayout->addWidget(waitInput);
    waitLayout->addStretch();
    pages_->addWidget(waitPage);

    auto *setIoPage = new QWidget(this);
    auto *setIoLayout = new QGridLayout(setIoPage);
    setIoLayout->setContentsMargins(0, 0, 0, 0);
    setIoLayout->setHorizontalSpacing(12);
    setIoLayout->setVerticalSpacing(5);
    setIoLayout->addWidget(fieldLabel(QStringLiteral("Output"), setIoPage), 0, 0);
    outputSelector_ = new QComboBox(setIoPage);
    outputSelector_->setObjectName(QStringLiteral("instructionSelector"));
    outputSelector_->addItems({QStringLiteral("DO1"), QStringLiteral("DO2"), QStringLiteral("DO3"),
                               QStringLiteral("DO4")});
    setIoLayout->addWidget(outputSelector_, 0, 1);
    setIoLayout->addWidget(fieldLabel(QStringLiteral("State"), setIoPage), 1, 0);
    stateSelector_ = new QComboBox(setIoPage);
    stateSelector_->setObjectName(QStringLiteral("instructionSelector"));
    stateSelector_->addItems({QStringLiteral("OFF"), QStringLiteral("ON")});
    setIoLayout->addWidget(stateSelector_, 1, 1);
    setIoLayout->setColumnStretch(1, 1);
    pages_->addWidget(setIoPage);

    auto *commentPage = new QWidget(this);
    auto *commentLayout = new QHBoxLayout(commentPage);
    commentLayout->setContentsMargins(0, 0, 0, 0);
    commentLayout->addWidget(fieldLabel(QStringLiteral("Text"), commentPage));
    commentInput_ = new QLineEdit(commentPage);
    commentInput_->setObjectName(QStringLiteral("instructionInput"));
    commentInput_->setPlaceholderText(QStringLiteral("Enter a comment"));
    commentLayout->addWidget(commentInput_, 1);
    pages_->addWidget(commentPage);

    auto *callPage = new QWidget(this);
    auto *callLayout = new QHBoxLayout(callPage);
    callLayout->setContentsMargins(0, 0, 0, 0);
    callLayout->addWidget(fieldLabel(QStringLiteral("Program"), callPage));
    programSelector_ = new QComboBox(callPage);
    programSelector_->setObjectName(QStringLiteral("instructionSelector"));
    programSelector_->addItems({QStringLiteral("Pick & Place"), QStringLiteral("Test Motion"),
                                QStringLiteral("Demo Program")});
    callLayout->addWidget(programSelector_, 1);
    pages_->addWidget(callPage);
    layout->addWidget(pages_);

    auto notify = [this] { emitChanged(); };
    auto notifyInput = [this](QDoubleSpinBox *input) {
        connect(input, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
                [this] { emitChanged(); });
    };
    for (QDoubleSpinBox *input : moveJInputs_) {
        notifyInput(input);
    }
    for (QDoubleSpinBox *input : moveLInputs_) {
        notifyInput(input);
    }
    for (QDoubleSpinBox *input : waitInputs_) {
        notifyInput(input);
    }
    connect(outputSelector_, &QComboBox::currentTextChanged, this, notify);
    connect(stateSelector_, &QComboBox::currentTextChanged, this, notify);
    connect(commentInput_, &QLineEdit::textChanged, this, notify);
    connect(programSelector_, &QComboBox::currentTextChanged, this, notify);
    connect(applyButton, &QPushButton::clicked, this, &InstructionEditorWidget::emitChanged);

    setEnabled(false);
}

QDoubleSpinBox *InstructionEditorWidget::addNumberField(QGridLayout *layout, int row, int column,
                                                         const QString &label, const QString &suffix,
                                                         double minimum, double maximum, int decimals)
{
    auto *labelWidget = fieldLabel(label, this);
    layout->addWidget(labelWidget, row, column);
    auto *input = configureSpinBox(suffix, minimum, maximum, decimals, this);
    layout->addWidget(input, row, column + 1);
    return input;
}

void InstructionEditorWidget::setInstruction(const QString &type,
                                              const InstructionParameters &parameters)
{
    updating_ = true;
    currentType_ = type;
    typeLabel_->setText(type.isEmpty() ? QStringLiteral("Select an instruction") : type);
    if (type == QStringLiteral("MOVEJ")) {
        pages_->setCurrentIndex(MoveJPage);
    } else if (type == QStringLiteral("MOVEL")) {
        pages_->setCurrentIndex(MoveLPage);
    } else if (type == QStringLiteral("WAIT")) {
        pages_->setCurrentIndex(WaitPage);
    } else if (type == QStringLiteral("SET_IO")) {
        pages_->setCurrentIndex(SetIoPage);
    } else if (type == QStringLiteral("COMMENT")) {
        pages_->setCurrentIndex(CommentPage);
    } else if (type == QStringLiteral("CALL")) {
        pages_->setCurrentIndex(CallPage);
    } else {
        pages_->setCurrentIndex(EmptyPage);
    }

    QVector<QDoubleSpinBox *> *inputs = nullptr;
    if (type == QStringLiteral("MOVEJ")) {
        inputs = &moveJInputs_;
    } else if (type == QStringLiteral("MOVEL")) {
        inputs = &moveLInputs_;
    } else if (type == QStringLiteral("WAIT")) {
        inputs = &waitInputs_;
    }
    if (inputs != nullptr) {
        for (int index = 0; index < inputs->size(); ++index) {
            const double value = index < parameters.numericValues.size()
                                     ? parameters.numericValues[index]
                                     : 0.0;
            inputs->at(index)->setValue(value);
        }
    }
    outputSelector_->setCurrentText(parameters.output);
    stateSelector_->setCurrentText(parameters.state);
    commentInput_->setText(parameters.text);
    programSelector_->setCurrentText(parameters.program);
    setEnabled(!type.isEmpty());
    updating_ = false;
}

InstructionParameters InstructionEditorWidget::currentParameters() const
{
    InstructionParameters parameters;
    const QVector<QDoubleSpinBox *> *inputs = nullptr;
    if (currentType_ == QStringLiteral("MOVEJ")) {
        inputs = &moveJInputs_;
    } else if (currentType_ == QStringLiteral("MOVEL")) {
        inputs = &moveLInputs_;
    } else if (currentType_ == QStringLiteral("WAIT")) {
        inputs = &waitInputs_;
    }
    if (inputs != nullptr) {
        for (QDoubleSpinBox *input : *inputs) {
            parameters.numericValues.append(input->value());
        }
    }
    parameters.output = outputSelector_->currentText();
    parameters.state = stateSelector_->currentText();
    parameters.text = commentInput_->text();
    parameters.program = programSelector_->currentText();
    return parameters;
}

QString InstructionEditorWidget::currentSummary(const InstructionParameters &parameters) const
{
    if (currentType_ == QStringLiteral("MOVEJ")) {
        return QStringLiteral("Target 01");
    }
    if (currentType_ == QStringLiteral("MOVEL")) {
        return QStringLiteral("Target 01");
    }
    if (currentType_ == QStringLiteral("WAIT")) {
        return QStringLiteral("%1 s").arg(parameters.numericValues.value(0), 0, 'f', 1);
    }
    if (currentType_ == QStringLiteral("SET_IO")) {
        return QStringLiteral("%1 %2").arg(parameters.output, parameters.state);
    }
    if (currentType_ == QStringLiteral("COMMENT")) {
        return parameters.text.isEmpty() ? QStringLiteral("Add a note") : parameters.text.left(28);
    }
    if (currentType_ == QStringLiteral("CALL")) {
        return parameters.program;
    }
    return QString();
}

void InstructionEditorWidget::emitChanged()
{
    if (!updating_ && !currentType_.isEmpty()) {
        const InstructionParameters parameters = currentParameters();
        emit parametersChanged(parameters, currentSummary(parameters));
    }
}

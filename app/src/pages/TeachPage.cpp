#include "TeachPage.h"

#include "ui/widgets/Panel.h"
#include "ui/widgets/RobotViewWidget.h"
#include "ui/widgets/ValueRow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

TeachPage::TeachPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("contentPage"));

    waypoints_ = {{QStringLiteral("Home")}, {QStringLiteral("Pick Approach")},
                  {QStringLiteral("Pick")}, {QStringLiteral("Place")}};

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 4, 48, 4);
    layout->setSpacing(4);

    auto *titleLabel = new QLabel(QStringLiteral("Teach"), this);
    titleLabel->setObjectName(QStringLiteral("pageTitle"));
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(
        QStringLiteral("Teach and capture robot positions."), this);
    descriptionLabel->setObjectName(QStringLiteral("pageDescription"));
    layout->addWidget(descriptionLabel);
    layout->addSpacing(4);

    auto *workspace = new QHBoxLayout;
    workspace->setSpacing(14);
    layout->addLayout(workspace, 1);

    auto *leftColumn = new QWidget(this);
    leftColumn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    auto *leftLayout = new QVBoxLayout(leftColumn);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(8);

    auto *posePanel = new Panel(QStringLiteral("CURRENT POSE"), leftColumn);
    posePanel->setCompact(true);
    posePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    selectedWaypointLabel_ = new QLabel(QStringLiteral("Placeholder pose | Selected: --"), posePanel);
    selectedWaypointLabel_->setObjectName(QStringLiteral("poseContext"));
    posePanel->contentLayout()->addWidget(selectedWaypointLabel_);

    auto *poseGrid = new QGridLayout;
    poseGrid->setContentsMargins(0, 0, 0, 0);
    poseGrid->setHorizontalSpacing(18);
    poseGrid->setVerticalSpacing(2);
    const QVector<QString> jointLabels = {QStringLiteral("J1"), QStringLiteral("J2"),
                                          QStringLiteral("J3"), QStringLiteral("J4"),
                                          QStringLiteral("J5"), QStringLiteral("J6")};
    for (int index = 0; index < jointLabels.size(); ++index) {
        poseGrid->addWidget(new ValueRow(jointLabels.at(index), QStringLiteral("--.-°"), posePanel),
                            index, 0);
    }
    const QVector<QString> cartesianLabels = {QStringLiteral("X"), QStringLiteral("Y"),
                                              QStringLiteral("Z"), QStringLiteral("Rx"),
                                              QStringLiteral("Ry"), QStringLiteral("Rz")};
    const QVector<QString> cartesianValues = {QStringLiteral("-- mm"), QStringLiteral("-- mm"),
                                              QStringLiteral("-- mm"), QStringLiteral("-- °"),
                                              QStringLiteral("-- °"), QStringLiteral("-- °")};
    for (int index = 0; index < cartesianLabels.size(); ++index) {
        poseGrid->addWidget(new ValueRow(cartesianLabels.at(index), cartesianValues.at(index), posePanel),
                            index, 1);
    }
    posePanel->contentLayout()->addLayout(poseGrid);
    leftLayout->addWidget(posePanel);

    auto *capturePanel = new Panel(QStringLiteral("CAPTURE WAYPOINT"), leftColumn);
    capturePanel->setCompact(true);
    capturePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto *nameRow = new QHBoxLayout;
    nameRow->setContentsMargins(0, 0, 0, 0);
    nameRow->setSpacing(12);
    auto *nameLabel = new QLabel(QStringLiteral("Name"), capturePanel);
    nameLabel->setObjectName(QStringLiteral("valueLabel"));
    nameRow->addWidget(nameLabel);
    waypointNameInput_ = new QLineEdit(capturePanel);
    waypointNameInput_->setObjectName(QStringLiteral("waypointNameInput"));
    waypointNameInput_->setPlaceholderText(QStringLiteral("Waypoint name"));
    nameRow->addWidget(waypointNameInput_, 1);
    capturePanel->contentLayout()->addLayout(nameRow);

    auto *captureButton = new QPushButton(QStringLiteral("CAPTURE POSE"), capturePanel);
    captureButton->setObjectName(QStringLiteral("secondaryButton"));
    captureButton->setMinimumHeight(30);
    capturePanel->contentLayout()->addWidget(captureButton);
    leftLayout->addWidget(capturePanel);

    auto *waypointsPanel = new Panel(QStringLiteral("WAYPOINTS"), leftColumn);
    waypointsPanel->setCompact(true);
    waypointsPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    waypointList_ = new QListWidget(waypointsPanel);
    waypointList_->setObjectName(QStringLiteral("waypointList"));
    waypointList_->setSelectionMode(QAbstractItemView::SingleSelection);
    waypointList_->setUniformItemSizes(true);
    waypointsPanel->contentLayout()->addWidget(waypointList_, 1);

    auto *actionRow = new QHBoxLayout;
    actionRow->setContentsMargins(0, 2, 0, 0);
    actionRow->setSpacing(8);
    auto *renameButton = new QPushButton(QStringLiteral("RENAME"), waypointsPanel);
    renameButton->setObjectName(QStringLiteral("secondaryButton"));
    renameButton->setEnabled(false);
    actionRow->addWidget(renameButton);
    auto *deleteButton = new QPushButton(QStringLiteral("DELETE"), waypointsPanel);
    deleteButton->setObjectName(QStringLiteral("stopButton"));
    deleteButton->setEnabled(false);
    actionRow->addWidget(deleteButton);
    actionRow->addStretch();
    waypointsPanel->contentLayout()->addLayout(actionRow);
    leftLayout->addWidget(waypointsPanel, 1);
    workspace->addWidget(leftColumn, 3);

    auto *robotViewPanel = new Panel(QStringLiteral("ROBOT VIEW"), this);
    robotViewPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    robotViewPanel->contentLayout()->addWidget(new RobotViewWidget(robotViewPanel), 1);
    workspace->addWidget(robotViewPanel, 7);

    connect(captureButton, &QPushButton::clicked, this, &TeachPage::captureWaypoint);
    connect(waypointList_, &QListWidget::currentRowChanged, this,
            [this, renameButton, deleteButton](int index) {
                selectWaypoint(index);
                const bool hasSelection = index >= 0;
                renameButton->setEnabled(hasSelection);
                deleteButton->setEnabled(hasSelection);
            });
    connect(renameButton, &QPushButton::clicked, this, &TeachPage::renameWaypoint);
    connect(deleteButton, &QPushButton::clicked, this, &TeachPage::deleteWaypoint);

    refreshWaypointList();
    waypointList_->setCurrentRow(0);
}

void TeachPage::refreshWaypointList()
{
    waypointList_->clear();
    for (const Waypoint &waypoint : waypoints_) {
        waypointList_->addItem(waypoint.name);
    }
}

void TeachPage::selectWaypoint(int index)
{
    selectedWaypoint_ = index;
    const QString selectedName = index >= 0 && index < waypoints_.size()
                                     ? waypoints_.at(index).name
                                     : QStringLiteral("--");
    selectedWaypointLabel_->setText(QStringLiteral("Placeholder pose | Selected: %1")
                                        .arg(selectedName));
}

void TeachPage::captureWaypoint()
{
    QString name = waypointNameInput_->text().trimmed();
    if (name.isEmpty()) {
        name = QStringLiteral("Waypoint %1").arg(waypoints_.size() + 1);
    }
    waypoints_.append({name});
    refreshWaypointList();
    waypointList_->setCurrentRow(waypoints_.size() - 1);
    waypointNameInput_->clear();
}

void TeachPage::renameWaypoint()
{
    if (selectedWaypoint_ < 0 || selectedWaypoint_ >= waypoints_.size()) {
        return;
    }
    const int index = selectedWaypoint_;
    bool accepted = false;
    const QString name = QInputDialog::getText(
                             this, QStringLiteral("Rename Waypoint"), QStringLiteral("Name"),
                             QLineEdit::Normal, waypoints_.at(index).name, &accepted)
                             .trimmed();
    if (!accepted || name.isEmpty()) {
        return;
    }
    waypoints_[index].name = name;
    refreshWaypointList();
    waypointList_->setCurrentRow(index);
}

void TeachPage::deleteWaypoint()
{
    if (selectedWaypoint_ < 0 || selectedWaypoint_ >= waypoints_.size()) {
        return;
    }
    const int index = selectedWaypoint_;
    waypoints_.removeAt(index);
    refreshWaypointList();
    if (waypoints_.isEmpty()) {
        selectWaypoint(-1);
        return;
    }
    waypointList_->setCurrentRow(qMin(index, waypoints_.size() - 1));
}

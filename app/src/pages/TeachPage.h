#ifndef LEBOWSKI_TEACHPAGE_H
#define LEBOWSKI_TEACHPAGE_H

#include <QVector>
#include <QWidget>
#include <QString>

class QLabel;
class QListWidget;
class QLineEdit;

class TeachPage final : public QWidget
{
    Q_OBJECT

public:
    explicit TeachPage(QWidget *parent = nullptr);

private:
    struct Waypoint {
        QString name;
    };

    void refreshWaypointList();
    void selectWaypoint(int index);
    void captureWaypoint();
    void renameWaypoint();
    void deleteWaypoint();

    QVector<Waypoint> waypoints_;
    int selectedWaypoint_ = -1;
    QLineEdit *waypointNameInput_ = nullptr;
    QListWidget *waypointList_ = nullptr;
    QLabel *selectedWaypointLabel_ = nullptr;
};

#endif // LEBOWSKI_TEACHPAGE_H

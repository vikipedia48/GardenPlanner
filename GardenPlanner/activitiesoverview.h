#ifndef ACTIVITIESOVERVIEW_H
#define ACTIVITIESOVERVIEW_H

#include "activity.h"
#include <QWidget>

namespace Ui {
class ActivitiesOverview;
}

class ActivitiesOverview : public QWidget
{
    Q_OBJECT

public:
    explicit ActivitiesOverview(QWidget *parent = nullptr);
    ActivitiesOverview(const std::vector<Activity>& allActivities, const QDate& day, QWidget *parent = nullptr);
    ~ActivitiesOverview();

    void addNewRowToTable(std::array<QString,5> items);

private:
    Ui::ActivitiesOverview *ui;
};

#endif // ACTIVITIESOVERVIEW_H

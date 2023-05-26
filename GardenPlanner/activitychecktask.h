#ifndef ACTIVITYCHECKTASK_H
#define ACTIVITYCHECKTASK_H

#include "activity.h"

#include <QObject>
#include <QRunnable>
#include <bitset>
#include <deque>

class ActivityCheckTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ActivityCheckTask();
    ActivityCheckTask(const Activity& activity, int index, const QDate& day, std::array<QString,5>* outputActivities);
    Activity activity;
    int index;
    QDate day;
    std::array<QString,5>* outputActivities;


signals:
    void sendThreadFinished();

    // QRunnable interface
public:
    void run() override;
};

#endif // ACTIVITYCHECKTASK_H

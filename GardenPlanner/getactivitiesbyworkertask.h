#ifndef GETACTIVITIESBYWORKERTASK_H
#define GETACTIVITIESBYWORKERTASK_H

#include "activity.h"
#include <QRunnable>



class GetActivitiesByWorkerTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    GetActivitiesByWorkerTask();
    GetActivitiesByWorkerTask(const Worker& worker, const std::vector<Activity>& activities,
                              QString* outputHtml, QDate filterByMonth = QDate());/*
                              : worker(worker), activities(activities), activitiesOutput(outputActivities), filterByMonth(filterByMonth) {

    }*/
    Worker worker;
    QDate filterByMonth;
    std::vector<Activity> activities;
    QString* outputHtml;
signals:
    void sendThreadFinished();


    // QRunnable interface
public:
    void run() override;
};

#endif // GETACTIVITIESBYWORKERTASK_H

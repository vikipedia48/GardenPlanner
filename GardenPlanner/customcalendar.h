#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H

#include <QCalendarWidget>
#include <QObject>
#include <QWidget>
#include <vector>
#include <map>
#include <bitset>
#include "activitiesoverview.h"
#include "activity.h"

class CustomCalendar : public QCalendarWidget
{
    Q_OBJECT
public:
    CustomCalendar(QWidget* parent = nullptr);
    CustomCalendar(const std::vector<Activity>& activities, QWidget* parent = nullptr);
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;
    void refresh(const std::vector<Activity>& newActivities);

    std::bitset<31> activeDays;
    std::vector<Activity> activities;
private slots:
    void receiveLoadActivities(QDate date);
private:
    ActivitiesOverview* overviewWindow;
};

#endif // CUSTOMCALENDAR_H

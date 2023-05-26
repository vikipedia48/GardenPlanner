#include "customcalendar.h"
#include "commonfunctions.h"
#include <QPainter>

CustomCalendar::CustomCalendar(QWidget* parent) : QCalendarWidget(parent)
{
    connect(this, SIGNAL(activated(QDate)), this, SLOT(receiveLoadActivities(QDate)));
}

CustomCalendar::CustomCalendar(const std::vector<Activity>& activities, QWidget *parent) : activities(activities), QCalendarWidget(parent)
{
    connect(this, SIGNAL(activated(QDate)), this, SLOT(receiveLoadActivities(QDate)));

}


void CustomCalendar::paintCell(QPainter *painter, const QRect &rect, QDate date) const
{
    painter->save();
    bool yes = std::any_of(begin(activities), end(activities), [&date](const auto& a) { return Util::isActivityInThisDay(a, date); });
    if (yes) painter->fillRect(rect, QBrush(QColor(255,30,60)));
    painter->setPen(QColor(0,0,0));
    painter->drawText(rect, Qt::TextSingleLine | Qt::AlignCenter, QString::number(date.day()));
    painter->restore();
}

void CustomCalendar::refresh(const std::vector<Activity>& newActivities)
{
    this->activities = newActivities;
    this->updateCells();
}

void CustomCalendar::receiveLoadActivities(QDate date)
{
    overviewWindow = new ActivitiesOverview(activities, date);
    overviewWindow->show();
}

#include "activitychecktask.h"
#include "commonfunctions.h"

ActivityCheckTask::ActivityCheckTask()
{

}

ActivityCheckTask::ActivityCheckTask(const Activity &activity, int index, const QDate &day, std::array<QString,5>* outputActivities) : activity(activity), index(index), day(day), outputActivities(outputActivities)
{
    this->setAutoDelete(true);
}

void ActivityCheckTask::run()
{
    if (Util::isActivityInThisDay(activity,day)) {
        auto plants = Util::getPlantList();
        *outputActivities = {QString::fromStdString(plants[activity.id-1].name), activity.date.toString(), QString::number(activity.duration), QString::number(activity.repeat), QString::fromStdString(activity.description)};
    }
    emit sendThreadFinished();
}

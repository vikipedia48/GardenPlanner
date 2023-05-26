#include "getactivitiesbyworkertask.h"
#include "commonfunctions.h"
#include "fmt/format.h"

GetActivitiesByWorkerTask::GetActivitiesByWorkerTask()
{

}
GetActivitiesByWorkerTask::GetActivitiesByWorkerTask(const Worker& worker, const std::vector<Activity>& activities, QString* outputHtml, QDate filterByMonth)
                          : worker(worker), activities(activities), outputHtml(outputHtml), filterByMonth(filterByMonth) {
    this->setAutoDelete(true);
}

void GetActivitiesByWorkerTask::run()
{
    std::vector<Activity> list;

    for (auto& v : activities) {
        if (!filterByMonth.isNull()) {
            if (!Util::isActivityInThisMonth(v, filterByMonth)) continue;
        }
        if (std::find(v.workersAssigned.begin(), v.workersAssigned.end(), worker) != v.workersAssigned.end()) {
            list.push_back(v);
        }
    }
    if (!list.empty()) {
        auto rv = QString::fromStdString(fmt::format(
        "<div align=left>"
            "<h3><b>{}</b></h3>"
            "<ul>", worker.name));
        for (auto& v : list) {
            auto temp = QString::fromStdString(fmt::format(
                "<li>{}: {}</li>", v.plant.name, v.description));
            rv+=temp;
        }
        rv += "</ul></div>";
        *outputHtml = rv;
    }

    emit sendThreadFinished();
}

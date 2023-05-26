#include "activitiesoverview.h"
#include "ui_activitiesoverview.h"
#include "translationstrings.h"
#include "activitychecktask.h"
#include "barrier.h"
#include <QThreadPool>
#include <bitset>
#include <deque>

ActivitiesOverview::ActivitiesOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivitiesOverview)
{
    ui->setupUi(this);
}

ActivitiesOverview::ActivitiesOverview(const std::vector<Activity> &allActivities, const QDate &day, QWidget *parent) : QWidget(parent), ui(new Ui::ActivitiesOverview)
{
    auto size = allActivities.size();
    auto scheduledActivities = std::vector<std::array<QString,5>*>(size);
    auto barrier = Barrier(size);
    for (auto i = 0; i < size; ++i) {
        scheduledActivities[i] = new std::array<QString,5>();
        auto task = new ActivityCheckTask(allActivities[i], i, day, scheduledActivities[i]);

        auto on_finished = [barrier = barrier]() mutable { barrier.arrive(); };
        connect(task, &ActivityCheckTask::sendThreadFinished, on_finished);
        QThreadPool::globalInstance()->start(task);
    }
    ui->setupUi(this);
    ui->label->setText(Translate::activitiesScheduledForThisDate()+": "+day.toString());
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(Translate::plant()));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(Translate::date()));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(Translate::durationDays()));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(Translate::repeatDays()));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(Translate::description()));

    barrier.wait();

    for (auto i = 0; i < size; ++i) {
        addNewRowToTable(*scheduledActivities[i]);
        delete scheduledActivities[i];
    }
}

ActivitiesOverview::~ActivitiesOverview()
{
    delete ui;
}

void ActivitiesOverview::addNewRowToTable(std::array<QString, 5> items)
{
    auto rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(items[0]));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(items[1]));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(items[2]));
    ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(items[3]));
    ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(items[4]));
}


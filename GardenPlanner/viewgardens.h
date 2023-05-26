#ifndef VIEWGARDENS_H
#define VIEWGARDENS_H

#include "addactivitydialog.h"
#include "customcalendar.h"
#include "garden.h"

#include <QNetworkReply>
#include <QWidget>

namespace Ui {
class ViewGardens;
}

class ViewGardens : public QWidget
{
    Q_OBJECT

public:
    explicit ViewGardens(std::int64_t userId, const QByteArray& json, QWidget *parent = nullptr);
    ~ViewGardens();
signals:
    void sendRequestForGardens();
    void sendEditGarden(std::int64_t gardenId);
    void sendRequestForActivities(std::int64_t gardenId);
    void sendRequestForGardenDeletion(std::int64_t gardenId);
    void sendRequestForActivityDeletion(std::int64_t activityId);
    void sendActivity(const Activity& activity);
    void sendRequestForUsersWorkers(std::int64_t userId);
    void sendRequestForUsersWorkersForReport(std::int64_t userId);
    void sendWorkers(const std::vector<Worker>& workers);
    void sendWorker(const QString& name);
    void sendWorkerForward(const Worker& worker);

private slots:
    void on_comboBox_currentGarden_currentIndexChanged(int index);
    void on_pushButton_addActivity_clicked();
    void on_pushButton_editActivity_clicked();
    void on_pushButton_deleteActivity_clicked();
    void on_pushButton_printReport_clicked();
    void on_pushButton_editGarden_clicked();
    void on_pushButton_loadActivities_clicked();
    void on_pushButton_deleteGarden_clicked();

    void receiveActivityCreate(const Activity& activity);
    void receiveActivities(const std::vector<Activity>& activities);
    void receiveActivityEdit(const Activity& activity);
    void receiveUsersWorkers(const std::vector<Worker>& workers);
    void receiveUsersWorkersForReport(const std::vector<Worker>& workers);
    void receiveRequestForWorkers(std::int64_t userId);
    void receiveWorker(const QString& name);
    void receiveWorkerForward(const Worker& worker);


private:
    Ui::ViewGardens *ui;
    CustomCalendar* calendar;
    AddActivityDialog* newActivity;
    std::vector<Garden> gardenList;
    std::int64_t userId;

    std::int64_t _indexForPrint;

    void deselectGarden(bool deselect);
    void loadUi();
    void loadGardensComboBox();
    int getSelectedGardenIndex();
    QString getMonthName(int n);
    void printReport(const std::vector<Worker>& workers, const Garden& garden);
    std::vector<Worker> parseWorkersJson(const QByteArray& bytes);
};

#endif // VIEWGARDENS_H

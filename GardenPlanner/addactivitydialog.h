#ifndef ADDACTIVITYDIALOG_H
#define ADDACTIVITYDIALOG_H

#include "activity.h"
#include "garden.h"
#include "waitscreen.h"
#include "worker.h"
#include <QNetworkReply>
#include <QWidget>

namespace Ui {
class AddActivityDialog;
}

class AddActivityDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddActivityDialog(std::int64_t userId, const Garden& garden, QWidget *parent = nullptr);
    AddActivityDialog(std::int64_t userId, const Garden& garden, const Activity& activityToBeEdited, QWidget *parent = nullptr);
    ~AddActivityDialog();
signals:
    void sendActivity(const Activity& activity);
    void sendRequestForWorkers(std::int64_t userId);
    void sendStopWaiting();
    void sendWorker(const QString& name);

private slots:

    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_add_clicked();
    void on_comboBox_workers_currentIndexChanged(int index);
    void on_pushButton_remove_clicked();
    void on_pushButton_loadWorkers_clicked();

    void receiveWorkers(const std::vector<Worker>& workers);
    void receiveWorker(const Worker& worker);

private:
    Ui::AddActivityDialog *ui;
    WaitScreen* waitScreen;
    Activity activityBeingAdded;
    std::vector<Worker> importedWorkers;
    std::vector<Plant> importedPlants;
    int userId;

    void loadUi();
};

#endif // ADDACTIVITYDIALOG_H

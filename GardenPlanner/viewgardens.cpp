#include "viewgardens.h"
#include "commonfunctions.h"
#include "cryptography.h"
#include "customcalendar.h"
#include "fmt/format.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_viewgardens.h"
#include "getactivitiesbyworkertask.h"
#include "barrier.h"
#include "blobtoplantlisttask.h"
#include <QThreadPool>
#include <QPrinter>
#include <QJsonParseError>

ViewGardens::ViewGardens(std::int64_t userId, const QByteArray& json, QWidget *parent) :
    QWidget(parent), userId(userId)
{
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(json, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Gui::ThrowError(Translate::errorReadingJson());
        goto end;
    }
    if (!document.isArray()) {
        Gui::ThrowError(Translate::errorReadingJson());
    }
    else {
        auto jsonArray = document.array();
        auto size = jsonArray.size();
        gardenList = std::vector<Garden>(size);
        auto barrier = Barrier(size);
        auto on_finished = [barrier = barrier]() mutable { barrier.arrive(); };
        for (auto i = 0; i < jsonArray.size(); ++i) {
            auto jsonObj = jsonArray[i].toObject();
            auto jsonPlants = QByteArray::fromBase64(jsonObj.value("plants").toString().toLocal8Bit());
            auto task = new BlobToPlantListTask(jsonPlants, std::addressof(gardenList[i].plants));
            connect(task, &BlobToPlantListTask::sendThreadFinished, on_finished);
            QThreadPool::globalInstance()->start(task);

            gardenList[i].id = jsonObj.value("id").toInteger();
            gardenList[i].name = jsonObj.value("name").toString().toStdString();
            gardenList[i].userId = jsonObj.value("user_id").toInteger();
        }
        barrier.wait();
    }
    end:;
    loadUi();
    calendar = new CustomCalendar();
    ui->calendarWindow->addWidget(calendar, 0, Qt::AlignmentFlag::AlignVCenter);
    calendar->show();
}

void ViewGardens::receiveActivities(const std::vector<Activity> &activities)
{
    // sve su aktivnosti od istog vrta, stoga:
    auto gardenId = activities[0].gardenId;
    for (auto i = 0; i < gardenList.size(); ++i) {
        if (gardenList[i].id == gardenId) {
            ui->listWidget_activities->clear();
            gardenList[i].activities = activities;
            for (auto& v : gardenList[i].activities) {
                QString label;
                label = QString::fromStdString(v.plant.name + " - " + v.description);
                ui->listWidget_activities->addItem(label);
            }
            break;
        }
    }
    calendar->refresh(activities);
}

void ViewGardens::deselectGarden(bool deselect)
{
    ui->pushButton_addActivity->setEnabled(!deselect);
    ui->pushButton_deleteActivity->setEnabled(!deselect);
    ui->pushButton_editActivity->setEnabled(!deselect);
    ui->pushButton_printReport->setEnabled(!deselect);
    ui->pushButton_editGarden->setEnabled(!deselect);
    ui->pushButton_deleteGarden->setEnabled(!deselect);
    ui->listWidget_plants->clear();
    ui->listWidget_activities->clear();
}

void ViewGardens::loadUi()
{
    ui = new Ui::ViewGardens;
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_activities->setFont(titleFont);
    ui->label_viewing->setText(Translate::viewing());
    ui->pushButton_editGarden->setText(Translate::editGarden());
    ui->pushButton_deleteGarden->setText(Translate::deleteGarden());
    ui->label_activities->setText(Translate::activities());
    ui->pushButton_addActivity->setText(Translate::add());
    ui->pushButton_editActivity->setText(Translate::viewSlashEdit());
    ui->pushButton_deleteActivity->setText(Translate::delete_());
    ui->pushButton_printReport->setText(Translate::makeMonthlyReport());
    ui->pushButton_loadActivities->setText(Translate::load());
    loadGardensComboBox();
}

void ViewGardens::loadGardensComboBox()
{
    ui->comboBox_currentGarden->clear();
    ui->comboBox_currentGarden->addItem(Translate::select());
    ui->comboBox_currentGarden->setCurrentIndex(0);
    deselectGarden(true);

    for (auto& v : gardenList) {
        ui->comboBox_currentGarden->addItem(QString::fromStdString(v.name));
    }
}

int ViewGardens::getSelectedGardenIndex()
{
    auto index = ui->comboBox_currentGarden->currentIndex();
    if (index == 0) {
        Gui::ThrowError(Translate::gardenNotSelected());
        return 0;
    }
    return index;
}

QString ViewGardens::getMonthName(int n)
{
    switch(n) {
        case 1: return Translate::january();
        case 2: return Translate::february();
        case 3: return Translate::march();
        case 4: return Translate::april();
        case 5: return Translate::may();
        case 6: return Translate::june();
        case 7: return Translate::july();
        case 8: return Translate::august();
        case 9: return Translate::september();
        case 10: return Translate::october();
        case 11: return Translate::november();
        case 12: return Translate::december();
        default: return "";
    }
}

void ViewGardens::printReport(const std::vector<Worker>& workers, const Garden& garden)
{
    if (workers.empty()) {
        Gui::ThrowError(Translate::error());
        return;
    }
    auto _year = Gui::GetNumberValueFromInputDialog(Translate::date(), Translate::enterYear());
    if (_year < QDate::currentDate().year()) {
        Gui::ThrowError(Translate::invalidInput());
        return;
    }
    auto _month = Gui::GetNumberValueFromInputDialog(Translate::date(), Translate::enterMonth());
    if (_month < 1 || _month > 12) {
        Gui::ThrowError(Translate::invalidInput());
        return;
    }

    auto year = (int)(_year);
    auto month = (int)_month;
    auto date = QDate(year, month, 1);
    QString html = QString::fromStdString(fmt::format(
                "<div align=justify>"
                    "<h1>{}</h1><br><h2>{}, {}</h2><br>"
                "</div>",garden.name,getMonthName(month).toStdString(),year));

    // fetchaj activityje od svakog radnika multithreaded
    auto size = workers.size();
    auto barrier = Barrier(size);
    std::vector<QString> detailContents(size);
    for (auto i = 0; i < workers.size(); ++i) {
        auto task = new GetActivitiesByWorkerTask(workers[i], garden.activities, std::addressof(detailContents[i]), date);
        auto on_finished = [barrier = barrier]() mutable { barrier.arrive(); };
        connect(task, &GetActivitiesByWorkerTask::sendThreadFinished, on_finished);
        QThreadPool::globalInstance()->start(task);
    }
    auto path = Gui::GetSavePath(Translate::pdfDocuments());
    if (path.isEmpty()) {
        Gui::ThrowError(Translate::error());
        goto end;
    }

    barrier.wait();
    for (auto& v : detailContents) html += v;
    {
        QTextDocument document;
        document.setHtml(html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPageSize(QPageSize::A4);
        printer.setOutputFileName(path);
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));
        document.print(&printer);
    }
    end:;
    QThreadPool::globalInstance()->clear();
}

std::vector<Worker> ViewGardens::parseWorkersJson(const QByteArray &bytes)
{
    std::vector<Worker> rv;
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Gui::ThrowError(Translate::errorReadingJson());
        return {};
    }
    if (!document.isArray()) {
        Gui::ThrowError(Translate::errorReadingJson());
        return {};
    }
    auto jsonArray = document.array();
    for (const auto& v : jsonArray) {
        auto jsonObj = v.toObject();
        rv.emplace_back(Worker(jsonObj.value("id").toInteger(), Cryptography::decryptRSA(jsonObj.value("name").toString().toUtf8())));
    }
    return rv;
}


ViewGardens::~ViewGardens()
{
    delete ui;
}

void ViewGardens::on_comboBox_currentGarden_currentIndexChanged(int index)
{
    if (index == 0) {
        deselectGarden(true);
        return;
    }
    deselectGarden(false);
    for (auto& v : gardenList[index-1].plants) {
        ui->listWidget_plants->addItem(QString::fromStdString(v.name));
    }
    for (auto& v : gardenList[index-1].activities) {
        ui->listWidget_activities->addItem(QString::fromStdString(v.plant.name + " - " + v.description));
    }
}

void ViewGardens::on_pushButton_addActivity_clicked()
{
    auto index = getSelectedGardenIndex();
    if (index == 0) return;
    auto thisGarden = gardenList[index-1];
    newActivity = new AddActivityDialog(thisGarden.userId, thisGarden);
    connect(newActivity, SIGNAL(sendActivity(const Activity&)), this, SLOT(receiveActivityCreate(const Activity&)));
    connect(newActivity, SIGNAL(sendRequestForWorkers(std::int64_t)), this, SLOT(receiveRequestForWorkers(std::int64_t)));
    connect(this, SIGNAL(sendWorkers(const std::vector<Worker>&)), newActivity, SLOT(receiveWorkers(const std::vector<Worker>&)));
    connect(newActivity, SIGNAL(sendWorker(const QString&)), this, SLOT(receiveWorker(const QString&)));
    connect(this, SIGNAL(sendWorkerForward(const Worker&)), newActivity, SLOT(receiveWorker(const Worker&)));
    newActivity->show();
}


void ViewGardens::on_pushButton_editActivity_clicked()
{
    auto selectedItems = ui->listWidget_activities->selectionModel()->selectedIndexes();
    if (selectedItems.size() != 1) {
        Gui::ThrowError(Translate::youMustSelectOneActivity());
        return;
    }
    auto index = getSelectedGardenIndex();

    auto thisGarden = gardenList[index-1];
    newActivity = new AddActivityDialog(thisGarden.userId, thisGarden, thisGarden.activities[selectedItems[0].row()]);
    connect(newActivity, SIGNAL(sendActivity(const Activity&)), this, SLOT(receiveActivityEdit(const Activity&)));
    connect(newActivity, SIGNAL(sendRequestForWorkers(std::int64_t)), this, SLOT(receiveRequestForWorkers(std::int64_t)));
    connect(this, SIGNAL(sendWorkers(const std::vector<Worker>&)), newActivity, SLOT(receiveWorkers(const std::vector<Worker>&)));
    connect(newActivity, SIGNAL(sendWorker(const QString&)), this, SLOT(receiveWorker(const QString&)));
    connect(this, SIGNAL(sendWorkerForward(const Worker&)), newActivity, SLOT(receiveWorker(const Worker&)));
    newActivity->show();
}

void ViewGardens::on_pushButton_deleteActivity_clicked()
{
    auto selectedItems = ui->listWidget_activities->selectionModel()->selectedIndexes();
    if (selectedItems.size() != 1) {
        Gui::ThrowError(Translate::youMustSelectOneActivity());
        return;
    }
    if (!Gui::GiveQuestion(Translate::areYouSureYouWantToDeleteThis())) return;
    auto index = getSelectedGardenIndex();
    auto activityId = gardenList[index-1].activities[selectedItems[0].row()].id;
    emit sendRequestForActivityDeletion(activityId);
    qDeleteAll(ui->listWidget_activities->selectedItems());
}


void ViewGardens::on_pushButton_printReport_clicked()
{
    auto index = getSelectedGardenIndex();
    if (index == 0) return;
    Gui::PrintMessage(Translate::makeMonthlyReport(), Translate::youMustLoadActivities());
    if (gardenList[index-1].activities.empty()) {
        Gui::ThrowError(Translate::noActivities());
        return;
    }
    _indexForPrint = index-1;
    emit sendRequestForUsersWorkersForReport(userId);
}

void ViewGardens::on_pushButton_editGarden_clicked()
{
    auto index = getSelectedGardenIndex();
    if (index == 0) return;
    emit sendEditGarden(gardenList[index-1].id);
}


void ViewGardens::on_pushButton_deleteGarden_clicked()
{
    auto index = getSelectedGardenIndex();
    if (index == 0) return;
    if (!Gui::GiveQuestion(Translate::areYouSureYouWantToDeleteThis())) return;
    emit sendRequestForGardenDeletion(gardenList[index-1].id);
}

void ViewGardens::receiveActivityCreate(const Activity &activity)
{
    auto index = getSelectedGardenIndex();
    gardenList[index-1].activities.push_back(activity);
    ui->listWidget_activities->addItem(QString::fromStdString(activity.plant.name + " - " + activity.description));
    calendar->refresh(gardenList[index-1].activities);
    emit sendActivity(activity);
}

void ViewGardens::receiveActivityEdit(const Activity &activity)
{
    auto index = getSelectedGardenIndex();
    auto indexActivity = ui->listWidget_activities->selectionModel()->selectedIndexes()[0].row();
    for (auto i = 0; i < gardenList[index-1].activities.size(); ++i) {
        if (gardenList[index-1].activities[i].id == activity.id) {
            gardenList[index-1].activities[i] = activity;
            ui->listWidget_activities->takeItem(i);
            ui->listWidget_activities->insertItem(i, QString::fromStdString(activity.plant.name + " - " + activity.description));
            break;
        }
    }
    calendar->refresh(gardenList[index-1].activities);
    emit sendActivity(activity);
}

void ViewGardens::receiveUsersWorkers(const std::vector<Worker>& workers)
{
    emit sendWorkers(workers);
}

void ViewGardens::receiveUsersWorkersForReport(const std::vector<Worker>& workers)
{
    if (!workers.empty()) {
        if (workers[0].id == -100) return;
    }
    printReport(workers, gardenList[_indexForPrint]);
}

void ViewGardens::receiveRequestForWorkers(std::int64_t userId)
{
    emit sendRequestForUsersWorkers(userId);
}

void ViewGardens::receiveWorker(const QString &name)
{
    emit sendWorker(name);
}

void ViewGardens::receiveWorkerForward(const Worker &worker)
{
    emit sendWorkerForward(worker);
}

void ViewGardens::on_pushButton_loadActivities_clicked()
{
    auto index = getSelectedGardenIndex();
    if (index == 0) return;
    emit sendRequestForActivities(gardenList[index-1].id);
}


#include "addactivitydialog.h"
#include "commonfunctions.h"
#include "cryptography.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_addactivitydialog.h"

AddActivityDialog::AddActivityDialog(int64_t userId, const Garden &garden, QWidget *parent) : QWidget(parent), userId(userId)
{
    activityBeingAdded = Activity();
    activityBeingAdded.gardenId = garden.id;
    importedPlants = garden.plants;
    loadUi();
}

AddActivityDialog::AddActivityDialog(int64_t userId, const Garden &garden, const Activity &activityToBeEdited, QWidget *parent) : QWidget(parent), userId(userId)
{
    activityBeingAdded = activityToBeEdited;
    importedPlants = garden.plants;
    loadUi();
    ui->dateEdit->setDate(activityToBeEdited.date);
    ui->lineEdit_duration->setText(QString::number(activityToBeEdited.duration));
    ui->lineEdit_repeat->setText(QString::number(activityToBeEdited.repeat));
    ui->textEdit_description->setText(QString::fromStdString(activityToBeEdited.description));
    auto selectedPlant = std::find(importedPlants.begin(), importedPlants.end(), activityToBeEdited.plant);
    ui->plantComboBox->setCurrentIndex(std::distance(importedPlants.begin(), selectedPlant));
    for (auto& v : activityBeingAdded.workersAssigned) ui->listWidget_workers->addItem(QString::fromStdString(v.name));
}

AddActivityDialog::~AddActivityDialog()
{
    delete ui;
}

void AddActivityDialog::loadUi()
{
    ui = new Ui::AddActivityDialog;
    ui->setupUi(this);
    for (auto& v : importedPlants) ui->plantComboBox->addItem(QString::fromStdString(v.name));
    this->setWindowModality(Qt::WindowModality::WindowModal);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_addActivity->setFont(titleFont);
    ui->lineEdit_addNewWorker->setVisible(false);
    ui->dateEdit->setDate(activityBeingAdded.id == -1 ? QDate::currentDate() : activityBeingAdded.date);
    ui->label_addActivity->setText(Translate::addActivity());
    ui->label_description->setText(Translate::description());
    ui->label_date->setText(Translate::date());
    ui->label_durationDays->setText(Translate::durationDays());
    ui->label_repeatEveryXDays->setText(Translate::repeatEveryXDays());
    ui->label_addWorkerToActivity->setText(Translate::addWorkerToActivity());
    ui->comboBox_workers->setItemText(0, Translate::select());
    ui->comboBox_workers->setItemText(1, Translate::newWorker());
    ui->lineEdit_addNewWorker->setPlaceholderText(Translate::name());
    ui->pushButton_add->setText(Translate::add());
    ui->label_assignedWorkers->setText(Translate::assignedWorkers());
    ui->pushButton_remove->setText(Translate::remove());
    ui->pushButton_cancel->setText(Translate::cancel());
}

void AddActivityDialog::on_pushButton_ok_clicked()
{
    bool ok;
    auto duration = ui->lineEdit_duration->text();
    if (duration.isEmpty()) {
        activityBeingAdded.duration = 1;
    }
    else {
        activityBeingAdded.duration = duration.toInt(&ok);
        if (!ok || activityBeingAdded.duration < 0) {
            Gui::ThrowError(Translate::durationMustBeEmptyOrNonNegative());
            return;
        }
    }
    auto repeat = ui->lineEdit_repeat->text();
    if (repeat.isEmpty()) {
        activityBeingAdded.repeat = 0;
    }
    else {
        activityBeingAdded.repeat = repeat.toInt(&ok);
        if (!ok || activityBeingAdded.repeat < 0) {
            Gui::ThrowError(Translate::repeatMustBeEmptyOrNonNegative());
            return;
        }
    }

    activityBeingAdded.plant = importedPlants[ui->plantComboBox->currentIndex()];
    activityBeingAdded.date = ui->dateEdit->date();
    activityBeingAdded.description = ui->textEdit_description->toPlainText().toStdString();
    emit sendActivity(activityBeingAdded);
    this->close();
}

void AddActivityDialog::on_pushButton_cancel_clicked()
{
    if (Gui::GiveQuestion(Translate::areYouSureYouWantToExit())) this->close();
}

void AddActivityDialog::on_pushButton_add_clicked()
{
    // nemre biti current index 0 jer se add disablea
    if (ui->comboBox_workers->currentIndex() == 1) {
        auto name = ui->lineEdit_addNewWorker->text();
        if (name.isEmpty()) {
            Gui::ThrowError(Translate::pleaseInputTheNewWorkersName());
            return;
        }
        auto encodedName = Cryptography::encryptRSA(name.toStdString()).toBase64();
        waitScreen = new WaitScreen();
        connect(this, SIGNAL(sendStopWaiting()), waitScreen, SLOT(closeWindow()));
        waitScreen->show();
        emit sendWorker(encodedName);
    }
    else {
        auto index = ui->comboBox_workers->currentIndex();
        auto worker = importedWorkers[index-2];
        activityBeingAdded.workersAssigned.push_back(worker);
        ui->listWidget_workers->addItem(QString::fromStdString(worker.name));
        ui->comboBox_workers->removeItem(index);
        importedWorkers.erase(importedWorkers.begin()+index-2);
    }
}

void AddActivityDialog::on_comboBox_workers_currentIndexChanged(int index)
{
    ui->lineEdit_addNewWorker->setVisible(index == 1);
    ui->pushButton_add->setEnabled(index != 0);
}

void AddActivityDialog::on_pushButton_remove_clicked()
{
    auto selectedItems = ui->listWidget_workers->selectionModel()->selectedIndexes();
    if (selectedItems.empty()) {
        Gui::ThrowError(Translate::youMustSelectOneActivity());
        return;
    }

    for (auto& v : selectedItems) {
        auto index = v.row();
        importedWorkers.push_back(activityBeingAdded.workersAssigned[index]);
        ui->comboBox_workers->addItem(QString::fromStdString(activityBeingAdded.workersAssigned[index].name));
        activityBeingAdded.workersAssigned[index] = Worker(-100);
    }
    std::remove_if(begin(activityBeingAdded.workersAssigned), end(activityBeingAdded.workersAssigned), [](const auto& v) { return v.id == -100; });
    qDeleteAll(ui->listWidget_workers->selectedItems());
}

void AddActivityDialog::on_pushButton_loadWorkers_clicked()
{
    emit sendRequestForWorkers(userId);
}

void AddActivityDialog::receiveWorkers(const std::vector<Worker>& workers)
{
    importedWorkers.clear();
    for (auto& v : workers) {
        // activityBeingAdded sadrži već nadodane radnike
        // ako su nadodani, onda ih nemaj dodati u importedWorkers JER se importedWorkers dodaju u combo box itd.
        // u importedWorkers idu oni koji nisu već dodani
        if (std::find(activityBeingAdded.workersAssigned.begin(), activityBeingAdded.workersAssigned.end(), v) == activityBeingAdded.workersAssigned.end()) {
            importedWorkers.push_back(std::move(v));
        }
    }
    for (auto& v : importedWorkers) ui->comboBox_workers->addItem(QString::fromStdString(v.name));
    // prva stavka je Select, druga je Add new worker
}

void AddActivityDialog::receiveWorker(const Worker& worker)
{
    if (worker.id != -100) {
        activityBeingAdded.workersAssigned.push_back(worker);
        ui->listWidget_workers->addItem(QString::fromStdString(worker.name));
    }
    emit sendStopWaiting();
}

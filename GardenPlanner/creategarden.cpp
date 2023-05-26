#include "creategarden.h"
#include "commonfunctions.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_creategarden.h"

CreateGarden::CreateGarden(int userId, QString draftFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGarden)
{
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_createGarden->setFont(titleFont);
    this->userId = userId;
    gardenBeingAdded = Garden();
    unaddedPlants = Util::getPlantList();
    if (!Util::isFileEmpty(draftFilePath)) {
        if(Gui::GiveQuestion(Translate::doYouWantToLoadPreviousSettings())) {
            gardenBeingAdded.plants = Util::getAddedPlantsFromDraftFile();
            unaddedPlants = Util::getAllPlantsThatArentInThisList(gardenBeingAdded.plants);
        }
        else Util::truncateDraftFile();
    }
    for (auto& v : gardenBeingAdded.plants) ui->listWidget_addedPlants->addItem(QString::fromStdString(v.name));
    for (auto& v : unaddedPlants)
        ui->listWidget_allPlants->addItem(QString::fromStdString(v.name));

    ui->label_createGarden->setText(Translate::createGarden());
    ui->label_gardenName->setText(Translate::gardenName());
    ui->pushButton_add->setText(Translate::add());
    ui->pushButton_remove->setText(Translate::remove());
    ui->pushButton_cancel->setText(Translate::cancel());
    ui->pushButton_save->setText(Translate::save());
}

CreateGarden::CreateGarden(int userId, const Garden& gardenToBeEdited, QWidget *parent) : QWidget(parent), ui(new Ui::CreateGarden)
{
    ui->setupUi(this);
    this->userId = userId;
    gardenBeingAdded = gardenToBeEdited;
    ui->gardenNameLineEdit->setText(QString::fromStdString(gardenToBeEdited.name));
    for (auto& v : gardenToBeEdited.plants) ui->listWidget_addedPlants->addItem(QString::fromStdString(v.name));
    unaddedPlants = Util::getAllPlantsThatArentInThisList(gardenToBeEdited.plants);
    for (auto& v : unaddedPlants) ui->listWidget_allPlants->addItem(QString::fromStdString(v.name));
}

CreateGarden::~CreateGarden()
{
    delete ui;
}

void CreateGarden::on_pushButton_save_clicked()
{
    auto name = ui->gardenNameLineEdit->text();
    if (name.isEmpty()) {
        Gui::ThrowError(Translate::pleaseNameYourGarden());
        return;
    }
    if (gardenBeingAdded.plants.empty()) {
        Gui::ThrowError(Translate::youMustSelectAtLeastOnePlant());
        return;
    }
    gardenBeingAdded.name = ui->gardenNameLineEdit->text().toStdString();
    emit sendGarden(gardenBeingAdded);
    Util::truncateDraftFile();
}

void CreateGarden::on_pushButton_remove_clicked()
{
    auto selectedIndexes = ui->listWidget_addedPlants->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) {
        Gui::ThrowError(Translate::youMustSelectAtLeastOnePlant());
        return;
    }
    for (auto& v : selectedIndexes) {
        unaddedPlants.push_back(gardenBeingAdded.plants[v.row()]);
        ui->listWidget_allPlants->addItem(QString::fromStdString(gardenBeingAdded.plants[v.row()].name));
    }
    qDeleteAll(ui->listWidget_addedPlants->selectedItems());
    Util::writeDraftToFile(gardenBeingAdded.plants);
}

void CreateGarden::on_pushButton_add_clicked()
{
    auto selectedIndexes = ui->listWidget_allPlants->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) {
        Gui::ThrowError(Translate::youMustSelectAtLeastOnePlant());
        return;
    }
    for (auto& v : selectedIndexes) {
        gardenBeingAdded.plants.push_back(unaddedPlants[v.row()]);
        ui->listWidget_addedPlants->addItem(QString::fromStdString(unaddedPlants[v.row()].name));
    }
    qDeleteAll(ui->listWidget_allPlants->selectedItems());
    Util::writeDraftToFile(gardenBeingAdded.plants);
}

void CreateGarden::on_pushButton_cancel_clicked() {
    if (Gui::GiveQuestion(Translate::areYouSureYouWantToExit())) this->close();
}


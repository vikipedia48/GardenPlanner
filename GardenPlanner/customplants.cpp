#include "customplants.h"
#include "commonfunctions.h"
#include "qjsonobject.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_customplants.h"
#include <QDir>
#include <QJsonDocument>

CustomPlants::CustomPlants(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomPlants)
{
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_customPlants->setFont(titleFont);

    ui->label_customPlants->setText(Translate::customPlants());
    ui->pushButton_clear->setText(Translate::clear());
    ui->pushButton_cancel->setText(Translate::cancel());
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(Translate::name()));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(Translate::description()));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(Translate::region()));

    loadFromJsonIntoTable();
}

CustomPlants::~CustomPlants()
{
    delete ui;
}

void CustomPlants::on_pushButton_cancel_clicked()
{
    if (Gui::GiveQuestion(Translate::areYouSureYouWantToExit())) this->close();
}

void CustomPlants::on_pushButton_ok_clicked()
{
    if (!Gui::GiveQuestion(Translate::areYouSureYouWantToSaveChanges())) return;
    QJsonObject content;
    for (auto i = 0; i < ui->tableWidget->rowCount(); ++i) {
        auto name = ui->tableWidget->item(i, 0);
        auto id = ui->tableWidget->item(i, 1);
        auto description = ui->tableWidget->item(i, 2);
        auto region = ui->tableWidget->item(i,3);
        if (name->text().isEmpty() && id->text().isEmpty() && description->text().isEmpty() && region->text().isEmpty()) continue;
        if (name->text().isEmpty()) {
            Gui::ThrowError(Translate::nameInRow() + QString::number(i) + Translate::cannotBeEmpty());
            return;
        }
        if (id->text().isEmpty()) {
            Gui::ThrowError(Translate::idInRow() + QString::number(i) + Translate::cannotBeEmpty() + Translate::pleaseEnterAUniqueNumber());
            return;
        }
        if (content.contains(id->text())) {
            Gui::ThrowError(Translate::idInRow() + QString::number(i) + Translate::mustBeUnique());
            return;
        }
        QJsonObject current;
        current.insert("name", name->text());
        current.insert("description", description->text());
        current.insert("region", region->text());
        content.insert(id->text(), current);
    }
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson(QJsonDocument::Indented);
    QFile file(QCoreApplication::applicationDirPath()+QDir::separator()+"customplants.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream iStream(&file);
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << bytes;
        file.close();
    }
    else {
        Gui::ThrowError(Translate::errorReadingFile());
        return;
    }
}

void CustomPlants::on_tableWidget_cellClicked(int row, int column)
{
    auto rowCount = ui->tableWidget->rowCount();
    if (row == rowCount-1) {
        addNewRowToTable();
    }
}

void CustomPlants::loadFromJsonIntoTable()
{
    QFile file(QCoreApplication::applicationDirPath()+QDir::separator()+"customplants.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto bytes = file.readAll();
    file.close();
    if (bytes.isEmpty()) {
        addNewRowToTable();
        return;
    }
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Gui::ThrowError(Translate::errorReadingJson());
        addNewRowToTable();
        return;
    }
    if (!document.isObject()) {
        Gui::ThrowError(Translate::errorReadingJson());
        addNewRowToTable();
        return;
    }
    auto jsonObj = document.object();
    if (jsonObj.empty()) addNewRowToTable();
    for (auto& v : jsonObj.keys()) {
        addNewRowToTable (v, jsonObj.value(v).toObject().value("name").toString(), jsonObj.value(v).toObject().value("description").toString(), jsonObj.value(v).toObject().value("region").toString());
    }
}

void CustomPlants::addNewRowToTable(QString item1, QString item2, QString item3, QString item4)
{
    auto rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(item1));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(item2));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(item3));
    ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(item4));
}

void CustomPlants::on_pushButton_clear_clicked()
{
    if (!Gui::GiveQuestion(Translate::areYouSureYouWantToClearTheTable())) return;
    ui->tableWidget->clearContents();
    ui->tableWidget->removeRow(0);
    addNewRowToTable();
}



#include "purchase.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_purchase.h"

Purchase::Purchase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Purchase)
{
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label->setFont(titleFont);
    ui->label->setText(Translate::purchaseGardenPlanner());
    ui->creditCardNumberLabel->setText(Translate::creditCardNumber());
    ui->expirationDateLabel->setText(Translate::expirationDate());
    ui->securityCodeLabel->setText(Translate::securityCode());
}

Purchase::~Purchase()
{
    delete ui;
}

void Purchase::on_pushButton_clicked()
{
    auto number = ui->lineEdit_creditCardNumber->text();
    auto expDate = ui->lineEdit_expirationDate->text();
    auto secCode = ui->lineEdit_securityCode->text();
    if (number.isEmpty()) {
        Gui::ThrowError(Translate::creditCardNumber() + Translate::cannotBeEmpty());
        return;
    }
    if (expDate.isEmpty()) {
        Gui::ThrowError(Translate::expirationDate() + Translate::cannotBeEmpty());
        return;
    }
    if (secCode.isEmpty()) {
        Gui::ThrowError(Translate::securityCode() + Translate::cannotBeEmpty());
        return;
    }
    emit sendPurchaseDetails(number, expDate, secCode);
    //this->close();
}


#include "register.h"
#include "commonfunctions.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_register->setFont(titleFont);
    ui->label_register->setText(Translate::register_());
    ui->label_username->setText(Translate::username());
    ui->label_password->setText(Translate::password());
    ui->label_confirmPassword->setText(Translate::confirmPassword());
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_clicked()
{
    auto username = ui->usernameLineEdit->text();
    if (username.size() < minimumUsernameLength) {
        Gui::ThrowError(Translate::yourUsernameMustBeLonger());
        return;
    }
    auto password = ui->passwordLineEdit->text();
    auto confirmPassword = ui->confirmPasswordLineEdit->text();
    if (password.isEmpty()) {
        Gui::ThrowError(Translate::passwordCannotBeEmpty());
        return;
    }
    if (password != confirmPassword) {
        Gui::ThrowError(Translate::passwordsDontMatch());
        return;
    }
    emit sendRegisterForm(username, password);
}



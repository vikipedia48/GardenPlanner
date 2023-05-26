#include "login.h"
#include "commonfunctions.h"
#include "translationstrings.h"
#include "ui_login.h"
#include "qtfunctions.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setBold(true);
    titleFont.setPointSize(defaultFontSize*Util::titleSizeModifier);
    ui->label_login->setFont(titleFont);
    ui->label_login->setText(Translate::login());
    ui->label_username->setText(Translate::username());
    ui->label_password->setText(Translate::password());
}

Login::~Login()
{
    delete ui;
}


void Login::on_pushButton_clicked()
{
    auto username = ui->usernameLineEdit->text();
    auto password = ui->passwordLineEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        Gui::ThrowError(Translate::invalidInput());
        return;
    }
    emit sendLoginForm(username, password);
}


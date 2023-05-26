#include "welcomescreen.h"
#include "commonfunctions.h"
#include "qapplication.h"
#include "translationstrings.h"
#include "ui_welcomescreen.h"
#include <QDebug>

WelcomeScreen::WelcomeScreen(QString user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeScreen)
{
    ui->setupUi(this);
    ui->label->setText(user.isEmpty() ? Translate::youAreNotLoggedIn() : Translate::welcome()+user+".");
    auto titleFont = QApplication::font();
    auto defaultFontSize = titleFont.pointSize();
    titleFont.setPointSize(defaultFontSize*Util::largeMessageSizeModifier);
    ui->label->setFont(titleFont);
}

WelcomeScreen::~WelcomeScreen()
{
    delete ui;
}

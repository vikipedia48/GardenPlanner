#include "waitscreen.h"
#include "ui_waitscreen.h"
#include "translationstrings.h"

WaitScreen::WaitScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitScreen)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowModality(Qt::WindowModality::WindowModal);
    ui->label->setText(Translate::pleaseWait());
}

WaitScreen::~WaitScreen()
{
    delete ui;
}

void WaitScreen::closeWindow()
{
    this->close();
}

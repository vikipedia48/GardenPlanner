#include "qtfunctions.h"
#include "fmt/format.h"
#include "translationstrings.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QInputDialog>

void Gui::ThrowError(const QString& msg)
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, Translate::error(), msg);
    messageBox.setFixedSize(700,200);
}

QString Gui::GetSavePath(const QString& tip)
{
    auto fileName = QFileDialog::getSaveFileName(nullptr,Translate::saveFile(),QDir::homePath(),tip);
    std::filesystem::path poopie = fileName.toStdString();
    auto final = poopie.parent_path() / fmt::format("{}.pdf", poopie.stem().string());
    return QString::fromStdString(final.string());
}

QString Gui::GetInputPath(const QString& windowName, const QString &tip)
{
    return QFileDialog::getOpenFileName(nullptr,windowName,QDir::homePath(),tip);
}

void Gui::PrintMessage(const QString &title, const QString &msg)
{
    QMessageBox messageBox;
    messageBox.information(nullptr,title,msg);
    messageBox.setFixedSize(700,200);
}

QString Gui::GetStringValueFromInputDialog(const QString &title, const QString &desc, QString placeholder)
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, title, desc, QLineEdit::Normal, placeholder, &ok);
    if (!ok || text.isEmpty()) {
        Gui::ThrowError(Translate::error());
        return "";
    }
    return text;
}

bool Gui::GiveQuestion(const QString &question)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, Translate::question(), question, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) return true;
    else return false;
}

double Gui::GetNumberValueFromInputDialog(const QString &title, const QString &desc)
{
    double rv;
    bool ok;
    QString text = QInputDialog::getText(nullptr, title, desc, QLineEdit::Normal);
    rv = text.toDouble(&ok);
    if (!ok) {
        Gui::ThrowError(Translate::invalidInput());
        return -999;
    }
    return rv;
}

#include "settings.h"
#include "commonfunctions.h"
#include "qsettings.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include "ui_settings.h"

#include <QFile>

Settings::Settings(const QString& iniPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings), iniPath(iniPath)
{
    ui->setupUi(this);
    updateTranslations();

    QFile file(iniPath);
    if (!file.exists()) {
        Gui::ThrowError(Translate::configIniNotFound());
        on_pushButton_default_clicked();
        return;
    }
    loadSettings();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_apply_clicked()
{
    // promijeni postavke i spremi u ini
    auto newFontSize = ui->fontSizeDoubleSpinBox->text().toInt();
    if (newFontSize < 1 || newFontSize > 64) {
        Gui::ThrowError(Translate::invalidFontSize());
        return;
    }
    Util::Language language;
    switch (ui->languageComboBox->currentIndex()) {
        case 1:
            language = Util::Language::CROATIAN;
            break;
        default:
            language = Util::Language::ENGLISH;
            break;
    }
    auto darkMode = ui->themeComboBox->currentIndex() == 1;
    emit sendNewSettings(language, newFontSize, darkMode);
    updateTranslations();
    auto titleFont = QFont(QApplication::font());
    titleFont.setBold(true);
    titleFont.setPointSize(newFontSize*Util::titleSizeModifier);
    ui->label_settings->setFont(titleFont);
}


void Settings::on_pushButton_default_clicked()
{
    ui->themeComboBox->setCurrentIndex(0);
    ui->fontSizeDoubleSpinBox->setValue(11);
    on_pushButton_apply_clicked();
}

void Settings::loadSettings(bool askIfSure)
{
    if (askIfSure) {
        if (!Gui::GiveQuestion(Translate::areYouSureYouWantToRevert())) return;
    }
    QSettings settings(iniPath, QSettings::IniFormat);
    auto language = settings.value("language").value<QString>();
    ui->languageComboBox->setCurrentIndex(language == "hr" ? 1 : 0);
    auto theme = settings.value("darkMode").value<bool>();
    ui->themeComboBox->setCurrentIndex(theme ? 1 : 0);
    auto fontSize = settings.value("fontSize").value<int>();
    ui->fontSizeDoubleSpinBox->setValue(fontSize);
    auto titleFont = QFont(QApplication::font());
    titleFont.setBold(true);
    titleFont.setPointSize(fontSize*Util::titleSizeModifier);
    ui->label_settings->setFont(titleFont);
}

void Settings::updateTranslations()
{
    ui->label_settings->setText(Translate::settings());
    ui->label_theme->setText(Translate::theme());
    ui->themeComboBox->setItemText(0, Translate::white());
    ui->themeComboBox->setItemText(1, Translate::dark());
    ui->label_fontSize->setText(Translate::fontSize());
    ui->label_language->setText(Translate::language());
    ui->pushButton_default->setText(Translate::defaultSettings());
    ui->pushButton_apply->setText(Translate::apply());
}


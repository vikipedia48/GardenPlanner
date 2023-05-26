#ifndef SETTINGS_H
#define SETTINGS_H

#include "commonfunctions.h"
#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(const QString& iniPath, QWidget *parent = nullptr);
    ~Settings();
signals:
    void sendNewSettings(Util::Language language, int fontSize, bool darkMode);

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_default_clicked();

private:
    Ui::Settings *ui;
    QString iniPath;
    void loadSettings(bool askIfSure = false);
    void updateTranslations();
};

#endif // SETTINGS_H

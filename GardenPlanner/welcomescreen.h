#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>

namespace Ui {
class WelcomeScreen;
}

class WelcomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QString user, QWidget *parent = nullptr);
    ~WelcomeScreen();

private:
    Ui::WelcomeScreen *ui;
};

#endif // WELCOMESCREEN_H

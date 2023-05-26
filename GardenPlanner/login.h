#ifndef LOGIN_H
#define LOGIN_H

#include <QNetworkReply>
#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
signals:
    void sendLoginForm(const QString& username, const QString& password);
    void sendLoggedIn();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H

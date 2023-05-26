#ifndef REGISTER_H
#define REGISTER_H

#include <QNetworkReply>
#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    static constexpr int minimumUsernameLength = 5;
    ~Register();
signals:
    void sendRegisterForm(const QString& username, const QString& password);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H

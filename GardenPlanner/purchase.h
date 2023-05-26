#ifndef PURCHASE_H
#define PURCHASE_H

#include <QWidget>

namespace Ui {
class Purchase;
}

class Purchase : public QWidget
{
    Q_OBJECT

public:
    explicit Purchase(QWidget *parent = nullptr);
    ~Purchase();
signals:
    void sendPurchaseDetails(const QString& number, const QString& expDate, const QString& secCode);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Purchase *ui;
};

#endif // PURCHASE_H

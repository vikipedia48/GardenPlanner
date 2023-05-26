#ifndef CUSTOMPLANTS_H
#define CUSTOMPLANTS_H

#include <QWidget>

namespace Ui {
class CustomPlants;
}

class CustomPlants : public QWidget
{
    Q_OBJECT

public:
    explicit CustomPlants(QWidget *parent = nullptr);
    ~CustomPlants();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_clear_clicked();

private:
    Ui::CustomPlants *ui;
    void loadFromJsonIntoTable();
    void addNewRowToTable(QString item1 = "", QString item2 = "", QString item3 = "", QString item4 = "");
};

#endif // CUSTOMPLANTS_H

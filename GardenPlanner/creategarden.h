#ifndef CREATEGARDEN_H
#define CREATEGARDEN_H

#include "garden.h"
#include <QWidget>

namespace Ui {
class CreateGarden;
}

class CreateGarden : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGarden(int userId, QString draftFilePath, QWidget *parent = nullptr);
    CreateGarden(int userId, const Garden& gardenToBeEdited, QWidget* parent = nullptr);
    ~CreateGarden();
signals:
    void sendGarden(const Garden& garden);
private slots:


    void on_pushButton_save_clicked();

    void on_pushButton_remove_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::CreateGarden *ui;
    Garden gardenBeingAdded;
    std::vector<Plant> unaddedPlants;
    int userId;
};

#endif // CREATEGARDEN_H

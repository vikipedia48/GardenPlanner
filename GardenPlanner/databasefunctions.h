#ifndef DATABASEFUNCTIONS_H
#define DATABASEFUNCTIONS_H

#include "garden.h"

#include <vector>



namespace Database
{
    Garden getGarden(int id);
    std::vector<Worker> getUsersWorkers(int id);
    std::vector<Plant> getPlantsInGarden(int id);
    std::vector<Garden> getUsersGardens(int id);
    void addNewActivity(const Activity& activity, int userId);
    void editActivity(const Activity& activity, int userId);
    void addNewGarden(const Garden& garden, int userId);
    void editGarden(const Garden& garden);
    void deleteGarden(int id);
    bool registerUser(const QString& username, const QString& name, const QString& password);
    bool authentication(const QString& username, const QString& password, int& outId, QString& outUsername);
    void addNewWorker(const Worker& worker, const QByteArray& privateKey, int userId);
    bool verifyCustomPlants();
};

#endif // DATABASEFUNCTIONS_H

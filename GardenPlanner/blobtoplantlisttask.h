#ifndef BLOBTOPLANTLISTTASK_H
#define BLOBTOPLANTLISTTASK_H

#include "plant.h"

#include <QObject>
#include <QRunnable>

class BlobToPlantListTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    BlobToPlantListTask();
    BlobToPlantListTask(const QByteArray& blob, std::vector<Plant>* outPlants);
    QByteArray blob;
    std::vector<Plant>* outPlants;
signals:
    void sendThreadFinished();

    // QRunnable interface
public:
    void run() override;
};

#endif // BLOBTOPLANTLISTTASK_H

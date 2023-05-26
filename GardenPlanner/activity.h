#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "plant.h"
#include "qdatetime.h"
#include "worker.h"
#include <string>
#include <vector>

class Activity
{
public:
    Activity();
    Activity(std::int64_t id, Plant plant, std::string description, int duration, int repeat, QDate date, std::int64_t gardenId)
        : id(id), plant(plant), description(description), duration(duration), repeat(repeat), date(date), gardenId(gardenId) {}

    std::int64_t id;
    Plant plant;
    std::string description;
    int duration;
    int repeat;
    std::vector<Worker> workersAssigned;
    QDate date;
    std::int64_t gardenId;

};

#endif // ACTIVITY_H

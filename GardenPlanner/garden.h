#ifndef GARDEN_H
#define GARDEN_H

#include "activity.h"
#include "plant.h"



class Garden
{
public:
    Garden();
    std::int64_t id;
    std::string name;
    std::vector<Plant> plants;
    std::vector<Activity> activities;
    std::int64_t userId;
    //std::vector<Worker> workers;
};

#endif // GARDEN_H

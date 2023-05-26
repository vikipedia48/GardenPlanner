#ifndef PLANT_H
#define PLANT_H

#include <string>

class Plant
{
public:
    Plant();
    Plant(int id, std::string name) : id(id), name(name) {}
    int id;
    std::string name;
    friend bool operator == (const Plant& lhs, const Plant& rhs) noexcept {
        return lhs.id == rhs.id;
    }
};

#endif // PLANT_H

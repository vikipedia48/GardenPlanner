#ifndef WORKER_H
#define WORKER_H

#include <string>

class Worker
{
public:
    Worker();
    Worker(std::int64_t id, std::string name) : id(id), name(name) {}
    Worker(std::int64_t id) : id(id) {}
    std::int64_t id;
    std::string name;
    bool operator == (const Worker& rhs) { return rhs.id == id; }
};

#endif // WORKER_H

#include "blobtoplantlisttask.h"
#include "commonfunctions.h"

BlobToPlantListTask::BlobToPlantListTask()
{

}

BlobToPlantListTask::BlobToPlantListTask(const QByteArray &blob, std::vector<Plant> *plants) : blob(blob), outPlants(plants)
{
    this->setAutoDelete(true);
}

void BlobToPlantListTask::run()
{
    std::vector<Plant> plants;
    auto plantList = Util::getPlantList();
    auto index = 0;
    std::array<char, 4> block;
    for (auto it = blob.begin(); it != blob.end(); it += 4) {
        std::memcpy(std::addressof(block), it, 4);
        const std::int32_t id = *reinterpret_cast<std::int32_t*>(block.data());
        if (id < Util::customPlantsMinimumId) {
            plants.push_back(plantList[id-1]);
            continue;
        }
        auto foundPlant = std::optional<Plant>();
        for (auto i = Util::customPlantsMinimumId-1; i < plantList.size(); ++i) {
            if (plantList[i].id == id) {
                //rv.emplace_back(Plant(id, plantList[i].name));
                foundPlant = Plant(id, plantList[i].name);
                break;
            }
        }

        plants.push_back(foundPlant.value_or(Plant(id, "Custom Plant")));
    }
    *outPlants = std::move(plants);
    emit sendThreadFinished();
}

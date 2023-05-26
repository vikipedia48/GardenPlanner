#include "barrier.h"
#include "commonfunctions.h"
#include "qtfunctions.h"
#include "translationstrings.h"
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>

std::vector<Plant> Util::getPlantList()
{
    std::vector<Plant> rv = {
        //Plant(0, "Općenito"),
        Plant(1, Translate::plant1()),
        Plant(2, Translate::plant2()),
        Plant(3, Translate::plant3()),
        Plant(4, Translate::plant4()),
        Plant(5, Translate::plant5()),
        Plant(6, Translate::plant6()),
        Plant(7, Translate::plant7()),
        Plant(8, Translate::plant8()),
        Plant(9, Translate::plant9()),
        Plant(10, Translate::plant10()),
        Plant(11, Translate::plant11()),
        Plant(12, Translate::plant12()),
        Plant(13, Translate::plant13()),
        Plant(14, Translate::plant14()),
        Plant(15, Translate::plant15()),
        Plant(16, Translate::plant16()),
        Plant(17, Translate::plant17()),
        Plant(18, Translate::plant18()),
        Plant(19, Translate::plant19()),
        Plant(20, Translate::plant20()),
        Plant(21, Translate::plant21()),
        Plant(22, Translate::plant22()),
        Plant(23, Translate::plant23()),
        Plant(24, Translate::plant24()),
        Plant(25, Translate::plant25()),
        Plant(26, Translate::plant26()),
        Plant(27, Translate::plant27()),
        Plant(28, Translate::plant28()),
        Plant(29, Translate::plant29()),
        Plant(30, Translate::plant30()),
        Plant(31, Translate::plant31()),
        Plant(32, Translate::plant32()),
        Plant(33, Translate::plant33()),
        Plant(34, Translate::plant34()),
        Plant(35, Translate::plant35()),
        Plant(36, Translate::plant36()),
        Plant(37, Translate::plant37()),
        Plant(38, Translate::plant38()),
        Plant(39, Translate::plant39()),
        Plant(40, Translate::plant40()),
        Plant(41, Translate::plant41()),
        Plant(42, Translate::plant42()),
        Plant(43, Translate::plant43()),
        Plant(44, Translate::plant44()),
        Plant(45, Translate::plant45()),
        Plant(46, Translate::plant46()),
        Plant(47, Translate::plant47()),
        Plant(48, Translate::plant48()),
        Plant(49, Translate::plant49()),
        Plant(50, Translate::plant50()),
        Plant(51, Translate::plant51()),
        Plant(52, Translate::plant52()),
        Plant(53, Translate::plant53()),
        Plant(54, Translate::plant54()),
        Plant(55, Translate::plant55()),
        Plant(56, Translate::plant56()),
        Plant(57, Translate::plant57()),
    };

    auto customPlants = Util::getCustomPlants();
    for (auto& v : customPlants) v.id+=Util::customPlantsMinimumId;
    rv.insert(rv.end(), customPlants.begin(), customPlants.end());

    return rv;
}

std::vector<Plant> Util::getCustomPlants()
{
    std::vector<Plant> rv;
    QFile file(QCoreApplication::applicationDirPath()+QDir::separator()+"customplants.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto bytes = file.readAll();
    file.close();
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Gui::ThrowError("Error reading JSON");
        return {};
    }
    if (!document.isObject()) {
        Gui::ThrowError("Error reading JSON");
        return {};
    }
    auto jsonObj = document.object();

    for (auto& v : jsonObj.keys()) {
        rv.emplace_back(Plant(v.toInt(), jsonObj.value(v).toObject().value("name").toString().toStdString()));
    }
    return rv;
}

std::vector<Plant> Util::getAddedPlantsFromDraftFile()
{
    // ?digitalni potpis?
    std::vector<Plant> rv = {};
    std::ifstream file((QCoreApplication::applicationDirPath()+QDir::separator()+"draft.bin").toStdString(), std::ios::in | std::ios::binary);
    std::array<char, 4> block;
    while (file.read(block.data(), 4)) {
      const std::int32_t id = *reinterpret_cast<std::int32_t*>(block.data());
        auto plantList = Util::getPlantList();
        if (id < Util::customPlantsMinimumId) {
            rv.emplace_back(Plant(id, plantList[id-1].name));
            continue;
        }
        auto foundPlant = std::optional<Plant>();
        for (auto i = Util::customPlantsMinimumId-1; i < plantList.size(); ++i) {
            if (plantList[i].id == id) {
                foundPlant = Plant(id, plantList[i].name);
                break;
            }
        }
        rv.push_back(foundPlant.value_or(Plant(id, "Custom Plant")));
    }
    file.close();
    return rv;
}

std::vector<Plant> Util::getAllPlantsThatArentInThisList(const std::vector<Plant>& diff)
{
    std::vector<Plant> rv;
    auto allPlants = Util::getPlantList();
    for (const auto& v : allPlants) {
        if (std::find(diff.begin(), diff.end(), v) != diff.end()) continue;
        rv.push_back(v);
    }
    return rv;
}

bool Util::isFileEmpty(const QString &path)
{
    QFile file(path);
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
    }
    return file.size()==0;
}

std::vector<char> Util::createBlobFromPlantList(const std::vector<Plant> &plants)
{
    std::vector<char> blob;
    blob.reserve(plants.size() * 4);
      for (const auto& plant : plants) {
        std::array<char, 4> bytes;
        std::memcpy(bytes.data(), std::addressof(plant.id), 4);
        for (auto byte : bytes) blob.push_back(byte);
      }
      return blob;
}

void Util::writeDraftToFile(const std::vector<Plant>& plants)
{
    QFile file (QCoreApplication::applicationDirPath()+QDir::separator()+"draft.bin");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    auto data = Util::createBlobFromPlantList(plants);
    file.write(data.data(), data.size());
    file.close();
}

void Util::truncateDraftFile()
{
    QFile file (QCoreApplication::applicationDirPath()+QDir::separator()+"draft.bin");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}

bool Util::isActivityInThisMonth(const Activity &activity, const QDate &day)
{
    // u day se proslijedi prvi dan u mjesecu
    auto aDate = activity.date;
    if (aDate.year() != day.year()) return false;
    if (aDate.month() == day.month()) return true;
    if (aDate.month() > day.month()) return false;

    // sve dalje su slučajevi kada je activity u istoj godini, ali ne u istom mjesecu, nego nekom prijašnjem

    // ako se activity ponavlja svaki dan
    auto aRepeat = activity.repeat;
    if (aRepeat == 1) return true;

    // ako je daysTo od (aDate + aDuration,day) negativan ili 0, return true
    auto aDuration = activity.duration;
    if (aDuration > 1) {
        auto tempDate = aDate.addDays(aDuration-1);
        if (tempDate.daysTo(day) <= 0) return true;
    }

    // sad repeatovi
    if (aRepeat < 1) return false;
    auto tempDate = aDate.addDays(aRepeat);
    for (; day.month() >= tempDate.month(); tempDate = tempDate.addDays(aRepeat)) {
        if (tempDate.month() == day.month()) return true;
    }
    return false;
}

bool Util::isActivityInThisDay(const Activity &activity, const QDate &day)
{
        auto aDate = activity.date;
        if (aDate.year() != day.year()) return false;

        auto daysTo = aDate.daysTo(day); // pozitivno ak je activity date prije dana posrijedi, negativno ak je posle
        auto aRepeat = activity.repeat;
        if (daysTo < 0) return false; // prijeći ovu liniju koda može samo dan kaj je prije neg kaj activity počne
        if (aRepeat == 1) return true; // edge case; repeata se activity svaki dan

        auto aDuration = activity.duration;
        if (aDuration > 1) {
            if (daysTo < aDuration) return true;
        }

        if (aRepeat < 1) return false;
        auto tempDay = aDate.addDays(aRepeat);
        for (; day.month() >= tempDay.month(); tempDay = tempDay.addDays(aRepeat)) {
            daysTo = day.daysTo(tempDay);
            if (daysTo < 0) continue;
            if (daysTo == 0) return true;
            if (aDuration > 1 && daysTo < aDuration) return true;
        }

        return false;
}

QByteArray Util::readLocalFile(const QString &path)
{
    QFile file(QCoreApplication::applicationDirPath() + QDir::separator() + path);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("could not open file");
    }
    auto data = file.readAll();
    return data;
}

void Util::writeLocalFile(QString const& path, QByteArray const& data)
{
    QFile file(QCoreApplication::applicationDirPath() + QDir::separator() + path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        throw std::runtime_error("could not open file");
    }
    file.write(data.data(), data.size());
}

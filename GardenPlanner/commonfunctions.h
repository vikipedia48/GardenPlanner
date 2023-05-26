#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include "activity.h"
#include "plant.h"

#include <vector>


namespace Util {

    static const int customPlantsMinimumId = 58;
    static const float titleSizeModifier = 16.0/11.0;
    static const float largeMessageSizeModifier = 24.0/11.0;
    std::vector<Plant> getPlantList();
    std::vector<Plant> getCustomPlants();
    std::vector<Plant> getAddedPlantsFromDraftFile();
    std::vector<Plant> getAllPlantsThatArentInThisList(const std::vector<Plant>& diff);
    std::vector<char> createBlobFromPlantList(const std::vector<Plant>& plants);
    void writeDraftToFile(const std::vector<Plant>& plants);
    void truncateDraftFile();
    bool isFileEmpty(const QString& path);
    bool isActivityInThisDay(const Activity& activity, const QDate& day);
    bool isActivityInThisMonth(const Activity& activity, const QDate& date);
    QByteArray readLocalFile(QString const& path);
    void writeLocalFile(QString const& path, QByteArray const& data);

    enum class Language {
        ENGLISH,
        CROATIAN
    };
}
/*
#define catch_and_trace(EXPR) \
    do { \
        try { \
            EXPR ; \
        } catch (std::exception const& ex) { \
            qDebug() << "at statement: '" << #EXPR << "' with error: " << ex.what(); \
            throw; \
        } \
    } while(false)
*/
#endif // COMMONFUNCTIONS_H

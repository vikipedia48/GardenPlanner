#include "databasefunctions.h"
#include "blobtoplantlisttask.h"
#include "barrier.h"
#include "cryptography.h"
#include "qtfunctions.h"

#include <QThreadPool>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QURL>

std::vector<Worker> Database::getUsersWorkers(int id)
{
    // recimo da se dobije std::vector<Worker> i std::vector<QByteArray> workerKeys
    std::vector<Worker> dobijeSe;
    std::vector<QByteArray> dobiveniWorkerKeyovi; // iste veličine ko dobijeSe

    // treba se decodeati RSA imena workera
    for (auto i = 0; i < dobijeSe.size(); ++i) {
        dobijeSe[i].name = Cryptography::decryptRSA(QByteArray(dobijeSe[i].name.c_str(),dobijeSe[i].name.length()), dobiveniWorkerKeyovi[i]);
    }
    return dobijeSe;
}

std::vector<Plant> Database::getPlantsInGarden(int id)
{
    return {};
}

void Database::editActivity(const Activity &activity, int userId)
{
    // TREBA SE ŠIFRIRATI S AES-OM
    auto key = QCryptographicHash::hash(QString("ActivityKey12345").toLocal8Bit(), QCryptographicHash::Sha256);
    auto iv = QCryptographicHash::hash(QString("Initialization09").toLocal8Bit(), QCryptographicHash::Md5);
    auto str = QString::fromStdString(activity.description);
    auto encodedActivityDescription = Cryptography::encryptAES(str, key, iv);

    for (auto& v : activity.workersAssigned) {
        if (v.id == -1) {
            // TREBA SE DODATI U BAZU I ŠIFRIRATI RSA-OM
            auto _worker = Worker(-1);
            QByteArray privateKey, encodedMsg;
            std::string msg = v.name;
            if (!Cryptography::encryptRSA(msg,encodedMsg,privateKey)) {
                Gui::ThrowError("RSA error!");
                return;
            }
            _worker.name = encodedMsg.toStdString(); // ?
            addNewWorker(_worker, privateKey, userId);
        }
    }

    // roko :
}

void Database::addNewActivity(const Activity &activity, int userId)
{
    // TREBA SE ŠIFRIRATI S AES-OM
    auto key = QCryptographicHash::hash(QString("ActivityKey12345").toLocal8Bit(), QCryptographicHash::Sha256);
    auto iv = QCryptographicHash::hash(QString("Initialization09").toLocal8Bit(), QCryptographicHash::Md5);
    auto str = QString::fromStdString(activity.description);
    auto encodedActivityDescription = Cryptography::encryptAES(str, key, iv);

    for (auto& v : activity.workersAssigned) {
        if (v.id == -1) {
            // TREBA SE DODATI U BAZU I ŠIFRIRATI RSA-OM
            auto _worker = v;
            QByteArray privateKey, encodedMsg;
            std::string msg = v.name;
            if (!Cryptography::encryptRSA(msg,encodedMsg,privateKey)) {
                Gui::ThrowError("RSA error!");
                return;
            }
            _worker.name = encodedMsg.toStdString(); // ?
            addNewWorker(_worker, privateKey, userId);
        }
    }


    // roko :
}

void Database::addNewGarden(const Garden &garden, int userId)
{

}

void Database::editGarden(const Garden &garden) //, int userId?
{

}

std::vector<Garden> Database::getUsersGardens(int id)
{
    //U prozoru View Garden, učita se vrt iz Gardens te jedna dretva procesira blob polje, a druga traži po bazi sve radnike koji su u ijednoj aktivnosti u tom vrtu
    // TO JE PROBLEM JER ZBOG PROMJENE U DIZAJNU, KLASA GARDEN VIŠE NE SADRŽI LISTU RADNIKA (NE TREBA MU)
    // BLOB SE I DALJE MOŽE PROCESIRATI NA DRUGOM THREADU, A GLAVNI THREAD SVE OSTALO

    // recimo da je fetchan blob u std::vector<char>
    std::vector<char> blob;
    auto gardenPlants = new std::vector<Plant>(blob.size()/4);
    auto task = new BlobToPlantListTask(blob, gardenPlants);
    auto barrier = Barrier(1);
    auto on_finished = [barrier = barrier]() mutable { barrier.arrive(); };
    QObject::connect(task, &BlobToPlantListTask::sendThreadFinished, on_finished);
    QThreadPool::globalInstance()->start(task);

    // NAPRAVI SVE OSTALO ZA INICIJALIZACIJU GARDEN

    barrier.wait();
    delete gardenPlants;
    return {};
}

Garden Database::getGarden(int id)
{
    return {};
}

void Database::deleteGarden(int id)
{

}

bool Database::registerUser(const QString& username, const QString& name, const QString& password)
{
    return true;
}

bool Database::authentication(const QString& username, const QString& password, int &outId, QString& outUsername)
{
    return true;
}

void Database::addNewWorker(const Worker &worker, const QByteArray& privateKey, int userId)
{

}

bool Database::verifyCustomPlants()
{


    return true;
}

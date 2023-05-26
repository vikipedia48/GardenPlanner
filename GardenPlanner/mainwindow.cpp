#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "creategarden.h"
#include "customplants.h"
#include "login.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qtfunctions.h"
#include "register.h"
#include "settings.h"
#include "viewgardens.h"
#include "welcomescreen.h"
#include "barrier.h"
#include "blobtoplantlisttask.h"
#include "cryptography.h"
#include "gardencookiejar.h"
#include "purchase.h"
#include <QStyle>
#include <QFile>
#include <QNetworkAccessManager>
#include <qnetworkcookie.h>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QThreadPool>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    requestManager = new QNetworkAccessManager();
    auto cookieJar = new GardenCookieJar();
    cookieJar->setCookiesFromUrl(loadCookiesFromCache(), QUrl(urlPrefix));
    requestManager->setCookieJar(cookieJar);

    QSettings settings(iniPath, QSettings::IniFormat);
    QFile file(iniPath);
    if (!file.exists()) {
        settings.setValue("darkMode",false);
        settings.setValue("fontSize",11);
        settings.setValue("language","en");
    }
    changeFontSize(settings.value("fontSize").value<int>());
    changeTheme(settings.value("darkMode").value<bool>());
    auto _language = settings.value("language").value<QString>();
    changeLanguage(_language == "hr" ? Util::Language::CROATIAN : Util::Language::ENGLISH);
    updateTranslationsInMainWindow();

    loggedIn = false;
    onLoginOrLogout();
    currentWindow = nullptr;
    on_actionGo_home_triggered();
    validateUser();

}
MainWindow::~MainWindow()
{
    delete requestManager;
    delete currentWindow;
    delete ui;
}

// FUNCTIONS FOR UI ELEMENT ON-CLICKS

void MainWindow::on_actionGo_home_triggered()
{
    closeCurrentWindow();
    currentWindow = new WelcomeScreen(loggedIn ? this->username : "", this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    currentWindow->show();
}

void MainWindow::on_actionLogin_triggered()
{
    closeCurrentWindow();
    currentWindow = new Login(this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    connect(currentWindow, SIGNAL(sendLoginForm(const QString&, const QString&)), this, SLOT(receiveLoginForm(const QString&, const QString&)));
    currentWindow->show();
}

void MainWindow::on_actionRegister_triggered()
{
    closeCurrentWindow();
    currentWindow = new Register(this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    connect(currentWindow, SIGNAL(sendRegisterForm(const QString&, const QString&)), this, SLOT(receiveRegisterForm(const QString&, const QString&)));
    currentWindow->show();
}

void MainWindow::on_actionLogout_triggered()
{
    deleteSession();
    loggedIn = false;
    isAdmin = false;
    username = "";
    closeCurrentWindow();
    onLoginOrLogout();
    clearCache();
    on_actionGo_home_triggered();
}

void MainWindow::on_actionAdd_garden_triggered()
{
    closeCurrentWindow();
    currentWindow = new CreateGarden(userId, draftFilePath, this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    connect(currentWindow, SIGNAL(sendGarden(const Garden&)), this, SLOT(receiveGarden(const Garden&)));
    currentWindow->show();
}

void MainWindow::on_actionView_gardens_triggered()
{
    auto request = isAdmin? QNetworkRequest(QUrl(urlPrefix+"gardens")) : QNetworkRequest(QUrl(urlPrefix+"users/"+QString::number(this->userId)+"/gardens"));;
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersGardens);
    requestManager->get(request);
}

void MainWindow::on_actionCustom_plants_triggered()
{
    closeCurrentWindow();
    currentWindow = new CustomPlants(this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    currentWindow->show();
}

void MainWindow::on_actionSettings_triggered()
{
    closeCurrentWindow();
    currentWindow = new Settings(iniPath, this);
    connect(currentWindow, SIGNAL(sendNewSettings(Util::Language, int, bool)), this, SLOT(receiveNewSettings(Util::Language, int, bool)));
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    currentWindow->show();
}


// SLOTS FOR RECEIVING SIGNALS FROM CHILD WIDGETS

void MainWindow::receiveNewSettings(Util::Language language, int fontSize, bool darkMode)
{
    changeLanguage(language);
    changeFontSize(fontSize);
    changeTheme(darkMode);
}

void MainWindow::receivePurchaseDetails(const QString &number, const QString &expDate, const QString &secCode)
{
    QJsonObject content;
    content.insert("credit_card_number", number);
    content.insert("expiration_date", expDate);
    content.insert("security_code", secCode);
    auto document = QJsonDocument();
    document.setObject(content);
    auto bytes = document.toJson();
    auto request = QNetworkRequest(QUrl(urlPrefix+"serial-key"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handlePurchase);
    requestManager->post(request, bytes);
}

void MainWindow::receiveRegisterForm(const QString &username, const QString &password)
{
    auto url = QUrl(urlPrefix+"auth/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject content;
    content.insert("username", username);
    content.insert("password", password);
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson();
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleRegister);
    requestManager->post(request, bytes);
}

void MainWindow::receiveLoginForm(const QString& username, const QString& password)
{
    auto url = QUrl(urlPrefix+"auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject content;
    content.insert("username", username);
    content.insert("password", password);
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson();
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleLogIn);
    requestManager->post(request, bytes);
}

void MainWindow::receiveGarden(const Garden &garden)
{
    QString urlString;
    QByteArray operation;
    QJsonObject content;
    content.insert("name", QString::fromStdString(garden.name));
    content.insert("plants", QJsonValue(QByteArray(Util::createBlobFromPlantList(garden.plants).data(), garden.plants.size()*4).toBase64().data()));
    if (garden.id == -1) {
        content.insert("user_id", this->userId);
        urlString = urlPrefix + "gardens";
        operation = "POST";
    }
    else {
        urlString = urlPrefix + "gardens/" + QString::number(garden.id);
        operation = "PATCH";
    }
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson();
    auto url = QUrl(urlString);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleGardenCreation);
    requestManager->sendCustomRequest(request, operation, bytes);
}

void MainWindow::receiveEditGarden(std::int64_t gardenId)
{
    QNetworkRequest request(QUrl(urlPrefix+"gardens/"+QString::number(gardenId)));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleEditGarden);
    requestManager->get(request);
}

void MainWindow::receiveRequestForActivities(int64_t gardenId)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"gardens/"+QString::number(gardenId)+"/activities"));
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleRequestForActivities);
    requestManager->get(request);
}

void MainWindow::receiveRequestForGardenDeletion(int64_t gardenId)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"gardens/"+QString::number(gardenId)));
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleGardenDeletion);
    requestManager->deleteResource(request);
}

void MainWindow::receiveRequestForActivityDeletion(int64_t activityId)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"activities/"+QString::number(activityId)));
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleGardenDeletion);
    requestManager->deleteResource(request);
}

void MainWindow::receiveActivity(const Activity &activity)
{
    QString urlString = urlPrefix+"activities";
    QByteArray operation;
    QJsonObject content;
    content.insert("garden_id",activity.gardenId);
    content.insert("plant_id",activity.plant.id);
    content.insert("description", QJsonValue(Cryptography::encryptAES(QString::fromStdString(activity.description)).toBase64().data()));
    content.insert("duration",activity.duration);
    content.insert("repeat",activity.repeat);
    content.insert("start_date",activity.date.toString("yyyy-MM-dd"));
    QJsonArray workerIds;
    for (auto& v : activity.workersAssigned) workerIds.append(v.id);
    content.insert("assigned_workers", workerIds);

    if (activity.id == -1) {
        operation = "POST";
    }
    else {
        content.insert("id", activity.id);
        operation = "PATCH";
        urlString += "/"+QString::number(activity.id);
    }
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson();
    auto request = QNetworkRequest(QUrl(urlString));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleActivity);
    requestManager->sendCustomRequest(request, operation, bytes);
}

void MainWindow::receiveRequestForUsersWorkers(std::int64_t userId)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"users/"+QString::number(userId)+"/workers"));
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersWorkers);
    requestManager->get(request);
}

void MainWindow::receiveRequestForUsersWorkersForReport(std::int64_t userId)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"users/"+QString::number(userId)+"/workers"));
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersWorkersForReport);
    requestManager->get(request);
}

void MainWindow::receiveWorker(const QString & name)
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"workers"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject content;
    content.insert("user_id",this->userId);
    content.insert("name", name);
    QJsonDocument document;
    document.setObject(content);
    auto bytes = document.toJson();
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleWorker);
    requestManager->post(request, bytes);
}


// SLOTS FOR HANDLING REPLIES FROM QNETWORKACCESSMANAGER

void MainWindow::handleRegister(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleRegister);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorInCreatingNewAccount());
        goto end;
    }
    on_actionGo_home_triggered();
    Gui::PrintMessage(Translate::register_(), Translate::success());
    end:;
    reply->deleteLater();
}

void MainWindow::handleLogIn(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleLogIn);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::wrongUsernameSlashPassword());
    }
    else {
        logIn(reply->readAll());
    }
    reply->deleteLater();
}

void MainWindow::handleGardenCreation(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleGardenCreation);
    if (reply->error() == QNetworkReply::NoError) {
        on_actionView_gardens_triggered();
        Gui::PrintMessage(Translate::createGarden(), Translate::success());
    }
    reply->deleteLater();
}

void MainWindow::handleEditGarden(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleEditGarden);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorGettingResource());
    }
    else {
        Garden gardenToBeEdited;
        auto bytes = reply->readAll();
        QJsonParseError jsonError;
        auto document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isObject()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        auto jsonObj = document.object();

        auto jsonPlants = QByteArray::fromBase64(jsonObj.value("plants").toString().toLocal8Bit());
        auto barrier = Barrier(1);
        auto task = new BlobToPlantListTask(jsonPlants, std::addressof(gardenToBeEdited.plants));
        auto on_finished = [barrier = barrier]() mutable { barrier.arrive(); };
        connect(task, &BlobToPlantListTask::sendThreadFinished, on_finished);
        QThreadPool::globalInstance()->start(task);

        gardenToBeEdited.id = jsonObj.value("id").toInteger();
        gardenToBeEdited.name = jsonObj.value("name").toString().toStdString();
        auto _userId = jsonObj.value("user_id").toInteger();
        closeCurrentWindow();

        barrier.wait();

        currentWindow = new CreateGarden(_userId, gardenToBeEdited, this);
        ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
        connect(currentWindow, SIGNAL(sendGarden(const Garden&)), this, SLOT(receiveGarden(const Garden&)));
        currentWindow->show();
    }

    end:;
    reply->deleteLater();
}

void MainWindow::handleUsersGardens(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersGardens);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorGettingResource());
        on_actionGo_home_triggered();
    }
    else {
        openViewGardensWindow(reply->readAll());
    }
    //end:;
    reply->deleteLater();
}

void MainWindow::handleRequestForActivities(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleRequestForActivities);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorGettingResource());
        on_actionGo_home_triggered();
    }
    else {
        // dakle imam activityje, ali bez workera
        // STOGA IDEM QUERYATI WORKERE ZA SVAKI ACTIVITY
        // međutim, moram spremiti u memoriju activityje da se ne zgube izvan lifetimea ovog scopea
        QJsonParseError jsonError;
        auto bytes = reply->readAll();
        auto document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isArray()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        auto jsonArray = document.array();
        auto arraySize = jsonArray.size();
        _tempDatabaseActivities.clear();
        _tempDatabaseActivities.reserve(arraySize);
        auto plantList = Util::getPlantList();
        for (const auto& v : jsonArray) {
            auto jsonObj = v.toObject();
            auto id = jsonObj.value("id").toInteger();
            auto gardenId = jsonObj.value("garden_id").toInteger();
            auto plantId = jsonObj.value("plant_id").toInt();

            auto foundPlant = std::optional<Plant>();
            if (plantId < Util::customPlantsMinimumId) {
                foundPlant = plantList[plantId-1];
            }
            for (auto i = Util::customPlantsMinimumId-1; i < plantList.size(); ++i) {
                if (plantList[i].id == id) {
                    //rv.emplace_back(Plant(id, plantList[i].name));
                    foundPlant = Plant(id, plantList[i].name);
                    break;
                }
            }

            auto duration = jsonObj.value("duration").toInt();
            auto repeat = jsonObj.value("repeat").toInt();
            auto date = QDate::fromString(jsonObj.value("date").toString(), "yyyy-MM-dd");
            std::string description = "";
            if (jsonObj.contains("description")) description = Cryptography::decryptAES(QByteArray::fromBase64(jsonObj.value("description").toString().toLocal8Bit()));

            _tempDatabaseActivities.emplace_back(Activity(id, foundPlant.value_or(Plant(id, "Custom Plant")), description, duration, repeat, date, gardenId));
            //ui->listWidget_activities->addItem(QString::fromStdString(plantList[plantId].name + " - " + description));
        }
        // sada su spremljeni activityji i treba se za svakog gettati workere
        _tempRequestsLeft = arraySize;
        connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleWorkersForActivity);
        for (auto i = 0; i < _tempDatabaseActivities.size(); ++i) {
            auto request = QNetworkRequest(QUrl(urlPrefix+"activities/"+QString::number(_tempDatabaseActivities[i].id)+"/workers"));
            request.setHeader(QNetworkRequest::LastModifiedHeader, i);
            requestManager->get(request);
        }
    }
    end:;
        //emit sendActivities(reply->readAll());
    reply->deleteLater();
}

void MainWindow::handleGardenDeletion(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleGardenDeletion);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::error());
    }

    else {
        on_actionGo_home_triggered();
        on_actionView_gardens_triggered();
        Gui::ThrowError(Translate::success());
    }
    reply->deleteLater();
}

void MainWindow::handleActivityDeletion(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleActivityDeletion);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::error());
    }
    reply->deleteLater();
}

void MainWindow::handleActivity(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleActivity);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::error());
    }
    reply->deleteLater();
}

void MainWindow::handleUsersWorkers(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersWorkers);
    std::vector<Worker> workers = {Worker(-100)};
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorGettingResource());
    }
    else {
        auto bytes = reply->readAll();
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(bytes, &error);
        if (error.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isArray()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        workers.clear();
        auto jsonArray = document.array();
        for (const auto& v : jsonArray) {
            auto jsonObj = v.toObject();
            workers.emplace_back(Worker(jsonObj.value("id").toInteger(), Cryptography::decryptRSA(QByteArray::fromBase64(jsonObj.value("name").toString().toUtf8()))));
        }
    }
    end:;
    emit forwardReply(workers);
    reply->deleteLater();
}

void MainWindow::handleUsersWorkersForReport(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleUsersWorkers);
    std::vector<Worker> workers = {Worker(-100)};
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::errorGettingResource());
    }
    else {
        auto bytes = reply->readAll();
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(bytes, &error);
        if (error.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isArray()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        workers.clear();
        auto jsonArray = document.array();
        for (const auto& v : jsonArray) {
            auto jsonObj = v.toObject();
            workers.emplace_back(Worker(jsonObj.value("id").toInteger(), Cryptography::decryptRSA(QByteArray::fromBase64(jsonObj.value("name").toString().toUtf8()))));
        }
    }
    end:;
    emit forwardReplyForReport(workers);
    reply->deleteLater();
}

void MainWindow::handleWorkersForActivity(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonParseError jsonError;
        auto bytes = reply->readAll();
        auto index = reply->request().header(QNetworkRequest::LastModifiedHeader).toInt();
        auto request = reply->request();
        auto document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isArray()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        auto jsonArray = document.array();
        for (const auto& v : jsonArray) {
            auto jsonObj = v.toObject();
            _tempDatabaseActivities[index].workersAssigned.emplace_back(jsonObj.value("id").toInteger(), Cryptography::decryptRSA(QByteArray::fromBase64(jsonObj.value("name").toString().toLocal8Bit())));
        }
    }

    end:;
    if (--_tempRequestsLeft == 0) {
        disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleWorkersForActivity);
        emit sendActivities(_tempDatabaseActivities);
        _tempDatabaseActivities.clear();
        _tempRequestsLeft = -1;
    }
    reply->deleteLater();
}

void MainWindow::handleWorker(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleWorker);
    auto workerToBeForwarded = Worker(-100);
    if (reply->error() == QNetworkReply::NoError) {
        auto bytes = reply->readAll();
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(bytes, &error);
        if (error.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        if (!document.isObject()) {
            Gui::ThrowError(Translate::errorReadingJson());
            goto end;
        }
        auto jsonObj = document.object();
        auto id = jsonObj.value("id").toInteger();
        auto name = Cryptography::decryptRSA(QByteArray::fromBase64(jsonObj.value("name").toString().toLocal8Bit()));
        workerToBeForwarded = Worker(id, name);
    }
    end:;
    emit forwardWorker(workerToBeForwarded);
    reply->deleteLater();
}

void MainWindow::handleValidation(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleValidation);
    if (reply->error() == QNetworkReply::NoError) {
        logIn(reply->readAll());
    }
    reply->deleteLater();
}

void MainWindow::loggedOut(QNetworkReply* reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::loggedOut);
}

void MainWindow::handlePurchase(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handlePurchase);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::error());
    }
    else {
        auto bytes = reply->readAll();
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(bytes,&error);
        if (error.error != QJsonParseError::NoError) {
            Gui::ThrowError(Translate::errorReadingJson());
        }
        else if (!document.isObject()) {
            Gui::ThrowError(Translate::errorReadingJson());
        }
        else {
            Util::writeLocalFile(this->username+"-serial.key", document.object().value("encrypted_serial_key").toString().toLocal8Bit());
            on_actionGo_home_triggered();
        }
    }
    reply->deleteLater();
}

void MainWindow::handlePurchaseValidation(QNetworkReply *reply)
{
    disconnect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handlePurchaseValidation);
    if (reply->error() != QNetworkReply::NoError) {
        Gui::ThrowError(Translate::invalidSerialKey());
        closeCurrentWindow();
        currentWindow = new Purchase();
        ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
        connect(currentWindow, SIGNAL(sendPurchaseDetails(const QString&, const QString&, const QString&)), this, SLOT(receivePurchaseDetails(const QString&,const QString&,const QString&)));
        currentWindow->show();
        //closeCurrentWindow();
    }
    reply->deleteLater();
}


// CLASS FUNCTIONS

void MainWindow::changeFontSize(int size)
{
    auto app = qApp;
    auto f = qApp->font();
    f.setPointSize(size);
    app->setFont(f);
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue("fontSize",size);
}

void MainWindow::changeLanguage(Util::Language language)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue("language",language == Util::Language::CROATIAN?"hr":"en");
    Translate::setLanguage(language);
    updateTranslationsInMainWindow();
}

void MainWindow::changeTheme(bool darkMode)
{
    QFile styleSheetFile(darkMode?darkThemePath:whiteThemePath);
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    auto app = qApp;
    app->setStyleSheet(styleSheet);
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue("darkMode",darkMode);
}

void MainWindow::updateTranslationsInMainWindow()
{
    ui->menuHome->setTitle(Translate::home());
    ui->menuUser->setTitle(Translate::user());
    ui->menuGardens->setTitle(Translate::gardens());
    ui->actionGo_home->setText(Translate::goHome());
    ui->actionSettings->setText(Translate::settings());
    ui->actionLogin->setText(Translate::login());
    ui->actionRegister->setText(Translate::register_());
    ui->actionLogout->setText(Translate::logout());
    ui->actionAdd_garden->setText(Translate::addGarden());
    ui->actionView_gardens->setText(Translate::viewGardens());
    ui->actionCustom_plants->setText(Translate::customPlants());
}

void MainWindow::closeCurrentWindow()
{
    if (currentWindow == nullptr) return;
        currentWindow->close();
        currentWindow = nullptr;
}

void MainWindow::openViewGardensWindow(const QByteArray &json)
{
    closeCurrentWindow();
    currentWindow = new ViewGardens(userId, json, this);
    ui->widgetWindow->addWidget(currentWindow, 0, Qt::AlignmentFlag::AlignVCenter);
    connect(currentWindow, SIGNAL(sendEditGarden(std::int64_t)), this, SLOT(receiveEditGarden(std::int64_t)));
    connect(currentWindow, SIGNAL(sendRequestForActivities(std::int64_t)), this, SLOT(receiveRequestForActivities(std::int64_t)));
    connect(this, SIGNAL(sendActivities(const std::vector<Activity>&)), currentWindow, SLOT(receiveActivities(const std::vector<Activity>&)));
    connect(currentWindow, SIGNAL(sendRequestForGardenDeletion(std::int64_t)), this, SLOT(receiveRequestForGardenDeletion(std::int64_t)));
    connect(currentWindow, SIGNAL(sendRequestForActivityDeletion(std::int64_t)), this, SLOT(receiveRequestForActivityDeletion(std::int64_t)));
    connect(currentWindow, SIGNAL(sendActivity(const Activity&)), this, SLOT(receiveActivity(const Activity&)));
    connect(currentWindow, SIGNAL(sendRequestForUsersWorkers(std::int64_t)), this, SLOT(receiveRequestForUsersWorkers(std::int64_t)));
    connect(this, SIGNAL(forwardReply(const std::vector<Worker>&)), currentWindow, SLOT(receiveUsersWorkers(const std::vector<Worker>&)));
    connect(currentWindow, SIGNAL(sendRequestForUsersWorkersForReport(std::int64_t)), this, SLOT(receiveRequestForUsersWorkersForReport(std::int64_t)));
    connect(this, SIGNAL(forwardReplyForReport(const std::vector<Worker>&)), currentWindow, SLOT(receiveUsersWorkersForReport(const std::vector<Worker>&)));
    connect(currentWindow, SIGNAL(sendWorker(const QString&)), this, SLOT(receiveWorker(const QString&)));
    connect(this, SIGNAL(forwardWorker(const Worker&)), currentWindow, SLOT(receiveWorkerForward(const Worker&)));
    currentWindow->show();
}

void MainWindow::logIn(const QByteArray &bytes)
{
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Gui::ThrowError(Translate::errorReadingJson());
        return;
    }
    if (!document.isObject()) {
        Gui::ThrowError(Translate::errorReadingJson());
        return;
    }
    {
        auto jsonObj = document.object();
        this->userId = jsonObj.value("id").toInteger();
        this->username = jsonObj.value("username").toString();
        this->isAdmin = jsonObj.value("is_admin").toBool();
        this->loggedIn = true;
        auto cookieJar = new GardenCookieJar();
        cookieJar->setCookiesFromUrl(loadCookiesFromCache(), QUrl(urlPrefix));
        requestManager->setCookieJar(cookieJar);
        validatePurchase();
    }
    onLoginOrLogout();
    on_actionGo_home_triggered();
}

void MainWindow::onLoginOrLogout()
{
    ui->actionAdd_garden->setEnabled(loggedIn);
    ui->actionView_gardens->setEnabled(loggedIn);
    ui->actionLogout->setEnabled(loggedIn);
    ui->actionLogin->setEnabled(!loggedIn);
    ui->actionRegister->setEnabled(!loggedIn);
}

void MainWindow::validateUser()
{
    const auto url = QUrl(urlPrefix+"auth/validate");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handleValidation);
    requestManager->post(request, QByteArray{});
}

void MainWindow::deleteSession()
{
    auto request = QNetworkRequest(QUrl(urlPrefix+"auth/logout"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::loggedOut);
    requestManager->post(request, QByteArray());
}

void MainWindow::validatePurchase()
{
    QByteArray key;
    try {
        key = Util::readLocalFile(this->username+"-serial.key");
    } catch (std::exception const&) {}

    auto request = QNetworkRequest(QUrl(urlPrefix+"serial-key/validate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    connect(requestManager, &QNetworkAccessManager::finished, this, &MainWindow::handlePurchaseValidation);
    requestManager->post(request, key);
}

void MainWindow::clearCache()
{
    QFile file(sessionIdPath);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}

QList<QNetworkCookie> MainWindow::loadCookiesFromCache()
{
    QList<QNetworkCookie> rv;
    QFile inputFile(sessionIdPath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList parts = line.split('=', Qt::SkipEmptyParts);
          if (parts.size() != 2) continue;

          auto name = parts[0];
          auto value = parts[1];
          rv.emplace_back(QNetworkCookie(name.toLocal8Bit(),value.toLocal8Bit()));
       }
       inputFile.close();
    }
    return rv;
}

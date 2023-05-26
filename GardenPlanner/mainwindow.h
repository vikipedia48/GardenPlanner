#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commonfunctions.h"
#include "garden.h"
#include "translationstrings.h"
#include <QMainWindow>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString draftFilePath = (QCoreApplication::applicationDirPath()+QDir::separator()+"draft.bin");
    QString customPlantsFilePath = (QCoreApplication::applicationDirPath()+QDir::separator()+"customplants.json");
    QString iniPath = QCoreApplication::applicationDirPath()+QDir::separator()+"config.ini";
    QString whiteThemePath = QCoreApplication::applicationDirPath()+QDir::separator()+"whiteTheme.qss";
    QString darkThemePath = QCoreApplication::applicationDirPath()+QDir::separator()+"darkTheme.qss";
    QString sessionIdPath = QCoreApplication::applicationDirPath()+QDir::separator()+".sessionId";
    QString urlPrefix = "http://127.0.0.1:5001/";

signals:
    void forwardReply(const std::vector<Worker>& workers);
    void forwardReplyForReport(const std::vector<Worker>& workers);
    void sendActivities(const std::vector<Activity> activities);
    void forwardWorker(const Worker& worker);

private slots:

    // slots for ui element on-clicks
    void on_actionGo_home_triggered();
    void on_actionLogin_triggered();
    void on_actionRegister_triggered();
    void on_actionLogout_triggered();
    void on_actionAdd_garden_triggered();
    void on_actionView_gardens_triggered();
    void on_actionCustom_plants_triggered();
    void on_actionSettings_triggered();

    // slots for receving signals in child widgets
    void receiveNewSettings(Util::Language language, int fontSize, bool darkMode);
    void receivePurchaseDetails(const QString &number, const QString &expDate, const QString &secCode);
    void receiveRegisterForm(const QString& username, const QString& password);
    void receiveLoginForm(const QString& username, const QString& password);
    void receiveGarden(const Garden& garden);
    void receiveEditGarden(std::int64_t gardenId);
    void receiveRequestForActivities(std::int64_t gardenId);
    void receiveRequestForGardenDeletion(std::int64_t gardenId);
    void receiveRequestForActivityDeletion(std::int64_t activityId);
    void receiveActivity(const Activity& activity);
    void receiveRequestForUsersWorkers(std::int64_t userId);
    void receiveRequestForUsersWorkersForReport(std::int64_t userId);
    void receiveWorker(const QString& name);

    // slots for handling replies from QNetworkAccessManager
    void handleRegister(QNetworkReply* reply);
    void handleLogIn(QNetworkReply* reply);
    void handleGardenCreation(QNetworkReply* reply);
    void handleEditGarden(QNetworkReply* reply);
    void handleUsersGardens(QNetworkReply* reply);
    void handleRequestForActivities(QNetworkReply* reply);
    void handleGardenDeletion(QNetworkReply* reply);
    void handleActivityDeletion(QNetworkReply* reply);
    void handleActivity(QNetworkReply* reply);
    void handleUsersWorkers(QNetworkReply* reply);
    void handleUsersWorkersForReport(QNetworkReply* reply);
    void handleWorkersForActivity(QNetworkReply* reply);
    void handleWorker(QNetworkReply* reply);
    void handleValidation(QNetworkReply* reply);
    void loggedOut(QNetworkReply* reply);
    void handlePurchase(QNetworkReply* reply);
    void handlePurchaseValidation(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    QWidget* currentWindow;
    QNetworkAccessManager* requestManager;
    bool loggedIn;
    bool isAdmin = false;
    std::int64_t userId;
    QString username = "";
    Translate translate;

    // class functions
    void changeFontSize(int size);
    void changeLanguage(Util::Language language);
    void changeTheme(bool darkMode);
    void updateTranslationsInMainWindow();
    void closeCurrentWindow();
    void openViewGardensWindow(const QByteArray& json);
    void logIn(const QByteArray& json);
    void onLoginOrLogout();
    void validateUser();
    void deleteSession();
    void validatePurchase();
    void clearCache();
    QList<QNetworkCookie> loadCookiesFromCache();

    int _tempRequestsLeft = -1;
    std::vector<Activity> _tempDatabaseActivities;

};
#endif // MAINWINDOW_H

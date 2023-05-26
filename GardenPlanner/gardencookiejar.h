#ifndef GARDENCOOKIEJAR_H
#define GARDENCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QCoreApplication>
#include <QDir>


class GardenCookieJar final : public QNetworkCookieJar
{
public:
    QString sessionIdPath = QCoreApplication::applicationDirPath()+QDir::separator()+".sessionId";
    GardenCookieJar();
    ~GardenCookieJar() override = default;

    // QNetworkCookieJar interface
public:
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override;
    void deleteAllCookies();
    QList<QNetworkCookie> allCookies() const;

};

#endif // GARDENCOOKIEJAR_H

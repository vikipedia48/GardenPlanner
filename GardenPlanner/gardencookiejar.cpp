#include "gardencookiejar.h"
#include "fmt/format.h"
#include "qnetworkcookie.h"

GardenCookieJar::GardenCookieJar()
{

}

bool GardenCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    QFile file(sessionIdPath);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    for (auto& v : cookieList) {
        auto line = v.name()+"="+v.value();
        file.write(line);
    }
    file.close();
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

void GardenCookieJar::deleteAllCookies()
{
    QFile file(sessionIdPath);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}

QList<QNetworkCookie> GardenCookieJar::allCookies() const {
    return QNetworkCookieJar::allCookies();
}

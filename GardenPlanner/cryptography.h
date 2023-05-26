#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <QString>
#include <string>
#include <QCryptographicHash>

namespace Cryptography
{
    static const QByteArray aesDefaultKey = QCryptographicHash::hash("AESCustomKey1234", QCryptographicHash::Sha256);
    static const QByteArray aesDefaultIv = QCryptographicHash::hash("Initialization12", QCryptographicHash::Md5);
    QByteArray encryptAES(const QString& str, QByteArray key = aesDefaultKey, QByteArray iv = aesDefaultIv);
    std::string decryptAES(const QByteArray& str, QByteArray key = aesDefaultKey, QByteArray iv = aesDefaultIv);
    QByteArray encryptRSA(const std::string& string, QByteArray genesis = "RSAKeyGenesis123");
    std::string decryptRSA(const QByteArray &encodedMsg, QByteArray genesis = "RSAKeyGenesis123");
};

#endif // CRYPTOGRAPHY_H

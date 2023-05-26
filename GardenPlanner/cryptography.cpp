#include "cryptography.h"
#include "commonfunctions.h"
#include "qtfunctions.h"
#include <qrsaencryption.h>
#include <qaesencryption.h>
#include <optional>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include "fmt/format.h"

QByteArray Cryptography::encryptAES(const QString& str, QByteArray key, QByteArray iv)
{
    QByteArray encodeText;
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC);

    encodeText = encryption.encode(str.toUtf8(), key, iv);
    return encodeText;
}

std::string Cryptography::decryptAES(const QByteArray &str, QByteArray key, QByteArray iv)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC);
    auto decoded = encryption.decode(QByteArray(str),key,iv);
    return encryption.removePadding(decoded).toStdString();
}

QByteArray Cryptography::encryptRSA(const std::string &string, QByteArray genesis)
{
    QByteArray pub, priv;
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_64);
    e.generatePairKey(pub, priv, genesis);

    QByteArray msg(string.c_str());
    auto encryptMessage = e.encode(msg, pub);
    return encryptMessage;
}

std::string Cryptography::decryptRSA(const QByteArray &encodedMsg, QByteArray genesis)
{
    QByteArray pub, priv;
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_64);
    e.generatePairKey(pub, priv, genesis);
    auto decodedMessage = e.decode(encodedMsg, priv);
    return decodedMessage.toStdString();
}

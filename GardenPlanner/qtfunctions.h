#ifndef QTFUNCTIONS_H
#define QTFUNCTIONS_H

#include <QString>

namespace Gui {
QString GetSavePath(const QString& tip);
QString GetInputPath(const QString& windowName,const QString& tip);
QString GetStringValueFromInputDialog(const QString& title, const QString& desc, QString placeholder = "");
double GetNumberValueFromInputDialog(const QString& title, const QString& desc);
void ThrowError(const QString& msg);
void PrintMessage(const QString& title, const QString& msg);
bool GiveQuestion(const QString& question);
}


#endif // QTFUNCTIONS_H

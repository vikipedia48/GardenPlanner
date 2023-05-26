#ifndef WAITSCREEN_H
#define WAITSCREEN_H

#include <QWidget>

namespace Ui {
class WaitScreen;
}

class WaitScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WaitScreen(QWidget *parent = nullptr);
    ~WaitScreen();
public slots:
    void closeWindow();
private:
    Ui::WaitScreen *ui;
};

#endif // WAITSCREEN_H

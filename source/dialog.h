#ifndef DIALOG_H
#define DIALOG_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QtUIUtils/checkvisibilitygroup.h>

namespace Ui {
class Dialog;
}

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

    const int FPS = 60;
    uint countdown_interval;

    QTimer countdown_timer;
    QElapsedTimer countup_timer;
    QTimer update_timer;

    QtUIUtils::CheckVisibilityGroup time_controls;

    uint getMilliseconds();
    void setMilliseconds(uint time);

    void enableUI(bool enable = true);

private slots:

    void mainLoop();
    void timeout();
    void startCountup();
    void startCountdown();
    void stop();

};

#endif // DIALOG_H

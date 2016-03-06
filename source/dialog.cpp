#include "dialog.h"
#include "ui_dialog.h"
#include <QtMultimedia/QSound>

namespace Ui
{

// a quick and dirty fix for Qt's automated screwup
class DialogFixed : public Dialog
{

public:
    DialogFixed(QMainWindow* window)
    {
        setupUi(window);
    }
};

}

Dialog::Dialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DialogFixed(this)),
    time_controls(ui->time_controls_check)
{
//    ui->setupUi(this);

//    time_controls = QtUIUtils::CheckVisibilityGroup(ui->time_controls_check);

    // should be a better way to do this
    for(int i = 0; i < 4; ++i)
    {
        QDial *dial = findChild<QDial*>("dial_" + QString::number(i));
        QSpinBox *spinBox = findChild<QSpinBox*>(QString("spinBox_") + QString::number(i));
        QLCDNumber *lcdNumber = findChild<QLCDNumber*>(QString("lcdNumber_") + QString::number(i));
        connect(dial, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        connect(dial, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        connect(spinBox, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));

        time_controls.add(spinBox);
        time_controls.add(dial);
    };

    update_timer.setInterval(1000/FPS);
    connect(&update_timer, SIGNAL(timeout()), this, SLOT(mainLoop()));

    countdown_timer.setSingleShot(true);
    connect(&countdown_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    countup_timer.invalidate();

    connect(ui->btnDown, SIGNAL(clicked()), this, SLOT(startCountdown()));
    connect(ui->btnUp, SIGNAL(clicked()), this, SLOT(startCountup()));
    connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::mainLoop()
{
    if(countdown_timer.isActive())
    {
        setMilliseconds(countdown_timer.remainingTime());
    }
    else if(countup_timer.isValid())
    {
        setMilliseconds(countup_timer.elapsed());
    }
}

void Dialog::timeout()
{
    setMilliseconds(countdown_interval);
    QSound::play("melno.wav");

    raise();
    activateWindow();

    update_timer.stop();
    enableUI();
}

uint Dialog::getMilliseconds()
{
    return ((ui->dial_0->value()*60 + ui->dial_1->value()) * 60 + ui->dial_2->value()) * 1000 + ui->dial_3->value();
}

void Dialog::setMilliseconds(uint time)
{
    ui->dial_3->setValue(time%1000);
    time /= 1000;
    ui->dial_2->setValue(time%60);
    time /= 60;
    ui->dial_1->setValue(time%60);
    time /= 60;
    ui->dial_0->setValue(time);
}

void Dialog::enableUI(bool enable)
{
    // should be a better way to do this
//    for(int i = 0; i < 4; ++i)
//    {
//        QDial *dial = findChild<QDial*>("dial_" + QString::number(i));
//        QSpinBox *spinBox = findChild<QSpinBox*>(QString("spinBox_") + QString::number(i));
//        dial->setEnabled(enable);
//        spinBox->setEnabled(enable);
//    };
    time_controls.enable(enable);
    ui->btnDown->setEnabled(enable);
    ui->btnUp->setEnabled(enable);
}

void Dialog::startCountup()
{
    countup_timer.start();
    update_timer.start();
    enableUI(false);
}

void Dialog::startCountdown()
{
    countdown_interval = getMilliseconds();
    countdown_timer.setInterval(countdown_interval);
    countdown_timer.start();
    update_timer.start();
    enableUI(false);
}

void Dialog::stop()
{
    if(countdown_timer.isActive())
    {
        setMilliseconds(countdown_timer.remainingTime());
        countdown_timer.stop();
    }
    else if(countup_timer.isValid())
    {
        setMilliseconds(countup_timer.elapsed());
        countup_timer.invalidate();
    }

    update_timer.stop();
    enableUI();
}



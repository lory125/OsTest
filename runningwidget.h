#ifndef RUNNINGWIDGET_H
#define RUNNINGWIDGET_H
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "process.h"
#include <QProgressBar>
#include <QTimer>
class runningWidget : public QWidget
{
    Q_OBJECT
public:
    explicit runningWidget(QWidget *parent = nullptr);
    QLabel *label1;
    list<process> *forReady;
    QProgressBar *pBar;
    QTimer *timer;
    QTimer *Ptimer;

    void dispath();
    int count=0;

signals:
    void oneDone();
    void ResetTable();
public slots:
    void setPtimer();
    void setForcheck(list<process>* e);
    void nextProcess();

};

#endif // RUNNINGWIDGET_H

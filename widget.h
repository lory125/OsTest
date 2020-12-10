#ifndef WIDGET_H
#define WIDGET_H
#include <QDebug>
#include "backupwidget.h"
#include "readywidget.h"
#include "runningwidget.h"
#include "suspendwidget.h"

#include <QPushButton>
#include <QTime>
#include <QWidget>
#include <QPalette>
#include <QGridLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    list<process>* readyProcess;
    backUpWidget *backW;
    readyWidget  *readyW;
    runningWidget *runW;
    suspendWidget *susW;
    QGridLayout *layout;

public slots:
    //void ResetBKtable();
};
#endif // WIDGET_H

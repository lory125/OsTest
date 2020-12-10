#ifndef BACKUPWIDGET_H
#define BACKUPWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include "process.h"

class backUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit backUpWidget(QWidget *parent = nullptr);
    list<process> *readyProcess;
    QTableView *table;
    QStandardItemModel *mod;
    process *p=new process();

signals:

public:
    void setTable();
    void setProcess(list<process>* e);

public slots:
    void ResetTable(list<process> *e);
    void generateProcess();
};

#endif // BACKUPWIDGET_H

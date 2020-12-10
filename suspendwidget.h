#ifndef SUSPENDWIDGET_H
#define SUSPENDWIDGET_H
#include "process.h"
#include <QPushButton>
#include <QWidget>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QLabel>
#include <list>
class suspendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit suspendWidget(QWidget *parent = nullptr);
    QTableView *table;
    QStandardItemModel *mod;
    list<process> *suspendProcess;
    void reSetTable();
signals:
    void ReReady(process *);


public slots:
    void setSuspend(process*);
    void getTableRow();

};

#endif // SUSPENDWIDGET_H

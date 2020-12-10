#include "backupwidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
backUpWidget::backUpWidget(QWidget *parent) : QWidget(parent)
{

//    p->setList(readyProcess);


    qDebug()<<"readyProcess=readyP;";
    QLabel *label =new QLabel("Back Up",this);
    label->setGeometry(250,0,100,50);

    QPushButton *p1=new QPushButton("Generate Process",this);
    p1->setGeometry(100,400,150,30);

    QPushButton *p2=new QPushButton("run",this);
    p2->setGeometry(300,400,150,30);
    p2->setVisible(false);

    this->setStyleSheet("background-color:rgb(255,255,255)");


    /*设置表格*/
    {
    table = new QTableView(this);
    table->setGeometry(0,50,550,340);
    mod = new QStandardItemModel;
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //p->generate();//****************************
    //setTable();//***********************************
    table->setModel(mod);
    }

    connect(p1,SIGNAL(clicked()),this,SLOT(generateProcess()));
}

void backUpWidget::setTable(){
    mod->clear();
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    list<process>::iterator iter = readyProcess->begin();
    for(int i=0;i<(int)p->readyProcess->size();i++){
        mod->setItem(i, 0,new QStandardItem(QString::number(iter->processPCB.processID)));
        mod->setItem(i, 1, new QStandardItem(QString::number(iter->processPCB.priority)));
        mod->setItem(i, 2, new QStandardItem(QString::number(iter->processPCB.needTime)));
        iter++;
    }
    table->setModel(mod);
}
void backUpWidget::ResetTable(list<process> *e){
    mod->clear();
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    list<process>::iterator iter = e->begin();
    qDebug()<<"重设表格";
    for(int i=0;i<(int)e->size();i++){
        mod->setItem(i, 0,new QStandardItem(QString::number(iter->processPCB.processID)));
        mod->setItem(i, 1, new QStandardItem(QString::number(iter->processPCB.priority)));
        mod->setItem(i, 2, new QStandardItem(QString::number(iter->processPCB.needTime)));
        iter++;
    }
    table->setModel(mod);
}
void backUpWidget::generateProcess(){
    qDebug()<<"generte";
    p->generate();
    setTable();
}



void backUpWidget::setProcess(list<process> *e)
{
    qDebug() << "第一步传参成功";
    readyProcess = e;
}

#include "widget.h"
#include <list>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(1400,900);
    this->setStyleSheet("background-color:rgb(255,255,255)");
    layout = new QGridLayout(this);

    readyProcess = new list<process>();

    backW = new backUpWidget(this);
    backW->setProcess(readyProcess);
    backW->p->setList(readyProcess);

    readyW = new readyWidget(this);
    readyW->setList(readyProcess);

    runW = new runningWidget(this);

    susW = new suspendWidget(this);

    connect(this->readyW,SIGNAL(reSetBKtable(list<process> *)),backW,SLOT(ResetTable(list<process> *)));
    connect(this->readyW,SIGNAL(passForcheck(list<process> *)),runW,SLOT(setForcheck(list<process> *)));
    connect(this->runW,SIGNAL(oneDone()),readyW,SLOT(oneFinished()));
    connect(this->readyW,SIGNAL(passSusProcess(process *)),susW,SLOT(setSuspend(process *)));
    connect(this->susW,SIGNAL(ReReady(process *)),readyW,SLOT(ReReadyP(process *)));
    connect(runW,SIGNAL(ResetTable()),readyW,SLOT(ResetTable()));
    //布局

   {
    layout->addWidget(backW,0,0,1,1);
    layout->addWidget(readyW,0,1,1,1);
    layout->addWidget(runW,1,0,1,1);
    layout->addWidget(susW,1,1,1,1);
    layout->rowMinimumHeight(450);
    layout->columnMinimumWidth(600);
    //layout->addWidget(p1,1,0,1,1);
    this->setLayout(layout);
}

}

Widget::~Widget()
{
}


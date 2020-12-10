#include "suspendwidget.h"
#include <QDebug>
suspendWidget::suspendWidget(QWidget *parent) : QWidget(parent)
{
    suspendProcess = new list<process>();
    QLabel *label =new QLabel("Suspend",this);
    label->setGeometry(250,0,100,50);

    QPushButton *p1=new QPushButton("Generate Process",this);
    p1->setGeometry(100,400,150,30);
    p1->hide();

    QPushButton *p2=new QPushButton("run",this);
    p2->setGeometry(300,400,150,30);
    p2->hide();

    /*设置表格*/
    {
    table = new QTableView(this);
    table->setGeometry(0,50,550,340);
    mod = new QStandardItemModel;
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setModel(mod);
    }

    connect(table,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(getTableRow()));
}

void suspendWidget::setSuspend(process *p)
{
    suspendProcess->push_back(*p);
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    mod->setItem(mod->rowCount(), 0,new QStandardItem(QString::number(p->processPCB.processID)));
    mod->setItem(mod->rowCount()-1, 1, new QStandardItem(QString::number(p->processPCB.priority)));
    mod->setItem(mod->rowCount()-1, 2, new QStandardItem(QString::number(p->processPCB.needTime)));
    table->setModel(mod);
    //mod->clear();

}

void suspendWidget::getTableRow()
{
    int row = table->currentIndex().row();
    qDebug()<<"行号：   "<<row;
        list<process>::iterator i = suspendProcess->begin();
        for (int j=0;j<row;j++) {
            i++;
        }
        process *p = new process;
        *p=*i;
        ReReady(p);
        qDebug()<<i->processPCB.processID;
        suspendProcess->erase(i);
        reSetTable();

}
void suspendWidget::reSetTable(){
    {//**********************重新设置表格
    mod->clear();
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间"});
    list<process>::iterator iter = suspendProcess->begin();
    for(int i=0;i<(int)suspendProcess->size();i++){
        /* 加载第一列(ID)数据 */
         mod->setItem(i, 0,new QStandardItem(QString::number(iter->processPCB.processID)));
        /* 加载第二列(priority)数据 */
        mod->setItem(i, 1, new QStandardItem(QString::number(iter->processPCB.priority)));
        /* 加载第三列(status)数据 */
        mod->setItem(i, 2, new QStandardItem(QString::number(iter->processPCB.needTime)));
        iter++;
    }
    table->setModel(mod);
    }
}


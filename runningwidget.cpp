#include "runningwidget.h"
#include <QDebug>
#include <QTimerEvent>

runningWidget::runningWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *label =new QLabel("Running",this);
    label1 = new QLabel("process Data:  ",this);
    label1->setGeometry(20,100,400,30);
    label1->setStyleSheet("QLabel{background:#FFFF60;}");
    label->setGeometry(250,0,100,50);
    timer = new QTimer;
    Ptimer = new QTimer;

    QPushButton *p1=new QPushButton("Generate Process",this);
    p1->setGeometry(100,400,150,30);
    p1->hide();

    QPushButton *p2=new QPushButton("run",this);
    p2->setGeometry(300,400,150,30);
    p2->hide();

    pBar = new QProgressBar(this);
    pBar->setGeometry(200,150,300,30);
    pBar->setMinimum(0);
    pBar->setMaximum(100);
    pBar->setValue(0);
    pBar->setFormat("%p%");
    pBar->setVisible(false);

    connect(timer,SIGNAL(timeout()),this,SLOT(nextProcess()));
    connect(Ptimer,SIGNAL(timeout()),this,SLOT(setPtimer()));
}

void runningWidget::setForcheck(list<process> *e)
{
    qDebug()<<"forcheck OK";
    forReady=e;
    dispath();
}

void runningWidget::nextProcess()       //timer激活
{
    if(forReady->front().remainigTime-1<=0){
        qDebug()<<"done one";
        count=0;
//        forReady->pop_front();//cpu调度完成，清除该进程
        timer->stop();
        Ptimer->stop();
        oneDone();
    }else{
        count=0;
        if(forReady->front().processPCB.priority>1)
            forReady->front().processPCB.priority--;
        timer->stop();
        Ptimer->stop();
        ResetTable();
        dispath();
        //重设表格

    }

}

void runningWidget::dispath(){
    if(!forReady->empty())      //当就绪队列里面还有进程等待调度的时候
    {
        timer->stop();
        if(forReady->front().processPCB.s == process::PCB::Suspand){
            process *p;
            p=&forReady->front();
            forReady->push_back(*p);
            forReady->pop_front();
        }
        else
        {
            forReady->front().processPCB.s = process::PCB::Running;
            QString s="process Data:  ID:"+QString::number(forReady->front().processPCB.processID) + "   优先级： " + QString::number(forReady->front().processPCB.priority)+"  时间：  "+QString::number(forReady->front().processPCB.needTime)+"  剩余时间：  "+QString::number(forReady->front().remainigTime);
            label1->setText(s);
            timer->setInterval(1000);

            pBar->setMaximum(forReady->front().processPCB.needTime);
            pBar->setValue(forReady->front().processPCB.needTime-forReady->front().remainigTime);
            qDebug()<<QString::number(forReady->front().processPCB.needTime);
            Ptimer->start(500);
            timer->start();
            qDebug()<<"timer start";
        }
    }
}


void runningWidget::setPtimer(){ // 
        count++;
        forReady->front().remainigTime--;
        ResetTable();
        pBar->setValue(forReady->front().processPCB.needTime-forReady->front().remainigTime);
        qDebug()<<count;

}


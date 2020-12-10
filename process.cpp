#include "process.h"
#include <QDebug>
    registerInfo cpuRegister(0, 0);//设置一个cpu寄存器用来交换
process::process(){

//    readyProcess = new list<process>();

    }
process::process(int processID, int priority, int needTime,int psw, int pc,int arriveTime, int burstTime,int size) {
    processPCB.processID = processID;
    processPCB.priority = priority;
    processPCB.needTime = needTime;
    processPCB.size = size;
    remainigTime = processPCB.needTime;

    processPCB.processRegister.set(psw, pc);
    this->arriveTime = arriveTime;
    this->burstTime = burstTime;
    this->remainigTime = burstTime;//创建进程的时候剩余时间就是阵发时间
    this->turnaroundTime = 0;
    }
bool process::Find(int ID) {//查找ID是否已经存在
qDebug()<<"FindS";
        for (list<process>::iterator i = readyProcess->begin(); i != readyProcess->end(); i++) {
qDebug()<<"for";
            if (i->processPCB.processID == ID)
qDebug()<<"return true";
                return true;
        }
qDebug()<<"return false";
        return false;
    }
void process::setList(list<process> *l)
{
    qDebug() << "第二步传参成功";

    readyProcess = l;
}

void process::generate(){
    process* p;
            int id = 0;

            for (int i = 0; i < 10; i++) {
                id = rand();

                p = new process(id, rand() % 10+1,rand() % 10 + 1, 0, 0, rand() % 5, 10 + rand() % 20,rand() % 30+10);/*rand() % 30+10*/
                readyProcess->push_back(*p);
            }
}

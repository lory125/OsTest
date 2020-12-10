#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <list>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <functional>
using namespace std;

class registerInfo {
public:
    int psw;//程序状态字
    int pc;//程序计数器
public:
    registerInfo() {}//default
    registerInfo(int psw, int pc) {//constructor
        this->psw = psw;
        this->pc = pc;
    }
    void set(int psw, int pc) {//set info
        this->psw = psw;
        this->pc = pc;
    }
    void switchRegister(registerInfo& p) {
        this->psw = p.psw;
        this->pc = p.pc;
    }
};
class process
{
public:
    process();
    process(int processID, int priority, int needTime, int psw, int pc,int arriveTime, int burstTime,int size);

    class PCB {
    public:
        enum Status{Backup=0,Ready=1,Running=2,Suspand=3};
        //string processStatus;//进程状态
        Status s=Ready;
        int processID;//进程ID
        int priority;//优先级
        int needTime;
        int size;
        registerInfo processRegister;//寄存器级上下文

    }processPCB;

public:

    int arriveTime;//进程到达时间
    int burstTime;//进程阵发时间
    int turnaroundTime;//一个进程的周转时间
    int remainigTime;//剩余时间
    list<process> *readyProcess;//就绪队列
    list<process>forCheck;//一个临时的检查队列


    void generate();
    void setList(list<process>* l);
    bool Find(int);
};

#endif // PROCESS_H

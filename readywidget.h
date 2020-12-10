#ifndef READYWIDGET_H
#define READYWIDGET_H
/******************************/

#define MIN_SLICE    10            /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 128       /*默认内存的大小*/
#define DEFAULT_MEM_START 1025        /*默认内存的起始位置*/

/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
/*********************************/
#include <QPushButton>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <process.h>
#include <QTextEdit>
extern int maxProcess;
class readyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit readyWidget(QWidget *parent = nullptr);
    list<process> *readyprocess; //后备任务；
    list<process> *forCheck = new list<process>(); //进入ready的任务；

    QTextEdit * textArea;
    QTableView *table;
    QStandardItemModel *mod;

/*************************/
    int mem_size = DEFAULT_MEM_SIZE;        /*内存大小*/
    int ma_algorithm = MA_BF;            /*当前分配算法*/
    int flag = 0;
    int algorithm;

    struct free_block_type {
        int size;
        int start_addr;
        struct free_block_type* next;
    };
    struct free_block_type* free_block;/*指向内存快空闲链表首指针*/

    struct allocated_block {/*每个进程分配到内存快*/
        int pid;
        int size;
        int start_addr;
        struct allocated_block* next;
    };
    struct allocated_block* allocated_block_head = NULL;

    /*******************/
    void setList(list<process> *e);
    void setForCheck();
    void listSort();
    void reSetTable();
    /*******************/
    struct free_block_type* init_free_block(int mem_size);//初始化内存空间
    struct allocated_block* find_process(int pid);
    void rearrange_BF();
    void rearrange_FF();
    void rearrange_WF();
    void rearrange(int algorithm);
    void set_algorithm();

    int new_process(int id,int size);
    int allocate_mem(struct allocated_block* ab);
    int free_mem(struct allocated_block* ab);
    int dispose(struct allocated_block* free_ab);
    void swap(int* p, int* q) {
        int temp;
        temp = *p;
        *p = *q;
        *q = temp;
        return;
    }
    void kill_process(int id);
    int display_mem_usage();
    /**************************/
signals:
    void reSetBKtable(list<process>*);
    void passForcheck(list<process>*);
    void passSusProcess(process *);

public slots:
    void setReady();
    void oneFinished();
    void getTableRow();
    void ReReadyP(process *);
    void ResetTable();

};

//对链表进行排序
template<char T, int U>//T表示大小，U标志以谁为依据
class greaterOrLess {
public:
    bool operator()(process& t1, process& t2) {
        int a = 0, b = 0;
        switch (U)
        {
        default:
            break;
        case 0://按照进程创建时间的先后
            a = t1.arriveTime;
            b = t2.arriveTime;
            break;
        case 1://按照进程阵发时间的大小
            a = t1.burstTime;
            b = t2.burstTime;
            break;
        case 2://按照进程优先级的大小
            a = t1.processPCB.priority;
            b = t2.processPCB.priority;
            break;
        }

        switch (T)
        {
        default:
            break;
        case '<':
            return a < b;
        case '>':
            return a > b;
        }
    }
};

#endif // READYWIDGET_H

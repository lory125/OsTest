#include "readywidget.h"
#include <QGridLayout>
#include <QLabel>
#include "process.h"
#include <QDebug>
int maxProcess = 0;
readyWidget::readyWidget(QWidget *parent) : QWidget(parent)
{
    free_block = init_free_block(mem_size);

    QLabel *label =new QLabel("Ready",this);
    label->setGeometry(250,0,100,50);

    QPushButton *p1=new QPushButton( "Run",this);
    p1->setGeometry(100,400,150,30);

    QPushButton *p2=new QPushButton("run",this);
    p2->setGeometry(300,400,150,30);
    p2->hide();


    connect(p1,SIGNAL(clicked()),this,SLOT(setReady()));

    /*设置表格*/
    {
    table = new QTableView(this);
    table->setGeometry(0,50,450,340);
    mod = new QStandardItemModel;

    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间","剩余时间","内存大小","起始位置"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setModel(mod);
    }

    textArea = new QTextEdit(this);
    textArea->setGeometry(460,50,200,340);


    connect(table,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(getTableRow()));
}

//设置进入ready的任务
void readyWidget::setForCheck(){
//    process* p = NULL;
    if(maxProcess==0 && readyprocess->size()!=0){

        list<process>::iterator i = readyprocess->begin();
        for(;maxProcess<6 ;maxProcess++){
            qDebug()<<i->remainigTime;//*********************
            i->remainigTime=i->processPCB.needTime;
            int isSuccess = new_process(readyprocess->front().processPCB.processID,readyprocess->front().processPCB.size);
            if(isSuccess==-1)
                break;
            forCheck->push_back(readyprocess->front());
            readyprocess->erase(i);
            i = readyprocess->begin();

        }
    }else{
        qDebug()<<"start while";
        list<process>::iterator i = readyprocess->begin();
        while (maxProcess!=6 && readyprocess->size()!=0) {
            qDebug()<<i->remainigTime;
            i->remainigTime=i->processPCB.needTime;
            qDebug()<<i->remainigTime<<"****";
            int isSuccess = new_process(readyprocess->front().processPCB.processID,readyprocess->front().processPCB.size);
            if(isSuccess==-1)
                break;
            forCheck->push_back(readyprocess->front());
            readyprocess->erase(i);
            i = readyprocess->begin();
            maxProcess++;
        }
    }

    listSort();
    reSetTable();
}

void readyWidget::listSort(){
    forCheck->sort(greaterOrLess<'>', 2>());
}

void readyWidget::reSetTable(){
    {//**********************重新设置表格
    mod->clear();
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间","剩余时间","起始位置","内存大小"});
    list<process>::iterator iter = forCheck->begin();
    struct allocated_block* ab;
    for(int i=0;i<(int)forCheck->size();i++){
        /* 加载第一列(ID)数据 */
         mod->setItem(i, 0,new QStandardItem(QString::number(iter->processPCB.processID)));
        mod->setItem(i, 1, new QStandardItem(QString::number(iter->processPCB.priority)));
        mod->setItem(i, 2, new QStandardItem(QString::number(iter->processPCB.needTime)));
        mod->setItem(i, 3, new QStandardItem(QString::number(iter->remainigTime)));
        ab = find_process(iter->processPCB.processID);
        if(ab!=nullptr)
        {
            mod->setItem(i, 4, new QStandardItem(QString::number(ab->start_addr)));
            mod->setItem(i, 5, new QStandardItem(QString::number(ab->size)));
        }
        iter++;
    }
    table->setModel(mod);
    reSetBKtable(readyprocess);
    passForcheck(forCheck);
    display_mem_usage();
    }
}

void readyWidget::getTableRow()
{
    int row = table->currentIndex().row();
    qDebug()<<"行号：   "<<row;
    if(row!=0){
        list<process>::iterator i = forCheck->begin();
        for (int j=0;j<row;j++) {
            i++;
        }
        process *p = new process;
        *p=*i;
        passSusProcess(p);
        qDebug()<<i->processPCB.processID;
        forCheck->erase(i);
        reSetTable();
    }
}

void readyWidget::ReReadyP(process *p)
{
    forCheck->push_back(*p);

    listSort();
    reSetTable();
}

void readyWidget::ResetTable()
{
    listSort();
    mod->clear();
    mod->setHorizontalHeaderLabels({"ID","优先级","所需时间","剩余时间","起始位置","内存大小"});
    list<process>::iterator iter = forCheck->begin();
    struct allocated_block* ab;
    for(int i=0;i<(int)forCheck->size();i++){
        /* 加载第一列(ID)数据 */
        mod->setItem(i, 0,new QStandardItem(QString::number(iter->processPCB.processID)));
        mod->setItem(i, 1, new QStandardItem(QString::number(iter->processPCB.priority)));
        mod->setItem(i, 2, new QStandardItem(QString::number(iter->processPCB.needTime)));
        mod->setItem(i, 3, new QStandardItem(QString::number(iter->remainigTime)));
        ab = find_process(iter->processPCB.processID);
        if(ab!=nullptr)
        {
            mod->setItem(i, 4, new QStandardItem(QString::number(ab->start_addr)));
            mod->setItem(i, 5, new QStandardItem(QString::number(ab->size)));
        }
        iter++;
    }
    table->setModel(mod);
    display_mem_usage();
}

//传递后备队列
void readyWidget::setList(list<process> *e){
    readyprocess = e;
}

//运行完毕一个
void readyWidget::setReady(){
    setForCheck();
}

void readyWidget::oneFinished(){
    maxProcess--;
    kill_process(forCheck->front().processPCB.processID);
    forCheck->pop_front();//cpu调度完成，清除该进程
    setForCheck();
}

/**
 * 内存管理部分
 *
 */

//初始化内存空间
readyWidget::free_block_type *readyWidget::init_free_block(int mem_size)
{
    struct free_block_type* fb;                         //申请空间
       fb = (struct free_block_type*)malloc(sizeof(struct free_block_type));
       if (fb == NULL) {
           printf("No mem\n");
           return NULL;
       }
       fb->size = mem_size;
       fb->start_addr = DEFAULT_MEM_START;
       fb->next = NULL;
       return fb;
}

/*按FF算法重新整理内存空闲块链表*/
void readyWidget::rearrange_FF() {
    struct free_block_type* tmp, * work;
    qDebug()<<"Rearrange free blocks for FF \n";
    tmp = free_block;
    while (tmp != NULL)
    {
        work = tmp->next;
        while (work != NULL)
        {
            if (work->start_addr < tmp->start_addr)
            {
                /*地址递增*/
                swap(&work->start_addr, &tmp->start_addr);
                swap(&work->size, &tmp->size);
            }
            else work = work->next;
        }
        tmp = tmp->next;
    }
}

/*按BF算法重新整理内存空闲块链表*/
void readyWidget::rearrange_BF() {
    struct free_block_type* tmp, * work;
    qDebug()<<"Rearrang"
              ".e free blocks for BF \n";
    tmp = free_block;
    while (tmp != NULL)
    {
        work = tmp->next;
        while (work != NULL) {
            if (work->size > tmp->size) { /*空间递增*/
                swap(&work->start_addr, &tmp->start_addr);
                swap(&work->size, &tmp->size);
            }
            else            work = work->next;
        }
        tmp = tmp->next;
    }

}

/*按WF算法重新整理内存空闲块链表*/
void readyWidget::rearrange_WF() {
    struct free_block_type* tmp, * work;
    qDebug()<<"Rearrange free blocks for wF \n";
    tmp = free_block;
    while (tmp != NULL)
    {
        work = tmp->next;
        while (work != NULL) {
            if (work->size < tmp->size) {/*空间递减*/
                swap(&work->start_addr, &tmp->start_addr);
                swap(&work->size, &tmp->size);
            }
            else work = work->next;
        }
        tmp = tmp->next;
    }
}
//按指定算法重新排列空闲区链表
void readyWidget::rearrange(int algorithm) {
    switch (algorithm) {
    case MA_FF:  rearrange_FF(); break;
    case MA_BF:  rearrange_BF(); break;
    case MA_WF:  rearrange_WF(); break;
    }
}

//选择当前的分配算法
void readyWidget::set_algorithm() {
    int algorithm=2;
    ma_algorithm = algorithm;
    rearrange(ma_algorithm);
}

/*将ab所表示的已分配区归还，并进行可能的合并*/
int readyWidget::free_mem(struct allocated_block* ab) {
    int algorithm = ma_algorithm;
    struct free_block_type*fbt, *work;

   // fbt = (struct free_block_type*)malloc(sizeof(struct  free_block_type));
    fbt = new free_block_type;
    if (!fbt)
        return -1;
    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    fbt->next = free_block;
    free_block = fbt;
    rearrange(MA_FF);
    fbt = free_block;
    while (fbt != NULL) {
        work = fbt->next;
        if (work != NULL) {
            /*如果区后面当前空与闲的空闲区相连，则合并*/
            if (fbt->start_addr + fbt->size == work->start_addr) {
                fbt->size += work->size;
                fbt->next = work->next;
                free(work);
                continue;
            }
        }
        fbt = fbt->next;

    }
    rearrange(algorithm); /*重新按当前的算法排列空闲区*/
    return 1;
}

//分配内存空间
int readyWidget::allocate_mem(struct allocated_block* ab) {
    struct free_block_type* fbt, * pre, * temp, * work;
    int request_size = ab->size;
    fbt = pre = free_block;
    while (fbt != NULL) {
        if (fbt->size >= request_size) {
            if (fbt->size - request_size >= MIN_SLICE) {
                mem_size -= request_size;
                fbt->size -= request_size;
                ab->start_addr = fbt->start_addr;
                fbt->start_addr = ab->start_addr + request_size;
            }
            else if (fbt->size - request_size <= MIN_SLICE && (fbt->size - request_size >= 0)) {/*分割后空闲区成为小碎片，一起分配*/
                mem_size -= fbt->size;
                ab->size=fbt->size;
                ab->start_addr = fbt->start_addr;
                fbt->start_addr += fbt->size;
                fbt->size=0;
                pre = fbt->next;
                //free(fbt);
                delete (fbt);
                free_block = pre;

            }
            else {
                temp = free_block;
                while (temp != NULL) {
                    work = temp->next;
                    if (work != NULL) {
                        if (temp->start_addr + temp->size == work->start_addr) {//当前空闲区与后面空闲区相连则合并
                            temp->size += work->size;
                            temp->next = work->next;
                            free(work);
                            continue;
                        }
                    }

                    temp = temp->next;
                }
                fbt = free_block;
                break;

            }
            rearrange(ma_algorithm);
            return 1;
        }
        pre = fbt;
        fbt = fbt->next;

    }
    return -1;
}

//加入新进程
int readyWidget::new_process(int id,int size) {
    struct allocated_block* ab;
    int ret;
    ab = (struct allocated_block*)malloc(sizeof(struct allocated_block));
    if (!ab)
        exit(-5);
    ab->next = NULL;
    ab->pid = id;
    if (size > 0)
        ab->size = size;
    ret = allocate_mem(ab);
    if ((ret == 1) && (allocated_block_head == NULL)) {
        allocated_block_head = ab;
        return 1;
    }
    /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret == 1) {
        ab->next = allocated_block_head;
        allocated_block_head = ab;
        return 2;
    }
    else if (ret == -1) { /*分配不成功*/
        free(ab);
        return -1;
    }
    return 3;
}

/*将ab所表示的已分配区归还，并进行可能的合并*/
int  readyWidget::free_men(struct allocated_block* ab) {
    int algorithm = ma_algorithm;
    struct free_block_type* fbt, * work;
    fbt = (struct free_block_type*)malloc(sizeof(struct  free_block_type));
    if (!fbt) return -1;
    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    /*插入到空闲区链表的头部并将空闲区按地址递增的次序排列*/
    fbt->next = free_block;
    free_block = fbt;
    rearrange(MA_FF);
    fbt = free_block;
    while (fbt != NULL) {
        work = fbt->next;
        if (work != NULL) {
            /*如果当前空闲区与后面的空闲区相连，则合并*/
            if (fbt->start_addr + fbt->size == work->start_addr) {
                fbt->size += work->size;
                fbt->next = work->next;
                free(work);
                continue;
            }
        }
        fbt = fbt->next;
    }
    rearrange(algorithm); /*重新按当前的算法排列空闲区*/
    return 1;
}

//释放节点
int readyWidget::dispose(struct allocated_block* free_ab) {
    struct allocated_block* pre, * ab;
    if (free_ab == allocated_block_head) { /*如果要释放第一个节点*/
        allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }

    pre = allocated_block_head;
    ab = allocated_block_head->next;

    while (ab != free_ab){
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

readyWidget::allocated_block *readyWidget::find_process(int pid)
{
    struct allocated_block* p;
    p = allocated_block_head;
    while (p) {
        if (p->pid == pid) {
            return p;
            break;
        }
        else p = p->next;
    }
    return nullptr;
}

void readyWidget::kill_process(int id) {
    struct allocated_block* ab;
    int pid=id;
//    printf("Kill Process, pid=");
//    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL) {
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab);  /*释放ab数据结构节点*/
    }
}

int readyWidget::display_mem_usage() {
    struct free_block_type* fbt = free_block;
    struct allocated_block* ab = allocated_block_head;
    textArea->clear();
    textArea->append("---------\n");
    if (fbt == NULL){
        textArea->append("Free Memory:");
        textArea->append( "  start_addr    size\n");
        textArea->append( "  null            0\n");
    }else{
        /* 显示空闲区 */
        textArea->append("Free Memory:");
        textArea->append( "  start_addr    size\n");
        while (fbt != NULL) {
            textArea->append("    "+QString::number(fbt->start_addr)+"           "+QString::number(fbt->size));
            fbt = fbt->next;
        }
    }

    /*----------------*/
    textArea->append("\n Used Memory:");
    textArea->append( "ID start_addr size\n");
    while (ab != NULL) {
        textArea->append(QString::number(ab->pid)+"  "+QString::number(ab->start_addr)+"  "+QString::number(ab->size));
        ab = ab->next;
    }
    textArea->append("---------\n");
    return 0;
}

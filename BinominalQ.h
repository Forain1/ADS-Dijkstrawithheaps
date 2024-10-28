#include <stdio.h>
#include <stdlib.h>
typedef struct biQnode* biQnodeP;

typedef struct biQnode{
    int key;
    int v;
    biQnodeP parent;
    biQnodeP firstchild;
    biQnodeP nextsibling;

}biQnode;


typedef struct biQ* biQP;
typedef struct biQ {
    int n;
    int nodeNum;
    int minindex;
    biQnodeP* whereNode;//用于标记对应下标节点在堆中的位置
    biQnodeP* Trees;//各个最小堆的集合
}biQ;


biQP CreateQ(int n);//1
void AddBi(biQP Q, biQP tempQ, int v , int key);//2
int* DeleteminBi(biQP Q,biQP tempQ);//3
void DestroyQ_tempQ(biQP Q,biQP tempQ);//4

//无需直接调用
biQnodeP MergeTrees(biQnodeP T1, biQnodeP T2);
void MergeBi(biQP Q1, biQP Q2);
void DecreaseKeyBi(biQP Q, int v, int key);





/*
使用该二项堆的时候需要先调用CreateQ函数创建二项堆，传入可能的最多元素个数n，
在此基础上通过Add函数添加元素新的路径，需要传入顶点标号v以及对应的权值key，该函数会自己判断是插入新的节点还是进行key值的更新
通过调用Deletemin函数可以得到一个数组[0]为key值,[1]为v值，对应为最小的权值以及该点的标号，数据使用后需要手动释放内存

堆中的元素key值与v值视为v到目标点s的路径

使用完该堆后需要调用DestroyQ函数释放内存

*/


/*
    Attention:
    这次修改主要修改了使用堆的方法，首先创建两个二项空堆，一个用来当作我们真正需要用的堆，另一个用来当作临时堆。
    这样一来临时堆就可以重复使用，用于合并，加入以及删除操作。
    所以就不必在上述函数中额外花费时间重复开空间释放空间
    需要注意的是，临时堆我们只用了Tree数组和nodnum,其他的成员并没有使用到，其中的whereNode数组只开辟了空间而没有实际作用，请勿直接调用改成员

*/
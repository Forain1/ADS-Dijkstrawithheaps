#include "BinominalQ.h"




biQP CreateQ(int n){
        biQP Q = (biQP)malloc(sizeof(biQ));
        Q->nodeNum = 0;
        Q->n = n;
        Q->whereNode = (biQnodeP*)malloc(n*sizeof(biQnodeP));
        Q->Trees = (biQnodeP*)malloc(n*sizeof(biQnodeP));
        for(int i=0;i<n;i++){
            Q->whereNode[i] = NULL;
            Q->Trees[i] = NULL;
        }
        Q->minindex=-1;
        return Q;
}

biQnodeP MergeTrees(biQnodeP T1, biQnodeP T2){
    if(T1->key>T2->key)return MergeTrees(T2,T1);
    T2->nextsibling=T1->firstchild;
    T1->firstchild=T2;
    T2->parent=T1;
    return T1;
}

void MergeBi(biQP Q1, biQP Q2){
     Q1->nodeNum += Q2->nodeNum;
     Q1->minindex = -1;//每次Merge实际上都要把根节点进行比较，结束的时候其实就知道新的minindex是多少
     biQnodeP T1,T2;
     int i,j;
     biQnodeP carry=NULL;
    //合并两个Q的算法实现
     for(i=0,j=1;j<=Q1->nodeNum;i++,j*=2){
        T1=Q1->Trees[i],T2=Q2->Trees[i];
        switch (4*!!carry+2*!!T2+!!T1)
        {
        case 0:
        case 1:break;
        case 2:
            Q1->Trees[i]=T2;
            carry=NULL;
            break;
        case 3:
            Q1->Trees[i]=NULL;
            carry=MergeTrees(T1,T2);
            break;
        case 4:
            Q1->Trees[i]=carry;
            carry=NULL;
            break;
        case 5:
            carry=MergeTrees(T1,carry);
            Q1->Trees[i]=NULL;
            break;
        case 6:
            carry=MergeTrees(T2,carry);
            Q1->Trees[i]=NULL;
            break;
        case 7:
            Q1->Trees[i]=carry;
            carry=MergeTrees(T1,T2);
            break;
        default:
            break;
        }
        //更新minindex
        if(Q1->minindex<0&&Q1->Trees[i]||Q1->Trees[i]&&Q1->Trees[i]->key<Q1->Trees[Q1->minindex]->key)Q1->minindex=i;
     }
     
}

void DecreaseKeyBi(biQP Q, int v, int key){
    biQnodeP tempNode = Q->whereNode[v];
    if(tempNode==NULL)printf("Error: DecreaseKeyBi: v is not in Q\n");
    if(tempNode->key>key){
        //上滤操作
        while(tempNode->parent && tempNode->parent->key>key){
            int tempkey = tempNode->parent->key;
            int tempv = tempNode->parent->v;
            tempNode->key=tempkey;
            tempNode->v=tempv;
            Q->whereNode[tempv] = tempNode;
            tempNode=tempNode->parent;
            
        }
        tempNode->key=key;
        tempNode->v=v;
        Q->whereNode[v] = tempNode;
    }
}



void AddBi(biQP Q,biQP tempQ,int v,int key){
    if(Q->whereNode[v])DecreaseKeyBi(Q,v,key);//若这个点已经存在，则更新其key值
    else{//否则，创建一个新的节点，并将其插入到树中
        tempQ->nodeNum = 1;
        tempQ->Trees[0] = (biQnodeP)malloc(sizeof(biQnode));
        tempQ->Trees[0]->firstchild=tempQ->Trees[0]->nextsibling=tempQ->Trees[0]->parent=NULL;
        tempQ->Trees[0]->key=key;
        tempQ->Trees[0]->v=v;
        // biQP tempQ = (biQP)malloc(sizeof(biQ));//为了符合Merge函数的API，创建一个新的tempQ，将这个新节点放进去

        // tempQ->nodeNum = 1;
        // tempQ->Trees = (biQnodeP*)malloc(Q->n*sizeof(biQnodeP));
        // for(int i=0;i<Q->n;i++)tempQ->Trees[i]=NULL;
        // tempQ->Trees[0] = (biQnodeP)malloc(sizeof(biQnode));
        // tempQ->Trees[0]->firstchild=tempQ->Trees[0]->nextsibling=tempQ->Trees[0]->parent=NULL;
        // tempQ->Trees[0]->key=key;
        // tempQ->Trees[0]->v=v;
        Q->whereNode[v] = tempQ->Trees[0];

        MergeBi(Q,tempQ);
        tempQ->nodeNum = 0;
        tempQ->Trees[0]=NULL;

    }
}

int* DeleteminBi(biQP Q,biQP tempQ){
    int i;
    int childnum;
    int tempQnum = tempQ->nodeNum;
    //如果这个Q中还有元素，则返回最小的key和v值，并对Q进行更新
    //否则返回NULL
if(Q->minindex>=0){

    biQnodeP tempTree = Q->Trees[Q->minindex];//取出根节点最小的那个树
    biQnodeP tempchild = tempTree->firstchild;//取出根节点最小的那个树根的第一个孩子
    
    int* key_v = (int*)malloc(2*sizeof(int));
    key_v[0] = tempTree->key;
    key_v[1] = tempTree->v;//获取那个节点的key和v值，用于返回给最短路径程序处理
    
    //对Q进行更新
    Q->nodeNum--;//减少了一个节点
    Q->whereNode[tempTree->v] = NULL;//v对应的节点已经不在了
    Q->Trees[Q->minindex] = NULL;//将根节点最小的那个树整个挖出来

    // biQP tempQ = (biQP)malloc(sizeof(biQ));//为了符合Merge函数的API，创建一个新的tempQ，将挖出的那个树的孩子放进去
    // tempQ->Trees = (biQnodeP*)malloc(Q->n*sizeof(biQnodeP));
    // for(int i=0;i<Q->n;i++)tempQ->Trees[i]=NULL;//初始化tempQ的树数组，在这里由于Merge过程只用了Trees数组和nodeNum，所以其他元素无需初始化
    // tempQ->nodeNum=0;//由于最终还是要放到Q中，所以直接将nodeNum置为0，避免了不必要的加减
    
    //只有当该树有孩子节点的时候才需要进行裁剪
    childnum = Q->minindex-1;
    for(i=childnum;i>=0;i--){
        tempQ->Trees[i] = tempchild;
        tempchild = tempchild->nextsibling;
        tempQ->Trees[i]->nextsibling=NULL;
        tempQ->Trees[i]->parent=NULL;
    }

    free(tempTree);//挖出的那个树的根节点已经不在了，释放掉
    MergeBi(Q,tempQ);


    //恢复tempQ

    for(i=childnum;i>=0;i--){
        tempQ->Trees[i]=NULL;
    }
    tempQ->nodeNum = tempQnum;

    return key_v;
    }
    else return NULL;
    
}


void DestroyQ_tempQ(biQP Q,biQP tempQ){
    while(Q->nodeNum>0)DeleteminBi(Q,tempQ);
    free(Q->Trees);
    free(Q->whereNode);
    free(Q);
    free(tempQ->Trees);
    free(tempQ->whereNode);
    free(tempQ);
}


//测试二项堆的主函数
// int main(){

//     FILE* fp = fopen("./dataset/USA-road-d.NY.gr","r");
//     int n=300000,i;
//     int nothing;
//     int path=400000;
//     biQP Q = CreateQ(n);
//     int key,v;
//     int* key_v;
//     for(i=0;i<path;i++){
//         fscanf(fp,"%d %d %d",&nothing,&v,&key);
//         AddBi(Q,key,v);
//     }
//     while(Q->nodeNum>0){
//         key_v = DeleteminBi(Q);
//         printf("key:%d v:%d\n",key_v[0],key_v[1]);
//     }

//     fclose(fp);
//     DestroyQ(Q);

// }
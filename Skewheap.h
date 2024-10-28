#include<stdio.h>
#include<stdlib.h>

typedef struct sNode* sNodeP;
typedef struct sNode{
    int key;
    int v;
    sNodeP left, right;
    sNodeP parent;
}sNode;

typedef struct Skewheap* SkewheapP;
typedef struct Skewheap{
    sNodeP root;
    sNodeP* wherenode;
}Skewheap;

SkewheapP CreateSkewheap(int n);//1
void InsertSkew(SkewheapP heap, int v, int key);//2
int* DeleteminSkew(SkewheapP heap);//3
void DestroySkew(SkewheapP heap);//4

//����ֱ�ӵ���
sNodeP MergeSkew(sNodeP root1, sNodeP root2);
sNodeP insertSkew(sNodeP root, sNodeP* wherenode, int v ,int key);
void DecreasekeySkew(SkewheapP heap, int v, int key);
int* deleteminSkew(sNodeP* root);




/*
��ʹ�ø�б��ʱ����Ҫ�ȸ������ڵ���n������CreateSkewheap��������б��.
Ȼ�����Insert��������ڵ㣬��Ҫ����v��key���Լ�һ��ָ��б�ѵ�ָ��heap.
����Deletemin����ɾ����С�ڵ㣬����һ��ָ��б�ѵ�ָ��heap������һ��intָ�룬��һ��Ԫ����keyֵ�ڶ���Ԫ����vֵ.
������DestroySkew��������б��
*/

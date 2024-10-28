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
    biQnodeP* whereNode;//���ڱ�Ƕ�Ӧ�±�ڵ��ڶ��е�λ��
    biQnodeP* Trees;//������С�ѵļ���
}biQ;


biQP CreateQ(int n);//1
void AddBi(biQP Q, biQP tempQ, int v , int key);//2
int* DeleteminBi(biQP Q,biQP tempQ);//3
void DestroyQ_tempQ(biQP Q,biQP tempQ);//4

//����ֱ�ӵ���
biQnodeP MergeTrees(biQnodeP T1, biQnodeP T2);
void MergeBi(biQP Q1, biQP Q2);
void DecreaseKeyBi(biQP Q, int v, int key);





/*
ʹ�øö���ѵ�ʱ����Ҫ�ȵ���CreateQ������������ѣ�������ܵ����Ԫ�ظ���n��
�ڴ˻�����ͨ��Add�������Ԫ���µ�·������Ҫ���붥����v�Լ���Ӧ��Ȩֵkey���ú������Լ��ж��ǲ����µĽڵ㻹�ǽ���keyֵ�ĸ���
ͨ������Deletemin�������Եõ�һ������[0]Ϊkeyֵ,[1]Ϊvֵ����ӦΪ��С��Ȩֵ�Լ��õ�ı�ţ�����ʹ�ú���Ҫ�ֶ��ͷ��ڴ�

���е�Ԫ��keyֵ��vֵ��Ϊv��Ŀ���s��·��

ʹ����öѺ���Ҫ����DestroyQ�����ͷ��ڴ�

*/


/*
    Attention:
    ����޸���Ҫ�޸���ʹ�öѵķ��������ȴ�����������նѣ�һ��������������������Ҫ�õĶѣ���һ������������ʱ�ѡ�
    ����һ����ʱ�ѾͿ����ظ�ʹ�ã����ںϲ��������Լ�ɾ��������
    ���ԾͲ��������������ж��⻨��ʱ���ظ����ռ��ͷſռ�
    ��Ҫע����ǣ���ʱ������ֻ����Tree�����nodnum,�����ĳ�Ա��û��ʹ�õ������е�whereNode����ֻ�����˿ռ��û��ʵ�����ã�����ֱ�ӵ��øĳ�Ա

*/
#include "Skewheap.h"

SkewheapP CreateSkewheap(int n){
    //����һ��б��
    SkewheapP heap = (SkewheapP)malloc(sizeof(Skewheap));
    heap->root = NULL;
    heap->wherenode = (sNodeP*)malloc(n*sizeof(sNodeP));
    for(int i=0;i<n;i++){
        heap->wherenode[i] = NULL;
    }
    return heap;
}


sNodeP MergeSkew(sNodeP root1, sNodeP root2){
    //�ϲ�����б��
    
    if(root1 == NULL){
        return root2;
    }

    if(root2 == NULL){
        return root1;
    }
    
    if(root1->key > root2->key)return MergeSkew(root2, root1);//ȷ��root1�е�key��Զ�ǱȽ�С���Ǹ�
    else{
        sNodeP rightchild = root1->right;
        root1->right = root1->left;
        root1->left = MergeSkew(rightchild, root2);
        root1->left->parent = root1;//�������ӵĸ��ڵ㣬�Ա��ں���������
    }
    return root1;
}


sNodeP insertSkew(sNodeP root, sNodeP* wherenode, int v, int key){
    
    //����һ���½ڵ���Ϊ�ϲ��Ķ�
    sNodeP temp = (sNodeP)malloc(sizeof(sNode));
    temp->key = key;
    temp->v = v;
    temp->left = NULL;
    temp->right = NULL;
    temp->parent = NULL;

    *wherenode = temp;//��¼����ڵ����ڵ�λ�ã�����ֱ�ӽ���keyֵ���޸�
    return MergeSkew(root, temp);
}


void DecreasekeySkew(SkewheapP heap, int v, int key){
    sNodeP node = heap->wherenode[v];
    if(node->key > key){
        //���˲���
        while(node->parent&&node->parent->key>key){
            node->key = node->parent->key;
            node->v = node->parent->v;
            heap->wherenode[node->v] = node;//ͬʱҪ�޸�wherenode��Ӧ��v��ָ��Ľڵ�
            node = node->parent; 
        }
        node->key = key;
        node->v = v;
        heap->wherenode[v] = node;
    }
}

void InsertSkew(SkewheapP heap, int v, int key){
    if(heap==NULL)printf("Please create a heap first\n");
    else{
        if(heap->wherenode[v]){
            DecreasekeySkew(heap, v, key);//���ýڵ��ڶ����Ѿ����ڣ������keyֵ�޸�
        }
        else{
           heap->root = insertSkew(heap->root, &heap->wherenode[v],v,key);//���ýڵ㲻���ڣ���ֱ�Ӳ���
        }
    }
}


int* deleteminSkew(sNodeP* root){
    if(root==NULL)return NULL;

    int* key_v = (int*)malloc(2*sizeof(int));
    key_v[0] = (*root)->key;
    key_v[1] = (*root)->v;


    sNodeP leftchild = (*root)->left;
    sNodeP rightchild = (*root)->right;
    if(leftchild)leftchild->parent = NULL;
    if(rightchild)rightchild->parent = NULL;
    
    
    free((*root));

    *root = MergeSkew(leftchild, rightchild);
    return key_v;
}

int* DeleteminSkew(SkewheapP heap){
    int* key_v=deleteminSkew(&heap->root);
    heap->wherenode[key_v[1]] = NULL;
    return key_v;
}



void DestroySkew(SkewheapP heap){
    free(heap->wherenode);
    while(heap->root!=NULL)DeleteminSkew(heap);
    free(heap);
}

//�������������Զ���ȷ�Ե�������
// int main(){
//     SkewheapP heap = CreateSkewheap(10);
//     int key;
//     int v;
//     int n;
//     scanf("%d", &n);
//     for(int i = 0; i<n; i++){
//         scanf("%d %d", &v, &key);
//         insertSkew(heap, v, key);
//     }
//     int* key_v;
//     while(heap->root!=NULL){
//         key_v = Deletemin(heap);
//         printf("key:%d v:%d\n", key_v[0], key_v[1]);
//     }
// }
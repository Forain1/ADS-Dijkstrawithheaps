#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

typedef struct FibNode* FibHeap;
struct FibNode{
	int nodeNum;
	int key;
	int vertex;
	int degree;
	FibHeap left,right,child,parent;
	bool marked;
};
FibHeap FibCreatHeap();
void AddNode(FibHeap node, FibHeap root);
FibHeap FibInsert(FibHeap H,int key,int v,FibHeap posi[]);
FibHeap FibMerge(FibHeap h1,FibHeap h2);
FibHeap FibRemoveMin(FibHeap H,FibHeap posi[]);
FibHeap FibDecreasekey(FibHeap H,FibHeap node,int key);
//int main()
//{
//	FibHeap posi[10];
//	for(int i=0;i<10;i++)
//	{
//		posi[i]=NULL;
//	}
//	FibHeap H2=FibCreatHeap();
//	H2=FibInsert(H2,4,4,posi);
//	H2=FibInsert(H2,5,5,posi);
//	H2=FibInsert(H2,6,6,posi);
//	H2=FibRemoveMin(H2,posi);
//	H2=FibInsert(H2,7,7,posi);
//	H2=FibDecreasekey(H2,posi[7],1);
//	return 0;
//}

FibHeap FibCreatHeap()
{
	FibHeap H=NULL;
	return H;
}
void AddNode(FibHeap root, FibHeap node)	//add node to the fornt of root
{
	node->left=root->left;
	node->right=root;
	root->left->right=node;
	root->left=node;
}
FibHeap CreateNode(int key,int v)	//create a new node and return
{
	FibHeap node=(FibHeap)malloc(sizeof(FibNode));
	node->nodeNum=1;
	node->degree=0;
	node->key=key;
	node->vertex=v;
	node->left=node->right=node;
	node->child=node->parent=NULL;
	node->marked=false;
	return node;
}
FibHeap FibInsert(FibHeap H,int key,int v,FibHeap posi[])	//insert an element to heap
{
	FibHeap tmp=H;
	FibHeap node=CreateNode(key,v);
	posi[v]=node;
	if(tmp==NULL)
		tmp=node;
	else
	{
		AddNode(tmp,node);
		if(node->key<tmp->key)
			tmp=node;
	}
	return tmp;
}
FibHeap FibMerge(FibHeap h1,FibHeap h2)	//Merge two heap into one linked-list
{
	if(h1==NULL)return h2;
	if(h2==NULL)return h1;
	FibHeap tmp;
	tmp=h1->right;
	h1->right=h2->right;
	h2->right->left=h1;
	h2->right=tmp;
	tmp->left=h2;
	FibHeap H;
	if(h1->key<h2->key)
		H=h1;
	else
		H=h2;
	return H;
}
void removeNode(FibHeap node)
{
	node->left->right=node->right;
	node->right->left=node->left;
}
FibHeap link(FibHeap root,FibHeap node)	//link node to root and return
{
//	cout<<"IN LINK!"<<endl;
	removeNode(node);
	if(root->child==NULL)
		root->child=node;
	else 
		AddNode(root->child,node);
	node->parent=root;
	root->nodeNum+=node->nodeNum;
	root->degree++;
	node->marked=false;
	return root;
}
FibHeap Consolidate(FibHeap H)
{
	FibHeap min=H;
	int nodeSum=min->nodeNum;
	for(FibHeap i=min->right;i!=min;i=i->right)
	{
//		cout<<"Node:"<<i->key<<"  "<<i->vertex<<endl;
		nodeSum+=i->nodeNum;
	}
	int maxDegree=(int)(log(nodeSum)/log(2));
	int D=maxDegree+1;
//	cout<<"nodeSum and D:"<<nodeSum<<"  "<<D<<endl;
	vector<FibHeap> cons(D+1,NULL);
	while(min!=NULL)
	{
		FibHeap x=min;
		if(x->right==x)
			min=NULL;
		else
		{
			min=x->right;
			removeNode(x);
		}
		x->left=x->right=x;
		
//		cout<<"ADD TO CONS:"<<x->key<<"  "<<x->degree<<endl;
		int d=x->degree;
		while(cons[d]!=NULL)
		{
			FibHeap y=cons[d];
			if(x->key>y->key)
			{
				FibHeap tmp=x;
				x=y;
				y=tmp;
			}
			x=link(x,y);
			cons[d]=NULL;
			d++;
		}
		cons[d]=x;
//		cout<<"IN CONS:"<<x->key<<"  "<<x->degree<<endl;
	}
	min=NULL;
	for(int i=0;i<=D;i++)
	{
		if(cons[i]!=NULL)
		{
//			cout<<"find cons:"<<cons[i]->key<<"  "<<cons[i]->vertex<<endl;
			if(min==NULL)
				min=cons[i];
			else
			{
				AddNode(min,cons[i]);
				if((cons[i])->key<min->key)
					min=cons[i];
			}
		}
	}
	return min;
}
FibHeap FibRemoveMin(FibHeap H,FibHeap posi[])
{
	if(H==NULL)return NULL;
	
	FibHeap tmp=H;
	while(tmp->child!=NULL)	//move tmp's child to root linked-list
	{
		FibHeap child=tmp->child;
		if(child->right==child)
			tmp->child=NULL;
		else
			tmp->child=child->right;
		removeNode(child);
		AddNode(tmp,child);
		child->parent=NULL;
	}
//	cout<<"Here OK!"<<endl;
	removeNode(tmp);
	posi[tmp->vertex]=NULL;
	if(tmp->right==tmp)
	{
		free(tmp);
		tmp=NULL;
	}
	else
	{
//		cout<<"IN DEL\n";
		FibHeap Del=tmp;
		tmp=tmp->right;
		free(Del);
		Del=NULL;
//		cout<<"DELETE OK!\n";
		tmp=Consolidate(tmp);
	}
	return tmp;
}
void cut(FibHeap H,FibHeap node)
{
	FibHeap parent=node->parent;
	removeNode(node);
	parent->degree--;
	if(node==node->right)
		parent->child=NULL;
	else 
		parent->child=node->right;
	
	node->parent=NULL;
	node->left=node->right=node;
	node->marked=false;
	AddNode(H,node);
}
void cascadingCut(FibHeap H,FibHeap node)
{
	FibHeap parent=node->parent;
	if(parent!=NULL)
	{
		if(node->marked==false)
			node->marked=true;
		else
		{
			cut(H,node);
			cascadingCut(H,parent);
		}
	}
}
FibHeap FibDecreasekey(FibHeap H,FibHeap node,int key)
{
	if(H==NULL)return NULL;
	if(node==NULL)return H;
	FibHeap tmp=H;
	if(key>node->key)
	{
		cout<<"Decrease Failed! New key must be smaller than current key!"<<endl;
		return H;
	}
	node->key=key;
	FibHeap parent=node->parent;
	if(parent!=NULL && (node->key<parent->key))
	{
		cut(H,node);
		cascadingCut(H,parent);
	}
	if(node->key<tmp->key)
		tmp=node;
	return tmp;
}

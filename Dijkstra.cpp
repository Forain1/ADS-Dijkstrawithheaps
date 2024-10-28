extern "C"{
	#include"BinominalQ.c"
	#include"Skewheap.c"
}
#include"FibHeap.h" 
#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<vector>
#include<queue>
#define MAXN 3000000
//#define MAXN 2005
#define INF 0x7f7f7f7f
using namespace std;
vector<pair<int,int> >g[MAXN];	//make graph
int vis[MAXN];	//judge connected block 


void InitDist(int d[],int n);
void DijkstraBi(biQP H,biQP tempQ,int start,int d[]);
void DijkstraSTL(int n,int start,int d[]);
void DijkstraFib(FibHeap H,FibHeap posi[],int start,int d[]);
void DijkstraSkew(SkewheapP H,int start,int d[]);
int main()
{
	printf("Dijkstra Algorithm Test\n");
	freopen("./testdata/test6.in","r",stdin);	//file operation: read input from testdata.in
	char c;
	int n,m;	//n: nodes , m: edges
	while(scanf("%c",&c)!=EOF)	//scan graph
	{
//		printf("%d %c\n",++cnt,c);
		if(c=='#')break;	//'#'is added by me in testdata.in, for convenience of test
		switch(c)
		{
			case 'p':	//scan n and m
				char s[100];
				scanf("%s %d %d\n",s,&n,&m);
				break;
			case 'a':	//scan edge
				int u,v,w;
				scanf("%d %d %d\n",&u,&v,&w);
				g[u].push_back(make_pair(v,w));
				break;
			default:	//skip line
				char str[100];
				gets(str);
		}
	}
	printf("Graph constructed!\n");
	printf("n:%d  m:%d\n",n,m);
	printf("\n");

	
	//Do Dijkstra
	int blockCnt;	//count for blocks
	clock_t stTime,edTime;
	double duration;
	
	int start;
	int d[MAXN];
	
	//STL version Dijkstra
	blockCnt = 0;
	InitDist(d,n);
	
	stTime=clock();
	
	for(int i=1;i<=n;i++){
		if(!vis[i])
		{
			blockCnt++;
			start = i;
			DijkstraSTL(n,start,d);
		}
	}
	edTime=clock();
	duration=((double)(edTime-stTime))/CLK_TCK;
	printf("There are %d connected blocks in graph.\n",blockCnt);
	printf("Runtime of DijkstraSTL is: %.3lf s\n",duration);
	printf("\n");
	
	//Bi version Dijkstra
	blockCnt = 0;
	InitDist(d,n);
	stTime=clock();
	//create H and tempQ so that they can be used reapeatedly in DijkstraBi//
	biQP H,tempQ;
	H=CreateQ(n+10);
	tempQ=CreateQ(n+10);

	for(int i=1;i<=n;i++){
		if(!vis[i])
		{
//			if(i%1000==0)
//			 printf("!!!!!!!!!!Now at i:%d\n",i);
			blockCnt++;
			start = i;
			DijkstraBi(H,tempQ,start,d);	
		}
	}
	DestroyQ_tempQ(H,tempQ);	//free space
	edTime=clock();
	duration=((double)(edTime-stTime))/CLK_TCK;
	printf("There are %d connected blocks in graph.\n",blockCnt);
	printf("Runtime of DijkstraBi is: %.3lf s\n",duration);
	printf("\n");
	


//Skew version Dijkstra
	blockCnt = 0;
	InitDist(d,n);
	stTime=clock();
	
	SkewheapP skew =  CreateSkewheap(n);
	for(int i=1;i<=n;i++){
		if(!vis[i])
		{
		// printf("!!!!!!!!!!Now at i:%d\n",i);
			blockCnt++;
			start = i;
			DijkstraSkew(skew,start,d);
		}
	}
	DestroySkew(skew);
	edTime=clock();
	duration=((double)(edTime-stTime))/CLK_TCK;
	printf("There are %d connected blocks in graph.\n",blockCnt);
	printf("Runtime of DijkstraSkw is: %.3lf s\n",duration);
	printf("\n");



	//Fib version Dijkstra
	blockCnt = 0;
	InitDist(d,n);
	FibHeap FibH=FibCreatHeap();
	FibHeap posi[MAXN];
	for(int i=0;i<=n;i++)
		posi[i]=NULL;
	stTime=clock();
	for(int i=1;i<=n;i++){
		if(!vis[i])
		{
			blockCnt++;
			start = i;
			DijkstraFib(FibH,posi,start,d);
		}
	}
	edTime=clock();
	duration=((double)(edTime-stTime))/CLK_TCK;
	printf("There are %d connected blocks in graph.\n",blockCnt);
	printf("Runtime of DijkstraFib is: %.3lf s\n",duration);
	
	return 0;
}

void InitDist(int d[],int n)
{
	for(int i=0;i<=n;i++)
	{
		d[i]=INF; vis[i]=0;
	}
}

void DijkstraBi(biQP H,biQP tempQ,int start,int d[])
{
	d[start]=0;
	AddBi(H,tempQ,start,0);
	while(H->nodeNum)	//at most n rounds for dij
	{
//		printf("round %d:  ",i);
		int *tmp=DeleteminBi(H,tempQ);
		if(tmp==NULL)break;	//Heap is empty
		int x=tmp[1];
		free(tmp);
//		cout<<"NOW IN"<<x<<endl;
		
		if(vis[x])continue;
		vis[x]=1;	//x is visited
		
		vector<pair<int,int> >::iterator it;
//		printf("Father node x:%d dist:%d\n",x,dist);
		for(it=g[x].begin();it!=g[x].end();it++)
		{
			int v,w;
			v=(*it).first; w=(*it).second;
//			printf("dist:%d, v:%d, w:%d, d[v]:%d\n",d[x],v,w,d[v]);
			if(w+d[x]<d[v])
			{
				d[v]=w+d[x];
 				AddBi(H,tempQ,v,d[v]);
			}
		}
	}
	// DestroyQ_tempQ(H,tempQ);	//free space
}

void DijkstraSkew(SkewheapP H,int start,int d[])
{
	

	d[start]=0;
	InsertSkew(H,start,0);
	while(H->root)	//at most n rounds for dij
	{
//		printf("round %d:  ",i);
		int *tmp=DeleteminSkew(H);
		if(tmp==NULL)break;	//Heap is empty
		int x=tmp[1];
		free(tmp);
		
		if(vis[x])continue;
		vis[x]=1;	//x is visited
		
		vector<pair<int,int> >::iterator it;
//		printf("Father node x:%d dist:%d\n",x,dist);
		for(it=g[x].begin();it!=g[x].end();it++)
		{
			int v,w;
			v=(*it).first; w=(*it).second;
//			printf("dist:%d, v:%d, w:%d, d[v]:%d\n",dist,v,w,d[v]);
			if(w+d[x]<d[v])
			{
				d[v]=w+d[x];
 				InsertSkew(H,v,d[v]);
			}
		}
	}
	// DestroyQ_tempQ(H,tempQ);	//free space
}

void DijkstraSTL(int n,int start,int d[])
{
	d[start]=0;
	priority_queue<pair<int,int> >H;
	H.push(make_pair(-0,start));
	while(!H.empty())
	{
		int x=H.top().second;
		H.pop();

		if(vis[x])continue;
		vis[x]=1;
		
		vector<pair<int,int> >::iterator it;
		for(it=g[x].begin();it!=g[x].end();it++)
		{
			int v,w;
			v=(*it).first; w=(*it).second;
			if(w+d[x]<d[v])
			{
				d[v]=w+d[x];
				H.push(make_pair(-d[v],v));
			}
		}
	}
}

void DijkstraFib(FibHeap H,FibHeap posi[],int start,int d[])
{
	int cnt1=0,cnt2=0;
	H=FibInsert(H,0,start,posi);
	d[start]=0;
	while(H!=NULL)
	{
		int x = H->vertex;
		H=FibRemoveMin(H,posi);
//		cout<<x<<endl;
		if(vis[x])continue;
		vis[x]=1;
		
		vector<pair<int,int> >::iterator it;
		for (it=g[x].begin();it!=g[x].end();it++)
		{
            int v = (*it).first;
            int weight = (*it).second;
            
			if (d[x] + weight < d[v])
			{
				d[v]=d[x]+weight;
				if(posi[v]==NULL)
				{
//					cout<<"INSERT"<<++cnt1<<endl;
					H=FibInsert(H,d[v],v,posi);
				}
				else
				{
//					cout<<"DECREASE"<<++cnt2<<endl;
					H=FibDecreasekey(H,posi[v],d[v]);
				}
            }
        }
	}
}

//Creat a complet graph of size n. Weight of edges is random.
//The graph is printed to data1.out
#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("./testdata/complete5.in","w",stdout);
	srand((int)time(0));
	int n=10000,m;
	m=n*(n-1)/2;
	printf("p sp %d %d\n",n,2*m);
	for(int i=1;i<=n-1;i++)
		for(int j=i+1;j<=n;j++)
		{
			int w=rand()%10000+1;
			printf("a %d %d %d\n",i,j,w);
			printf("a %d %d %d\n",j,i,w);
		}
	return 0;
}

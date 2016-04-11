#include<iostream>
#include<iomanip>
#define INF 32768
using std::cout;
using std::cin;
class flo
{
	int **a,n,**next;
	public:
		flo(int num=0)
		{
			n=num;
			a=new int*[n];
			next=new int*[n];
			for(int i=0;i<n;i++)
			{	a[i]=new int[n];
				next[i]=new int[n];
			}

		}
		void input()
		{	for(int i=0;i<n;i++)
				for(int j=0;j<n;j++)
				{	if(i!=j)
						a[i][j]=INF;
						next[i][j]=-1;
				}
			int s,d,w;
			while(true)
			{
				cout<<"\nenter source vertex, destination vertex and weight of edge(source or destination -1 to stop):";
				cin>>s>>d>>w;
				if(s>=n||d>=n)
				{	cout<<"\nInvalid vertices!";
					continue;
				}
				if(w<=0||w>=INF)
				{	cout<<"\nWeight out of bounds!";
					continue;
				}
				if(s==d)
				{	cout<<"\nweight fixed when source and destination same!";
					continue;
				}

				if(s==-1||d==-1)
					break;
				a[s][d]=w;
				next[s][d]=d;
			}
		}
		void print()
		{	cout<<std::endl;
			for(int i=0;i<n;i++)
			{	for(int j=0;j<n;j++)
				{
					if(a[i][j]==INF)
						cout<<"INF ";
					else
						cout<<std::setw(3)<<a[i][j]<<" ";
				}
				cout<<std::endl;
			}

		}
		void cal_dis()
		{
			for(int k=0;k<n;k++)
			{
				for(int i=0;i<n;i++)
					for(int j=0;j<n;j++)
						if(a[i][k]+a[k][j]<a[i][j])
						{	a[i][j]=a[i][k]+a[k][j];
							next[i][j]=next[i][k];
						}
				cout<<"D"<<k<<":";
				if(k==n-1)
					cout<<"(final shortest distance matrix)";
				print();
			}
		}
		int  *path(int u,int v)
		{	int *a=new int[5];
			if(next[u][v]==-1)
			{
				a[0]=-1;

			}
			else
			{
				a[0]=1;
				a[1]=u;
				while(u!=v)
				{
					u=next[u][v];
					a[++a[0]]=u;
				}
			}
			return a;
		}
};
int main(void)
{
	int n,*p;
	cout<<"enter number of vertices:";
	cin>>n;
	flo f(n);
	f.input();
	f.cal_dis();
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{	p=f.path(i,j);
			if(*p==-1)
				cout<<"\nNo path between "<<i<<" and "<<j;
			else
			{
				cout<<"\nPath from"<<i<<" to "<<j<<":";
				for(int k=1;k<=*p;k++)
				{	if(k!=*p)
						cout<<p[k]<<"->";
					else
						cout<<p[k];
				}
			}
		}
	}
	return 0;
}


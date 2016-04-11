#include<iostream>
using std::cout;
using std::cin;
class ham
{
	int **a,n,*path;
	public:
		ham(int num=0)
		{
			n=num;
			a=new int*[n];
			path=new int[n];
			for(int i=0;i<n;i++)
				a[i]=new int[n];
		}
		void input()
		{	int s,d;
			while(true)
			{
				cout<<"\nenter source vertex, destination vertex:(-1 to stop)";
				cin>>s>>d;
				if(s>=n||d>=n)
				{	cout<<"\nInvalid vertices!";
					continue;
				}
				if(s==-1||d==-1)
					break;
				a[s][d]=a[d][s]=1;
			}
			cout<<"adjacency matrix:\n";
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
					cout<<a[i][j]<<" ";
				cout<<"\n";
			}
		}
		bool issafe(int v,int pos)
		{	if(pos==0)
				return true;
			if(a[path[pos-1]][v]==0)
				return false;
			for(int i=0;i<pos;i++)
				if(path[i]==v)
					return false;
			return true;
		}
		int findnext(int pos)
		{
			int i;
			if(pos == 0)
			{
				if(path[pos]==-1)
					return 0;
				else if(path[pos]<n-1)
					return path[pos]+1;
				else
					return -1;
			}
			else if(pos>=n)
				return -1;
			else {
				i = path[pos]+1;
				while(i < n) {
					if(issafe(i,pos)==true)
						return i;
					i++;
				}
				return -1;
			}
		}
		void hamcycle(void)
		{
			int pos, v;

			for(int i=0;i<n;i++)
				path[i]=-1;
			pos = 0;
			while(true)
			{
				v=findnext(pos);
				if(v==-1)
				{
					path[pos--]=-1;
					if(pos==-1)
						return;
				}
				else
				{
					path[pos++]=v;
					if(pos==n)
					{	printsolution();
						path[--pos]=-1;
						pos--;
					}
				}
			}

		}
		void printsolution()
		{
			cout<<"\nCycle:";
			for(int i=0;i<n;i++)
				cout<<path[i]<<"->";
			cout<<path[0]<<std::endl;
		}
};
int main(void)
{	int n;
	cout<<"enter number of vertices:";
	cin>>n;

	ham h(n);
	h.input();
	h.hamcycle();
	return 0;
}


/*
Output:

enter source vertex, destination vertex:0 1

enter source vertex, destination vertex:0 3

enter source vertex, destination vertex:0 5

enter source vertex, destination vertex:1 2

enter source vertex, destination vertex:1 4

enter source vertex, destination vertex:2 3

enter source vertex, destination vertex:3 4

enter source vertex, destination vertex:4 5

enter source vertex, destination vertex:-1 2
adjacency matrix:
0 1 0 1 0 1
1 0 1 0 1 0
0 1 0 1 0 0
1 0 1 0 1 0
0 1 0 1 0 1
1 0 0 0 1 0

Cycle:0->1->2->3->4->5->0

Cycle:0->3->2->1->4->5->0

Cycle:0->5->4->1->2->3->0

Cycle:0->5->4->3->2->1->0

Cycle:1->0->5->4->3->2->1

Cycle:1->2->3->0->5->4->1

Cycle:1->2->3->4->5->0->1

Cycle:1->4->5->0->3->2->1

Cycle:2->1->0->3->4->5->2

Cycle:2->1->0->5->4->3->2

Cycle:2->1->4->3->0->5->2

Cycle:2->1->4->5->0->3->2

Cycle:2->3->0->1->4->5->2

Cycle:2->3->0->5->4->1->2

Cycle:2->3->4->1->0->5->2

Cycle:2->3->4->5->0->1->2

Cycle:3->0->5->4->1->2->3

Cycle:3->2->1->0->5->4->3

Cycle:3->2->1->4->5->0->3

Cycle:3->4->5->0->1->2->3

Cycle:4->1->2->3->0->5->4

Cycle:4->3->2->1->0->5->4

Cycle:4->5->0->1->2->3->4

Cycle:4->5->0->3->2->1->4

Cycle:5->0->1->2->3->4->5

Cycle:5->0->1->4->3->2->5

Cycle:5->0->3->2->1->4->5

Cycle:5->0->3->4->1->2->5

Cycle:5->4->1->0->3->2->5

Cycle:5->4->1->2->3->0->5

Cycle:5->4->3->0->1->2->5

Cycle:5->4->3->2->1->0->5
 **/

#include <iostream>
#include <stdlib.h>
using namespace std;
int no;

bool place(int board[15][15],int i,int col,int size)
{
	int m,n;
	for(m=col-1,n=i-1;m>=0 && n>=0;m--,n--)
	{
		if(board[n][m] == 1)
		{
			board[i][col] = 0;
			return false;
		}	
			
	}
	for(m=col-1,n=i+1;m>=0 && n<size;m--,n++)
	{
		if(board[n][m] == 1)
		{
			board[i][col] = 0;
			return false;
		}	
		
	}
	for(m=col-1;m>=0;m--)
	{
		if(board[i][m] == 1)
		{
			board[i][col] = 0;
			return false;
		}	
			
	}
	return true;
}

bool nqueens(int board[15][15],int col,int size,int queen)
{
	//cout<<queen<<"\n";
	int i,j,m,n,k;
	bool h = false;
	if(queen > no)
	{
		return true;
	}
	for(i=0;i<size;i++)
	{	
		board[i][col] = 1;

		if(place(board,i,col,size) == false)
		{
			continue;
		}
		else
		{
			 h = nqueens(board,++col,size,queen+1);
			if(h)
			{
				return true;
			}
			board[i][--col] = 0;							
		}
	}
	return false;
}


int main()
{
	int i,j,size,queen=1;
	bool f = false;
	cout<<"Enter number of queens : \n";
	cin>>no;
	cout<<"Enter size of chessboard : \n";
	cin>>size;
	int board[15][15];
	if(no > size)
	{
		cout<<"More queens than size of chessboard not allowed.\n";
		exit(0);
	}
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			board[i][j] = 0;
		}
	}
	i = j = 0;
	f = nqueens(board,i,size,queen);
	if(f)
	{
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			cout<<board[i][j];
		}
		cout<<"\n";
	}
	}		
}



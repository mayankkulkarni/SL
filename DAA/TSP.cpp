#include <iostream>
#include <stdlib.h>
#include <iomanip>
#define inf 999
using namespace std;
int g[50][50],v,e,path[20],cost[20];

void input(){
	cout<<"Enter number of cities :";
	cin>>v;
	cout<<"Enter cost of reaching each city :\n";
	for(int i=0;i<v;i++){
		for(int j=0;j<v;j++){
			if(i != j){
				cout<<"Enter cost of going from city "<<i+1<<" to "<<j+1<<" : ";
				cin>>g[i][j];
			}
			else
				g[i][j] = inf;
		}
	}
}

void display(int a[50][50],int n){
	for(int i = 0;i<n;i++){
		for(int j = 0;j<n;j++){
			if(a[i][j] == inf)
				cout<<setw(3)<<"inf"<<" ";
			else	
				cout<<setw(3)<<a[i][j]<<" ";
		}		
		cout<<"\n";
	}
}

int row_min(int a[50][50],int row){
	int min = inf;
	for(int i = 0;i<v;i++){
			if(a[row][i] < min)
				min = a[row][i];			
	}
	return min;
}

int col_min(int a[50][50],int col){
	int min = inf;
	for(int i = 0;i<v;i++){
			if(a[i][col] < min)
				min = a[i][col];			
	}
	return min;
}

int row_minimize(int a[50][50]){
	int min[v],sum = 0;
	for(int i=0;i<v;i++){
		min[i] = row_min(a,i);
		if(min[i] == inf)
			return 0;

		for(int j=0;j<v;j++){
			if(a[i][j] == 0){
				min[i] = 0;
				break;
			}
		}

		for(int j=0;j<v;j++){
			if(a[i][j] != inf){
				a[i][j] -= min[i];
			}
		}
		sum += min[i];
	}
	return sum;
}

int col_minimize(int a[50][50]){
	int min[v],sum = 0;
	for(int i=0;i<v;i++){
		min[i] = col_min(a,i);
		if(min[i] == inf)
			return 0;
		for(int j=0;j<v;j++){
			if(a[j][i] == 0){
				min[i] = 0;
				break;
			}
		}
		for(int j=0;j<v;j++){
			if(a[j][i] != inf){
				a[j][i] -= min[i];
			}
		}
		sum += min[i];
	}
	return sum;
}

int find_inpath(int path[20],int k,int n){
	for(int i=0; i<=n; i++){
		if(path[i] == k){	
			return 0;
		}
	}
	return 1;
}

void dynamic_reduction(){
	int row_sum=row_minimize(g),col_sum=col_minimize(g),count=0;
	cout<<"Initial Reduced Matrix :\n";
	display(g,v);
	cout<<"\n";
	int m[50][50],temp[10][2];
	for(int i=0;i<20;i++){
		path[i] = -1;
		cost[i] = 0;
	}
	cost[0] = row_sum + col_sum;
	path[0] = 0;
	for(int i = 0;i<v-1;){
		count = 0;
		for(int j=0;j<v;j++){
		for(int n=0;n<v;n++){
			for(int k=0;k<v;k++){
				if(g[n][k] != inf)
					m[n][k] = g[n][k];
				else
					m[n][k] = inf;
			}
		}
		for(int k=0;k<=i;k++){
			for(int n=0;n<v;n++)
				m[path[k]][n] = inf;
		}
		if(find_inpath(path,j,i)){
				for(int k=1;k<=i;k++){
					for(int n=0;n<v;n++)
						m[n][path[k]] = inf;
				}
				for(int n=0;n<v;n++)
					m[n][j] = inf;
				for(int n=i;n>=1;n--){
					m[path[n]][path[0]] = inf;
				}
				m[j][path[0]] = inf;
				cout<<"CITY = "<<j+1<<"\n";
				row_sum = row_minimize(m);
				col_sum = col_minimize(m);
				cout<<"REDUCED MATRIX FOR CITY "<<j<<" : \n";
				display(m,v);
				cout<<"\n";
				int red_cost = row_sum + col_sum;
				temp[count][0] = j;
				temp[count][1] = red_cost + cost[i] + g[path[i]][j];
				cout<<"Total Reduced Cost : "<<temp[count][1]<<"\n\n";
				count++;
			}
		}
		cout<<"CITIES AVAILABLE WITH COST:\n";
		for(int n = 0;n<count;n++){
			cout<<temp[n][0]+1<<" :: "<<temp[n][1]<<"\n";
		}
		cout<<"\n";
		int min = inf,u=0;
		for(int n=0;n<count;n++){
			if(min > temp[n][1]){
				u = temp[n][0];
				min = temp[n][1];
			}
		}
		cout<<"SELECTED CITY : "<<u<<"\n";
		cout<<"CURRENT COST : "<<min<<"\n";
		path[++i] = u;
		cost[i] = min;	
	}
	
}

int main(){
	input();
	display(g,v);
	dynamic_reduction();
	cout<<"The TSP path is :\n";
	for(int i=0;i<v;i++)
	{
		cout<<path[i]+1<<" ";
	}
	cout<<"\n\nThe minimum cost is : ";
	cout<<cost[v-1]<<"\n";
	cout<<"\n";
}
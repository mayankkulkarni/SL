//============================================================================
// Name        : 1eredfa.cpp
// Author      : KAPIL
// Version     :
// Copyright   : TE 10
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stack>
#include <string>
using namespace std;

struct node{
	char data;
	bool nullable;
	int pos,fpos[25],lpos[25];
	node *llink,*rlink;
}*root;

struct followpos{
	char ch;
	int follow[20];
};

string postfix;
followpos follow_table[20];
int state[10][10],sym[10],dfa[30],j=1;

int search(int a,int x[]){
	for(int i=1;i<=x[0];i++){
		if(x[i]==a)
			return i;
	}
	return -1;
}

void arr_app(int a[],int b[]){
	int c=a[0];
	for(int i=1;i<=b[0];i++){
		if(search(b[i],a)==-1)
			a[++c]=b[i];
	}
	a[0]=c;
}

void arr_cpy(int a[],int b[]){
	int c=b[0];
	for(int i=1;i<=b[0];i++)
		a[i]=b[i];
}

void arr_union(int a[],int b[],int c[]){
	int c1=a[0];
	for(int i=1;i<=a[0];i++)
		c[i]=a[i];
	for(int i=1;i<=b[0];i++){
		if(search(b[i],c)==-1)
			c[++c1]=b[i];
	}
	a[0]=c1;
}

void follow(node *T){
	if(T->data=='*'){
		for(int i=1;i<=T->lpos[0];i++)
			arr_app(follow_table[T->lpos[i]].follow,T->llink->fpos);
	}
	if(T->data=='.'){
		for(int i=1;i<=T->llink->lpos[0];i++)
			arr_app(follow_table[T->llink->lpos[i]].follow,T->rlink->fpos);
	}
}
void display_follow(){
	cout<<"\nPOS\tNODE\tFOLLOWPOS\n";
	for(int i=1;i<j;i++){
		cout<<i<<"\t"<<follow_table[i].ch<<"\t";
		for(int k=1;k<=follow_table[i].follow[0];k++)
			cout<<follow_table[i].follow[k];
	}
}
void display(node*t){
	cout<<t->data<<"\t"<<t->pos<<"\t"<<t->nullable<<"\t";
	for(int i=1;i<=t->fpos[0];i++)
		cout<<t->fpos[i]<<" ";
	cout<<"\t";
	for(int i=1;i<=t->lpos[0];i++)
		cout<<t->lpos[i]<<" ";
	cout<<"\n";
}

void traverse(node *t){
	if(t==NULL)
		return;
	traverse(t->llink);
	display(t);
	traverse(t->rlink);
}
node* create_tree(){
	stack<node*> stk;
	int i=0;
	char x;
	node *t1,*t2;
	while(postfix[i]){
		x=postfix[i++];
		node*T=new node;
		if( isalpha(x) || x=='#' ){
			T->data=x;
			T->pos=j;
			T->nullable=false;
			follow_table[j].ch=x;
			T->fpos[0]=1;
			T->fpos[1]=j;
			T->lpos[0]=1;
			T->lpos[1]=j++;
			T->llink=NULL;
			T->rlink=NULL;
			if(search((int)x,sym)==-1)
					sym[sym[0]++]=x;
			stk.push(T);
		}
		else if(x=='*'){
			T->data=x;
			T->pos=0;
			T->nullable=true;
			T->rlink=NULL;
			t1=stk.top();
			stk.pop();
			T->llink=t1;
			arr_cpy(T->fpos,T->llink->fpos);
			arr_cpy(T->lpos,T->rlink->lpos);
			follow(T);
			stk.push(T);
		}
		else if(x=='.'){
			T->data=x;
			T->pos=0;
			T->nullable=false;
			t1=stk.top();
			stk.pop();
			t2=stk.top();
			stk.pop();
			T->rlink=t1;
			T->llink=t2;
			T->nullable=T->llink->nullable && T->rlink->nullable;
			if(T->llink->nullable)
				arr_union(T->llink->fpos,T->rlink->fpos,T->fpos);
			else
				arr_cpy(T->fpos,T->llink->fpos);
			if(T->rlink->nullable)
				arr_union(T->llink->lpos,T->rlink->lpos,T->lpos);
			else
				arr_cpy(T->lpos,T->rlink->lpos);
			follow(T);
			stk.push(T);

		}
		else if(x=='|'){
			T->data=x;
			T->pos=0;
			T->nullable=false;
			t1=stk.top();
			stk.pop();
			t2=stk.top();
			stk.pop();
			T->rlink=t1;
			T->llink=t2;
			T->nullable=T->llink->nullable && T->rlink->nullable;
			arr_union(T->llink->fpos,T->rlink->fpos,T->fpos);
			arr_union(T->llink->lpos,T->rlink->lpos,T->lpos);
			follow(T);
			stk.push(T);
		}
	}
	root=stk.top();
	stk.pop();
	sym[0]--;
	return root;
}







int main() {
	node *r;
	cout << "Enter postfix string :";
	cin>>postfix;
	string str="#.";
	postfix+=str;
	r=create_tree();
	display_follow();
	return 0;
}


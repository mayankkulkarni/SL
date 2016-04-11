
#include <iostream>
#include <stdlib.h>
using namespace std;

typedef struct node
{
	char data;
	int freq;
	struct node *left,*right;
}node;

class Huffman
{
	node *root;
	int code[50];
public:
	
	Huffman()
	{
		root = NULL;
	}
	void construct();
	void encode(int t,node *n);
	void decode(char s[60]);
	node* getroot();
};

class heap
{
	node *a[40];
	int size;
public:
	void heapify(node *arr[20],int n);
	heap()
	{
		
		for(int i=0;i<40;i++)
		{
			a[i] = (node*)malloc(sizeof(node));
			a[i]->freq = -1;
			a[i]->data = ' ';
			a[i]->left = NULL;
			a[i]->right = NULL;
		}
		size = 0;
	}
	void insert(node *k);
	node* del();
	void swap(node *t1, node *t2);
	void display();
	node* getnode(int i);
	int getsize();
};

node* heap::getnode(int i)
{
	return a[i];
}

int heap::getsize()
{
	return size;
}

void heap::swap(node *t1, node *t2)
{
	node temp1 = *t1;
	*t1 = *t2;
	*t2 = temp1;
}

void heap::heapify(node *arr[20],int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		insert(arr[i]);
	}
}

void heap::insert(node *k)
{
	int i = size;
	a[++i] = k;	
	while(i != 1)
	{
		if(a[i/2]->freq > a[i]->freq)
		{
			swap(a[i],a[i/2]);
			i = i/2;
		}
		else
			break;
	}
	size += 1;
	display();	
}

node* heap::del()
{
	int i = 1;
	swap(a[1],a[size]);
	node *temp = a[size];
	size -= 1;
	while(2*i + 1 <= size)
	{
			if(a[i]->freq > a[2*i]->freq && a[i]->freq < a[2*i + 1]->freq)
			{
				swap(a[i],a[2*i]);
				i = 2*i;
			}
			else if(a[i]->freq > a[2*i + 1]->freq && a[i]->freq < a[2*i]->freq) 
			{
				swap(a[i],a[2*i + 1]);
				i = 2*i +1;
			}
			else if(a[i]->freq > a[2*i + 1]->freq && a[i]->freq > a[2*i]->freq)
			{
				if(a[2*i]->freq > a[2*i + 1]->freq)
				{
					swap(a[i],a[2*i + 1]);
				}
				else //if(a[2*i]->freq <= a[2*i + 1]->freq)
				{
					swap(a[i],a[2*i]);
				}
			}
			else
				break;
	}
	if(a[1]->freq > a[size]->freq && size == 2) 
		swap(a[1],a[size]);
	display();
	return temp;
}

void heap::display()
{
	int i;
	for(i=0;i<=size;i++)
	{
		cout<<a[i]->freq<<" ";
	}
	cout<<"\n";
}

node* Huffman::getroot()
{
	return root;
}

void Huffman::construct()
{
	int n,i;
	cout<<"Enter number of elements in Code :";
	cin>>n;
	node *test[40];
	for(i=0;i<n;i++)
	{
		test[i] = (node*)malloc(sizeof(node));
		cout<<"Enter "<<i<<"th data :";
		cin>>test[i]->data;
		cout<<"Enter "<<i<<"th frequency :";
		cin>>test[i]->freq;
		test[i]->left = NULL;
		test[i]->right = NULL;
	}
	heap h;
	h.heapify(test,n);	
	while(h.getsize() != 1)
	{
		node *t1 = h.del();
		node *t2 = h.del();		
		node *t3 = (node*)malloc(sizeof(node));
		t3->freq = t1->freq + t2->freq;
		t3->data = ' ';
		t3->left = t1;
		t3->right = t2;
		h.insert(t3);
	}
	root = h.getnode(1);
}

void Huffman::encode(int t,node *n)
{
	//cout<<n->freq<<" ";
	if(n->left)
	{
		code[t] = 0;
		//cout<<code[t]<<"\n";
		encode(t+1,n->left);
	}
	if(n->right)
	{
		code[t] = 1;
		//cout<<code[t]<<"\n";
		encode(t+1,n->right);
	}
	if(!n->left && !n->right)
	{
		cout<<n->data<<" = ";
		int i;
		for(i=0;i<t;i++)
			cout<<code[i];
		cout<<"\n";
	}	
}

void Huffman::decode(char s[60])
{
	int i=0;
	node *temp = root;
	cout<<"Decoded string : ";
	while(s[i] != '\0')
	{
		if(!temp->left && !temp->right)
		{
			cout<<temp->data;
			temp = root;
		}
		if(s[i] == '0')
			temp = temp->left;
		if(s[i] == '1')
			temp = temp->right;
		i++;
	}
	if(!temp->left && !temp->right)
	{
		cout<<temp->data;
		temp = root;
	}
	cout<<"\n";
}

int main()
{
	char s[60];
	Huffman tree;
	tree.construct();
	tree.encode(0,tree.getroot());
	cout<<"Enter string to decode :";
	cin>>s;
	tree.decode(s);
}
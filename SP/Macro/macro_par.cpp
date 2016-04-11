#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

class mnt
{
public:
	string macro;
	int mdtptr;
	int pntptr;
	int par_no;
};

class macro
{
public:
	string macdef[30];
	int length;
};

class macroprocessor
{
public:
	mnt mtab[10];
	macro mdtab[10];
	string pntab[30];
	string aptab[30];
	int l,l2,mntptr,mdtptr,aptptr,pntptr;
	string pass1[40],pass2[40];
	macroprocessor()
	{
		l=0;l2=0;mntptr = 0;mdtptr = 0;pntptr = 0;aptptr=0;
	}
	void add_mnt(string token,int n);
	int search_mnt(string token);
	int search_pnt(string pn,int first,int last);
	void calc_pass1();
	void print_pass1();
	void print_pass2();
};

int macroprocessor::search_pnt(string pn,int first,int last)
{
	for(int i=first;i<first+last;i++)
	{
		if(pntab[i].compare(pn) == 0)
			return i;
	}
	return -1;
}

void macroprocessor::add_mnt(string token,int n)
{
	mtab[mntptr].mdtptr = mdtptr;
	mtab[mntptr].macro = token;
	if(n == 0)
		mtab[mntptr].pntptr = pntptr;
	else
		mtab[mntptr].pntptr = pntptr-n;
	mtab[mntptr++].par_no = n;
}

int macroprocessor::search_mnt(string token)
{
	for(int i=0;i<mntptr;i++)
	{
		if(mtab[i].macro.compare(token) == 0)
			return i;
	}
	return -1;
}

void macroprocessor::calc_pass1()
{
	string line,token;
	ifstream f("input1.txt");
	while(!f.eof())
	{
		getline(f,line,'\n');
		if(line.compare("macro") == 0)
		{
			int i = 0,n=pntptr;
			string name;
			getline(f,line,'\n');
			istringstream iss(line);
			iss>>name;
			while(iss>>token)
			{
				pntab[pntptr++] = token;
			}
			add_mnt(name,pntptr-n);
			while(1)
			{
				getline(f,line,'\n');
				if(line.compare("mend") == 0)
					break;
				mdtab[mdtptr].macdef[i++] = line;
			}
			mdtab[mdtptr].length = i;
			mdtptr++;
			continue;
		}
		else 
		{
			istringstream iss(line);
			iss>>token;
			int pos = search_mnt(token);
			if(pos == -1)
			{
				pass1[l++] = line;
				pass2[l2++] = line;
				continue;
			}
			pass1[l++] = token;
			int start = mtab[pos].pntptr , last = mtab[pos].par_no,k = aptptr;
			while(iss>>token)
			{
				aptab[aptptr++] = token;
			}
			for(int i=0;i<mdtab[mtab[pos].mdtptr].length;i++)
			{
				ostringstream o1;
				o1<<"+";
				string curr_line = mdtab[mtab[pos].mdtptr].macdef[i];
				istringstream is(curr_line);
				while(is>>token)
				{
					int loc = search_pnt(token,start,last);
					if(loc != -1)
						o1<<aptab[k + loc - start];
					else
						o1<<token;
					o1<<" ";
				}
				pass2[l2++] = o1.str();
			}
		}
	}
}

void macroprocessor::print_pass1()
{
	ofstream f("int.txt");
	cout<<"OUTPUT OF PASS 1 :\n";
	for(int i=0;i<l;i++)
	{
		cout<<pass1[i]<<endl;
		f<<pass1[i]<<endl;
	}
	cout<<"\n";
}

void macroprocessor::print_pass2()
{
	ofstream f("output.txt");
	cout<<"OUTPUT OF PASS 2 :\n";
	for(int i=0;i<l2;i++)
	{
		cout<<pass2[i]<<endl;
		f<<pass2[i]<<endl;
	}
	cout<<"\n";
}

int main()
{
	macroprocessor m;
	m.calc_pass1();
	m.print_pass1();
	m.print_pass2();
}
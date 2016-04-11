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
	int l,l2,mntptr,mdtptr;
	string pass1[40],pass2[40];
	macroprocessor()
	{
		l=0;l2=0;mntptr = 0,mdtptr = 0;
	}
	void add_mnt(string token);
	int search_mnt(string token);
	void calc_pass1();
	void print_pass1();
	void print_pass2();
};

void macroprocessor::add_mnt(string token)
{
	mtab[mntptr].mdtptr = mdtptr;
	mtab[mntptr++].macro = token;
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
			int i = 0;
			getline(f,line,'\n');
			add_mnt(line);
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
			for(int i=0;i<mdtab[mtab[pos].mdtptr].length;i++)
			{
				ostringstream o1;
				o1<<"+";
				pass2[l2++] = o1.str() + mdtab[mtab[pos].mdtptr].macdef[i];
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
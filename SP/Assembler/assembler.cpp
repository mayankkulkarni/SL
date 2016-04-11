#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

class mot
{
public:
	string mnemonic, instr_cl;
	int opcode,length;
};

class symbol_table
{
public:
	string sym;
	int addr;
};

class index
{
public:
	int i,j;
};

class assembler
{
public:
	mot optab[26][5];
	symbol_table symtab[50];
	int bucket[26],s,lc,l,l2;
	string pass1[30],pass2[30];
	assembler()
	{
		for(int i=0;i<26;i++)
			bucket[i] = 0;
		s = 0;
		l=0;
		lc = 0;
		l2 = 0;
	}
	void fill_optab();
	int search_sym(string sym);
	void add_sym(string sym); 
	void calc_pass1();
	void calc_pass2();
	index search_op(string op);
	char isreg(string token);
	void print_pass1();
	void print_pass2();
};

void assembler::add_sym(string sym)
{
	symtab[s++].sym=sym;
}	

int assembler::search_sym(string sym)
{
	int i;
	for(i=0;i<s;i++)
	{
		if(symtab[i].sym.compare(sym) == 0)
			return i;
	}
	return -1;
}

index assembler::search_op(string op)
{
	index r;
	int i;
	r.i=op[0] - 97;
	r.j = -1;
	for(i=0;i<bucket[r.i];i++)
	{
		if(optab[r.i][i].mnemonic.compare(op) == 0)
			r.j = i;
	}
	return r;
}

void assembler::fill_optab()
{
	string line;
	int pos=0;
	ifstream f("optab.txt");
	if(f.is_open())
	{
		while(!f.eof())
		{
			getline(f,line);
			pos = line[0] - 97;
			istringstream iss(line);
			iss>>optab[pos][bucket[pos]].mnemonic;
			iss>>optab[pos][bucket[pos]].instr_cl;
			iss>>optab[pos][bucket[pos]].opcode;
			iss>>optab[pos][bucket[pos]].length;
			bucket[pos]++;
		}
		f.close();
		cout<<"OPTAB GENERATED\n";
	}
	for(int i=0;i<26;i++)
	{
		int j=0;
		while(j < bucket[i])
		{
			cout<<optab[i][j].mnemonic<<" "<<optab[i][j].instr_cl<<" "<<optab[i][j].opcode<<" "<<optab[i][j].length<<"\n";
			j++; 
		}
	}
}

char assembler::isreg(string token)
{
	if(token.compare("areg") == 0)
		return '1';
	else if(token.compare("breg") == 0)
		return '2';
	else if(token.compare("creg") == 0)
		return '3';
	else if(token.compare("dreg") == 0)
		return '4';
	else if(token.compare("gt") == 0)
		return '5';
	else if(token.compare("gte") == 0)
		return '6';
	else if(token.compare("lt") == 0)
		return '7';
	else if(token.compare("lte") == 0)
		return '8';
	return '0';
}

void assembler::calc_pass1()
{
	int itoken=0;
	string line,token;
	ifstream f("source_code.txt");
	if(!f.is_open())
	{
		cout<<"Error opening source code file\n";
		exit(0);
	}
	while(!f.eof())
	{
		getline(f,line,'\n');
		istringstream iss(line);
		iss>>token;
		if(token.compare("start") == 0)
		{
			iss>>itoken;
			lc = itoken;
			ostringstream o1;
			o1<<itoken;
			o1<<" ad , 1 - c , ";
			o1<<itoken;
			pass1[l++] = o1.str();
			continue;
		}
		index r = search_op(token);
		if(r.j == -1)
		{
			if(search_sym(token) == -1)
				add_sym(token);
			else
			{
				int pos = search_sym(token);
				iss>>token;
				r = search_op(token);
				mot h = optab[r.i][r.j];
				if(h.instr_cl.compare("dl") == 0)
				{
					ostringstream o1;
					symtab[pos].addr = lc;
					o1<<lc;
					iss>>token;
					o1<<" dl , ";
					o1<<h.opcode;
					o1<<" c , ";
					o1<<token;
					pass1[l++] = o1.str();
					lc++;
					continue;
				}
			}
		}
		int pos = search_sym(token);
		if(pos != -1)
		{
			symtab[pos].addr = lc;
			iss>>token;
		}
		r = search_op(token);
		if(r.j != -1)
		{
			mot h = optab[r.i][r.j];
			ostringstream o1;
			o1<<lc;
			lc++;
			o1<<" ";	
			if(h.length == 0)
			{
				o1<<h.instr_cl+" , ";
				o1<<h.opcode;
				o1<<" ";
				pass1[l++] = o1.str();
				continue;
			}
			if(h.length == 1)
			{
				o1<<h.instr_cl+" , ";
				o1<<h.opcode;
				o1<<" - s , ";
				iss>>token;
				pos = search_sym(token);
				if(pos == -1)
					add_sym(token);
				o1<<search_sym(token);
				pass1[l++] = o1.str();
				continue;
			}
			if(h.length == 2)
			{
				o1<<h.instr_cl+" , ";
				o1<<h.opcode;
				o1<<" ";
				iss>>token;
				if(isreg(token) == '0')
				{
					cout<<"Invalid identfier at location "<<--lc<<"\n";
					exit(0);
				}
				o1<<isreg(token);
				o1<<" ";
				iss>>token;
				iss>>token;
				pos = search_sym(token);
				if(pos == -1)
					add_sym(token);
				o1<<"s , ";
				o1<<search_sym(token);
				pass1[l++] = o1.str();
			}
		}
	}
}

void assembler::calc_pass2()
{
	int loc;
	string token;
	for(int i=0;i<l;i++)
	{
		istringstream iss(pass1[i]);
		iss>>loc;
		iss>>token;
		if(token.compare("is") != 0)
		{
			ostringstream o1;
			o1<<loc;
			pass2[l2++] = o1.str();
			continue;
		}
		else
		{
			ostringstream o1;
			o1<<loc;
			o1<<" ";
			iss>>token;
			iss>>token;
			o1<<token;
			o1<<" ";
			iss>>token;
			o1<<token;
			o1<<" ";
			iss>>token;
			iss>>token;
			iss>>loc;
			o1<<symtab[loc].addr;
			pass2[l2++] = o1.str();
		}
	}
}

void assembler::print_pass1()
{
	ofstream op("pass1.txt");
	cout<<"OUTPUT OF PASS 1 :\n";
	for(int i=0;i<l;i++)
	{
		cout<<pass1[i]<<endl;
		op<<pass1[i]<<endl;
	}
}

void assembler::print_pass2()
{
	ofstream op("pass2.txt");
	cout<<"\nOUTPUT OF PASS 2 :\n";
	for(int i=0;i<l;i++)
	{
		cout<<pass2[i]<<endl;
		op<<pass2[i]<<endl;
	}
}

int main()
{
	assembler a;
	a.fill_optab();
	a.calc_pass1();
	a.print_pass1();
	a.calc_pass2();
	a.print_pass2();
}
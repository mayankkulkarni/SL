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

class index
{
public:
	int i,j;
};

class littab
{
public:
	string literal;
	int add;
};

class pooltab
{
public:
	int start_lit;
};

class assembler
{
public:
	mot optab[26][5];
	littab ltab[30];
	pooltab ptab[10];
	int bucket[26],litptr,lc,l,l2,poolptr;
	string pass1[50],pass2[50];
	assembler()
	{
		for(int i=0;i<26;i++)
			bucket[i] = 0;
		litptr = 0;
		poolptr = 0;
		l=0;
		lc = 0;
		l2 = 0;
		ptab[0].start_lit = 0;
	}
	void fill_optab();
	void add_lit(string literal);
	int search_lit(string token);
	void calc_pass1();
	void calc_pass2();
	index search_op(string op);
	char isreg(string token);
	void print_pass1();
	void print_pass2();
};

void assembler::add_lit(string literal)
{
	ltab[litptr++].literal = literal;
}

int assembler::search_lit(string token)
{
	for(int i = ptab[poolptr].start_lit;i<litptr;i++){
		if(ltab[i].literal.compare(token) == 0)
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
	else if(token.compare("le") == 0)
		return '8';
	return '0';
}

void assembler::fill_optab()
{
	string line;
	int pos=0;
	ifstream f("optab_lit.txt");
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

void assembler::calc_pass1()
{
	lc=0;
	int itoken;
	string line,token;
	ifstream f("source_code_lit.txt");
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
			cout<<"Invalid instruction at : "<<lc;
			exit(0);
		}
		else
		{
			mot h = optab[r.i][r.j];
			if(h.mnemonic.compare("ltorg")==0)
			{
				int temp = lc;
				for(int i=ptab[poolptr].start_lit;i<litptr;i++)
				{
					ltab[i].add = lc++;
				}
				poolptr++;
				ptab[poolptr].start_lit = litptr;
				ostringstream o1;
				o1<<temp;
				o1<<" ";
				o1<<h.instr_cl<<" , ";
				o1<<h.opcode;
				pass1[l++] = o1.str();
				for(int i=ptab[poolptr-1].start_lit;i<litptr;i++)
				{
					ostringstream o2;
					o2<<ltab[i].add<<" ";
					o2<<ltab[i].literal;
					pass1[l++] = o2.str();
				}
				continue;
			}
			if(h.length == 0)
			{
				ostringstream o1;
				o1<<lc;
				lc++;
				o1<<" "<<h.instr_cl<<" , ";
				o1<<h.opcode;
				pass1[l++] = o1.str();
				continue;
			}

			if(h.length == 1)
			{
				iss>>token;
				ostringstream o1;
				o1<<lc;
				lc++;
				o1<<" "<<h.instr_cl<<" , ";
				o1<<h.opcode<<" ";
				if(isreg(token) != '0')
				{
					o1<<isreg(token);
				}
				else
				{
					int pos = search_lit(token);
					if(pos == -1)
						add_lit(token);
					o1<<"l , ";
					o1<<search_lit(token);		
				}
				pass1[l++] = o1.str();
			}
			if(h.length == 2)
			{
				ostringstream o1;
				o1<<lc;
				lc++;
				o1<<" "<<h.instr_cl<<" , ";
				o1<<h.opcode<<" ";
				iss>>token;
				if(isreg(token) == '0')
				{
					cout<<"Invalid instruction at "<<--lc<<"\n";
					exit(0);
				}
				o1<<isreg(token)<<" ";
				iss>>token;
				iss>>token;
				int pos = search_lit(token);
				if(pos == -1)
					add_lit(token);
				pos = search_lit(token);
				o1<<"l , "<<pos;
				pass1[l++] = o1.str();
			}
		}
	}
	f.close();
	int temp = lc;
	for(int i=ptab[poolptr].start_lit;i<litptr;i++)
	{
		ltab[i].add = temp++;
	}
	poolptr++;
	ptab[poolptr].start_lit = litptr;
	for(int i=ptab[poolptr-1].start_lit;i<litptr;i++)
	{
		ostringstream o2;
		o2<<ltab[i].add<<" ";
		o2<<ltab[i].literal;
		pass1[l++] = o2.str();
	}
}

void assembler::calc_pass2()
{
	int loc,p = ptab[0].start_lit;
	string token;
	for(int i=0;i<l;i++)
	{
		istringstream iss(pass1[i]);
		iss>>loc;
		if(loc == 0)
			continue;
		iss>>token;
		if((token.compare("is") != 0) && (token.compare("dl") != 0) && (token.compare("ad") != 0))
		{
			ostringstream o1;
			o1<<loc;
			o1<<" ";
			iss>>token;
			o1<<token;
			pass2[l2++] = o1.str();
		}
		else if(token.compare("is") != 0)
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
			o1<<ltab[loc].add;
			pass2[l2++] = o1.str();
		}
	}
}

void assembler::print_pass1()
{
	int i;
	string token;
	ofstream op("pass1.txt");
	cout<<"OUTPUT OF PASS 1 :\n";
	for(i=0;i<l;i++)
	{
		istringstream iss(pass1[i]);
		iss>>token;
		iss>>token;
		if((token.compare("is") != 0) && (token.compare("dl") != 0) && (token.compare("ad") != 0))
			continue;
		cout<<pass1[i]<<endl;
		op<<pass1[i]<<endl;
	}
}

void assembler::print_pass2()
{
	int i;
	ofstream op("pass2.txt");
	cout<<"\nOUTPUT OF PASS 2 :\n";
	for(i=0;i<l;i++)
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
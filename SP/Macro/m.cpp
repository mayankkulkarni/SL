/*
 * m.cpp
 *
 *  Created on: 14-Mar-2016
 *      Author: pict
 */
#include<iostream>
#include<fstream>
#include<sstream>
using std::cout;
using std::cin;
using std::string;
class macro_name_table
{
	string name;
	int count;
	int count_key;
	int pt;
	int kpt;
	int mdtp;
	friend class macro;
};
class parameter_table
{
	string parameter;
	string number;
	friend class macro;
};
class key_parameter_table
{	string parameter;
	string number;
	string value;
	friend class macro;
};
class actual_parameter_table
{
	string parameter;
	string value;
	friend class macro;
};
class macro
{
	macro_name_table mnt[5];
	parameter_table pt[15];
	key_parameter_table kpt[15];
	actual_parameter_table apt[15];
	string mdt[30],pass2[30];
	int mnt_t,pt_t,kpt_t,mdtp,apt_t,l;
public:
	macro()
	{
		mnt_t=pt_t=kpt_t=mdtp=apt_t=l=0;
	}
	string searchpt(int n,string para)
	{
		for(int i=n;;i++)
			if(para.compare(pt[i].parameter)==0)
				return pt[i].number;
		return "";
	}
	void printpt()
	{	cout<<"parameter table:\nIndex\tNo.\tParameter\n";
		for(int i=0;i<pt_t;i++)
			cout<<i<<"\t"<<pt[i].number<<"\t"<<pt[i].parameter<<"\n";
	}
	void printmnt()
	{
		cout<<"macro name table:\nNo.\tName\tNo.P\tno.KP\tPt\tKPT\tMDTP\n";
		for(int i=0;i<mnt_t;i++)
			cout<<i<<"\t"<<mnt[i].name<<"\t"<<mnt[i].count<<"\t"<<mnt[i].count_key<<"\t"<<mnt[i].pt<<"\t"<<mnt[i].kpt<<"\t"<<mnt[i].mdtp<<"\n ";
	}
	void printmdt()
	{	cout<<"Macro Definition Table:\n";
		for(int i=0;i<mdtp;i++)
		{
			cout<<i<<". "<<mdt[i]<<std::endl;
		}
	}
	void print_kpt()
	{
		cout<<"Key parameter table:\n";
				for(int i=0;i<kpt_t;i++)
				{
					cout<<i<<". "<<kpt[i].number<<" "<<kpt[i].parameter<<" "<<kpt[i].value<<std::endl;
				}
	}
	void pass_I()
	{	string token,line;
		std::ifstream file("input.txt");
		std::ofstream file1("int.txt");
		while(!file.eof())
		{	std::getline(file,line);
			std::istringstream iss1(line);
			iss1>>token;
			if(token.compare("macro")==0)
			{	int c=01,ck=01;
				std::getline(file,line);
				std::istringstream iss(line);
				iss>>token;
				mnt[mnt_t].pt=-1;
				mnt[mnt_t].kpt=-1;
				mnt[mnt_t].name=token;
				while(iss>>token)
				{	if(c==1)
						mnt[mnt_t].pt=pt_t;
					if(token.find("=")!=std::string::npos)
					{	if (ck==1)
							mnt[mnt_t].kpt=kpt_t;
						int temp=token.find("=");

						kpt[kpt_t].parameter=token.substr(0,temp);

						kpt[kpt_t].value=token.substr(temp+1,token.length());
						std::ostringstream oss;
						oss<<"#"<<c;
						ck++;
						kpt[kpt_t++].number=oss.str();
						pt[pt_t].parameter=token.substr(0,temp);
					    std::ostringstream oss1;
						oss1<<"#"<<c++;
					    pt[pt_t++].number=oss1.str();

					}
					else
					{	pt[pt_t].parameter=token;
						std::ostringstream oss;
						oss<<"#"<<c++;
						pt[pt_t++].number=oss.str();
					}
				}
				mnt[mnt_t].count=c-1;
				mnt[mnt_t].count_key=ck-1;
				std::getline(file,line);
				mnt[mnt_t].mdtp=mdtp;
				while(line.compare("mend")!=0)
				{
					std::istringstream iss(line);
					std::ostringstream oss;
					while(iss>>token)
					{
						if(token[0]=='&')
						{
							oss<<searchpt(mnt[mnt_t].pt,token)<<" ";
						}
						else
							oss<<token<<" ";
					}
					mdt[mdtp++]=oss.str();
					std::getline(file,line);
				}
				mdt[mdtp++]="mend";
				mnt_t++;
			}
			else
				file1<<line<<"\n";
		}
	}
	int searchmnt(string name)
	{
		for(int i=0;i<mnt_t;i++)
			if(name.compare(mnt[i].name)==0)
				return i;
		return -1;
	}
	int get_index_kpt(string token,int p,int c)
	{
		for(int i=p;i<p+c;i++)
		{
			if(kpt[i].number.compare(token)==0)
				return i;
		}
		return -1;
	}
	string get_no_kpt(string para,int p,int c)
	{
		for(int i=p;i<p+c;i++)
		{
			if(kpt[i].parameter.compare(para)==0)
				return kpt[i].number;
		}
		return "";
	}
	void pass_II()
	{
		std::ifstream in("int.txt");
		string token,line;
		while(!in.eof())
		{	std::ostringstream oss;
			std::getline(in,line);
			std::istringstream iss(line);
			iss>>token;
			if(searchmnt(token)!=-1)
			{
				int k=searchmnt(token);
				int c=mnt[k].count;
				for(int i=0;i<c;i++)
				{
					apt[i].parameter=pt[mnt[k].pt+i].number;
					int temp=get_index_kpt(apt[i].parameter,mnt[k].kpt,mnt[k].count_key);
					if(temp!=-1)
					{
						apt[i].value=kpt[temp].value;
					}

				}
				for(int i=0;i<c;i++)
				{
					if(iss>>token)
					{	if(token.find("=")!=std::string::npos)
						{	string para,value;
							std::istringstream iss2(token);
							std::getline(iss2,para,'=');
							std::getline(iss2,value,'=');
							para='&'+para;
							string number=get_no_kpt(para,mnt[k].kpt,mnt[k].count_key);
							for(int i=0;i<c;i++)
							{
								if(number.compare(apt[i].parameter)==0)
									apt[i].value=value;
							}
						}
						else
						{
							apt[i].value=token;
						}
					}
				}
				int c1=0;
				string token1;
				while(mdt[mnt[k].mdtp+c1].compare("mend")!=0)
				{	std::ostringstream oss1;
					oss1<<"+";
					std::istringstream iss1(mdt[mnt[k].mdtp+c1]);
					while(iss1>>token1)
					{	if(token1[0]!='#')
							oss1<<token1<<" ";
						else
						{
							for(int j=0;j<c;j++)
							{
								if(apt[j].parameter.compare(token1)==0)
									oss1<<apt[j].value<<" ";
							}
						}

					}
					c1++;
					pass2[l++]=oss1.str();

				}
			}
			else
			{	oss<<line;
				pass2[l++]=oss.str();
			}
		}
	}
	void op_passII()
	{
		for(int i=0;i<l;i++)
			cout<<pass2[i]<<std::endl;
	}
};
int main(void)
{
	macro m;
	m.pass_I();
	m.printmnt();
	m.printmdt();
	m.printpt();
	m.print_kpt();
	m.pass_II();
	m.op_passII();

	return 0;
}
/*
 * Output:
 macro name table:
No.	Name	No.P	no.KP	Pt	KPT	MDTP
0	mac1	0	0	-1	-1	0
 1	eval	3	0	0	-1	4
 2	calc	4	2	3	0	9
 Macro Definition Table:
0. mover areg m
1. add breg m
2. movem creg m
3. mend
4. mover areg #1
5. sub areg #2
6. add areg #3
7. mover areg #3
8. mend
9. #4 mover areg #1
10. #3 areg #2
11. movem areg #1
12. mend
parameter table:
Index	No.	Parameter
0	#1	&x
1	#2	&y
2	#3	&z
3	#1	&x
4	#2	&y
5	#3	&op
6	#4	&lab


 */

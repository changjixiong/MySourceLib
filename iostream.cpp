#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <string>

using namespace std;

int main()
{
	ifstream fin;
	ofstream fout;
	fin.open("fpga_data_hex.txt");
	string strContent;
	string strContentOut;
	string strtemp1;
	string strtemp2;
	while(getline(fin,strtemp1))
	{
		strContent+=strtemp1;
	}

	int i=strContent.length();
	
	fin.close();
	while(strContent.length()>0)
	{
		strtemp1=strContent.substr(0,1024);
		strContent=strContent.substr(1024);	
		strtemp2="";

		while(strtemp1.length()>0)
		{
			strtemp2+="0x"+strtemp1.substr(0,2);
			strtemp1=strtemp1.substr(2);
			strtemp2+=",";
		}

		strtemp2=strtemp2.substr(0,strtemp2.length()-1);
		strContentOut+="{";
		strContentOut+=strtemp2;
		strContentOut+="},";
		
	}
	strContentOut="{"+strContentOut;
	strContentOut[strContentOut.length()-1]='}';
	

	fout.open("fpga_data_hex_out.txt");
	fout<<strContentOut;
	fout.close();
	
	

	return 0;
}


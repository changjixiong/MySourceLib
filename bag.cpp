#include<stdio.h>
#include<string>
#include<iostream>
#include <math.h>

using namespace std;
int main()
{
	
	
	int TimeNeed[100];
	int Values[100];
	int x[1001]={0};
	int TimeTotal;
	int gresstotal;
	
	cin>>TimeTotal;
	cin>>gresstotal;
	
	int i,j;
	for(i=0;i<gresstotal;i++)
	{
		cin>>TimeNeed[i];
		cin>>Values[i];
	}
	
	for(i=0;i<gresstotal;i++)
	{
		for(j=TimeTotal;j>=TimeNeed[i];j--)
		{
			if(x[j-TimeNeed[i]]+Values[i]>x[j])
			{
				x[j]=x[j-TimeNeed[i]]+Values[i];
			}
		}
	}
	
	cout<<x[TimeTotal];
	
	return 0;
	
}

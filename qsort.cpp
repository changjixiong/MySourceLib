#include <windows.h>
#include <time.h>
#include "stdio.h"



void printNum(int * num,int total)
{
	int i=0;
	for (i=0;i<total-1;i++)
	{
		printf("%d,",num[i]);
	}
	printf("%d\n",num[i]);
	
}

void qsort(int *pData,int left,int right)
{
	int i=left;
	int j=right;
	int middle=0;
	int nTemp=0;

	middle=pData[(left+right)/2];//求中间值
	//middle=pData[(rand()%(right-left+1))+left]; //生成大于等于left小于等于right的随机数

	do{
		while((pData[i]<middle)&&(i<right))//从左扫描大于中值的数
			i++;

		while((pData[j]>middle) && (j>left))//从右扫描小于中值的数
			j--;

		//找到了一对值,交换
		if(i<=j)
		{
			nTemp=pData[j];
			pData[j]=pData[i];
			pData[i]=nTemp;
			i++;
			j--;
		}

	}while(i<=j);//如果两边扫描的下标交错，就停止（完成一次）
	
	//当左边部分有值(left<j)，递归左半边
	if(left<j)
	{
		qsort(pData,left,j);
	}

	//当右边部分有值(right>i)，递归右半边
	if(right>i)
	{
		qsort(pData,i,right);
	}
} 

int main()
{

	srand((unsigned)time(NULL));
	int a[100];

	for (int i=0;i<100;i++)
	{
		a[i]=rand()%100;
	}

	printNum(a,100);
	printf("\n");

	qsort(a,0,99);

	printNum(a,100);

	getchar();

	return 0;
}

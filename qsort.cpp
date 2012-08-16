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

	middle=pData[(left+right)/2];//���м�ֵ
	//middle=pData[(rand()%(right-left+1))+left]; //���ɴ��ڵ���leftС�ڵ���right�������

	do{
		while((pData[i]<middle)&&(i<right))//����ɨ�������ֵ����
			i++;

		while((pData[j]>middle) && (j>left))//����ɨ��С����ֵ����
			j--;

		//�ҵ���һ��ֵ,����
		if(i<=j)
		{
			nTemp=pData[j];
			pData[j]=pData[i];
			pData[i]=nTemp;
			i++;
			j--;
		}

	}while(i<=j);//�������ɨ����±꽻����ֹͣ�����һ�Σ�
	
	//����߲�����ֵ(left<j)���ݹ�����
	if(left<j)
	{
		qsort(pData,left,j);
	}

	//���ұ߲�����ֵ(right>i)���ݹ��Ұ��
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

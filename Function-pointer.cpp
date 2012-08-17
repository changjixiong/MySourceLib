int g(int);
int (*gp)(int);

gp=g;

typedef int (*Fun)(int a, int b);
int m(int, int);

Fun funp = m;


////////////////////////////////////////////////////
#include <stdio.h>

struct MSG_MAP 
{
	int nMessage;
	long (*pfn)();
};

long f1()
{
	return printf("f1\n");
}

long f2()
{
	return printf("f2\n");
}

long f3()
{
	return printf("f3\n");
}

struct MSG_MAP _msgmap[3]=
{
	1,f1,
	2,f2,
	3,f3,
};

long CallFun(int n)
{
	for (int i=0;i<(sizeof(_msgmap)/sizeof(_msgmap[0]));i++)
	{
		if (n==_msgmap[i].nMessage)
		{
			return (*_msgmap[i].pfn)();
		}
	}
}

int main()
{
	CallFun(3);
	CallFun(1);
	CallFun(2);
	printf("hello world\n");
}


#include <python.h>
#include <stdio.h>
#include <iostream>
using namespace std;


void ExecPythonCommand()
{
	//直接执行
	PyRun_SimpleString("from time import time,ctime\n"
						"print 'Today is',ctime(time())\n");
}

//调用无参数函数
void InvokeNoParm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	//导入模块
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		//获取函数地址
		pFunc = PyObject_GetAttrString(pMod, "a");
		if(pFunc)
		{
			//函数调用
			PyEval_CallObject(pFunc, NULL);
		}
		else
		{
			cout << "cannot find function a" << endl;
		}
	}
	else
	{
		cout << "cannot find mymod.py" << endl;
	}
}

//调用一参数函数
void InvokeWith1Parm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	int   iRetVal = 0;
	//导入模块
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "square");
		if(pFunc)
		{
		//创建参数
		pParm = Py_BuildValue("(i)", 5);
		//函数调用
		pRetVal = PyEval_CallObject(pFunc, pParm);
		//解析返回值
		PyArg_Parse(pRetVal, "i", &iRetVal);
		cout << "square 5 is: " << iRetVal << endl;
		}
		else
		{
		cout << "cannot find function square" << endl;
		}
	}
	else
	{
	cout << "cannot find mymod.py" << endl;
	}
}

//调用多参数函数
void InvokeWith2Parm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	int   iRetVal = 0;
	//导入模块
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "add");
		if(pFunc)
		{
			//创建两个参数
			pParm = PyTuple_New(2);
			//为参数赋值
			PyTuple_SetItem(pParm, 0, Py_BuildValue("i",2000));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("i",3000));
			//函数调用
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//解析返回值
			PyArg_Parse(pRetVal, "i", &iRetVal);
			cout << "2000 + 3000 = " << iRetVal << endl;
		}
		else
		{
		cout << "cannot find function square" << endl;
		}
	}
	else
	{
	cout << "cannot find mymod.py" << endl;
	}
}

void InvokeWith2Parm2()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	float   iRetVal;

	//导入模块
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "normal");
		if(pFunc)
		{
			//创建两个参数
			pParm = PyTuple_New(2);
			//为参数赋值
			PyTuple_SetItem(pParm, 0, Py_BuildValue("f",10.0));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("f",0.01));
			//函数调用
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//解析返回值
			PyArg_Parse(pRetVal, "f", &iRetVal);
			cout << "normal(10,2) = " << iRetVal << endl;
		}
		else
		{
			cout << "cannot find function normal" << endl;
		}
	}
	else
	{
		cout << "cannot find mymod.py" << endl;
	}
}

void InvokeWith2Parm3()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	char *   iRetVal;
	
	//导入模块
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "addString");
		if(pFunc)
		{
			//创建两个参数
			pParm = PyTuple_New(2);
			//为参数赋值
			PyTuple_SetItem(pParm, 0, Py_BuildValue("s","ab"));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("i",5));
			//函数调用
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//解析返回值
			PyArg_Parse(pRetVal, "s", &iRetVal);
			cout << "addString(ab,5) = " << iRetVal << endl;
		}
		else
		{
			cout << "cannot find function addString" << endl;
		}
	}
	else
	{
		cout << "cannot find mymod.py" << endl;
	}
}

int main()
{
	char *cstr;
	PyObject *pstr, *pmod, *pdict,*main_dict;
	PyObject *pfunc, *pargs;

	Py_Initialize();                       //python   解释器的初始化
	
	if(!Py_IsInitialized())
	{
		printf( "can 't   init   python ");
		return -1;
	} 


	ExecPythonCommand();
	InvokeNoParm();
	InvokeWith1Parm();
	InvokeWith2Parm();
	InvokeWith2Parm2();
	InvokeWith2Parm3();
	Py_Finalize();
	
	return 0;

	
	
	
	
	
 return 0;
}

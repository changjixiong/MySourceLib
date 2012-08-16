#include <python.h>
#include <stdio.h>
#include <iostream>
using namespace std;


void ExecPythonCommand()
{
	//ֱ��ִ��
	PyRun_SimpleString("from time import time,ctime\n"
						"print 'Today is',ctime(time())\n");
}

//�����޲�������
void InvokeNoParm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	//����ģ��
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		//��ȡ������ַ
		pFunc = PyObject_GetAttrString(pMod, "a");
		if(pFunc)
		{
			//��������
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

//����һ��������
void InvokeWith1Parm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	int   iRetVal = 0;
	//����ģ��
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "square");
		if(pFunc)
		{
		//��������
		pParm = Py_BuildValue("(i)", 5);
		//��������
		pRetVal = PyEval_CallObject(pFunc, pParm);
		//��������ֵ
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

//���ö��������
void InvokeWith2Parm()
{
	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	int   iRetVal = 0;
	//����ģ��
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "add");
		if(pFunc)
		{
			//������������
			pParm = PyTuple_New(2);
			//Ϊ������ֵ
			PyTuple_SetItem(pParm, 0, Py_BuildValue("i",2000));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("i",3000));
			//��������
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//��������ֵ
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

	//����ģ��
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "normal");
		if(pFunc)
		{
			//������������
			pParm = PyTuple_New(2);
			//Ϊ������ֵ
			PyTuple_SetItem(pParm, 0, Py_BuildValue("f",10.0));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("f",0.01));
			//��������
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//��������ֵ
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
	
	//����ģ��
	pMod = PyImport_ImportModule("mymod");
	if(pMod)
	{
		pFunc = PyObject_GetAttrString(pMod, "addString");
		if(pFunc)
		{
			//������������
			pParm = PyTuple_New(2);
			//Ϊ������ֵ
			PyTuple_SetItem(pParm, 0, Py_BuildValue("s","ab"));
			PyTuple_SetItem(pParm, 1, Py_BuildValue("i",5));
			//��������
			pRetVal = PyEval_CallObject(pFunc, pParm);
			//��������ֵ
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

	Py_Initialize();                       //python   �������ĳ�ʼ��
	
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

#include <stdio.h>

class CTreeNode
{
public:
	CTreeNode()
	{
		pRchild=NULL;
		pLchild=NULL;
	}
// 	const CNode & operator=(const CNode& node)
// 	{
// 		this->nValue=node.nValue;
// 		this->pLchild=node.pLchild;
// 		this->pRchild=node.pRchild;
// 
// 		return *this;
// 	}

public:
	int nValue;
	CTreeNode * pRchild,* pLchild;
};

typedef CTreeNode * BTree;



class CQueueNode
{
public:
	CQueueNode()
	{
		pNode=NULL;
		pNext=NULL;
	}

	CTreeNode * pNode;
	CQueueNode * pNext;
};

class CQueue
{
public:
	CQueue()
	{
		pTop=NULL;
		pBottom=NULL;
	}

	CQueueNode * pTop;
	CQueueNode * pBottom;
};

bool QueueIsEmpty(CQueue & queue)
{
	
	return queue.pTop==NULL ? true:false;
}

void EnQueue(CQueue & queue,CTreeNode * pNode)
{
	if (pNode==NULL)
	{
		return;
	}

	CQueueNode * pTempQueueNode=new CQueueNode();
	pTempQueueNode->pNode=pNode;

	if (queue.pTop==NULL)
	{
		queue.pTop=pTempQueueNode;
		queue.pBottom=queue.pTop;
	}
	else
	{
		queue.pBottom->pNext=pTempQueueNode;
		queue.pBottom=pTempQueueNode;
	}
}

bool DeQueue(CQueue & queue,CQueueNode ** pNode)
{
	if (queue.pTop==NULL)
	{
		*pNode=NULL;
		return false;
	}
	else
	{
		*pNode=queue.pTop;
		queue.pTop=queue.pTop->pNext;
		(*pNode)->pNext=NULL;
		if (QueueIsEmpty(queue))
		{
			queue.pBottom=NULL;
		}	
		return true;
	}
}

int QueueElemNum(CQueue & queue)
{
	int num=0;
	CQueueNode * pTempNode=queue.pTop;
	while (pTempNode)
	{
		num++;
		pTempNode=pTempNode->pNext;
	}

	return num;
}



void Destroy(BTree * root)
{
	if (*root)//not null
	{
		if ((*root)->pLchild)
		{
			Destroy(&(*root)->pLchild);
		}

		if ((*root)->pRchild)
		{
			Destroy(&(*root)->pRchild);
		}
	}

	delete (*root);
	*root=NULL;
}

int AddNode(BTree * root,int key)
{
	
	if (*root)
	{
		if (key==(*root)->nValue)
		{
			return -1;
		}

		if (key<(*root)->nValue)//加在左边
		{
			if ((*root)->pLchild)
			{
					AddNode(&(*root)->pLchild,key);				
			}
			else
			{	
				CTreeNode * ptempNode=new CTreeNode();
				ptempNode->nValue=key;

				
				(*root)->pLchild=ptempNode;
			}
		}
		else //加在右边
		{
			if ((*root)->pRchild)
			{
				AddNode(&(*root)->pRchild,key);
			}
			else
			{
				CTreeNode * ptempNode=new CTreeNode();
				ptempNode->nValue=key;
		
				(*root)->pRchild=ptempNode;
			}
		}

		return 0;
	}
	else
	{
		CTreeNode * ptempNode=new CTreeNode();
		ptempNode->nValue=key;

		*root=ptempNode;

		return 0;
	}
}

void PrintTreePreOrder(BTree * root)
{
	if (*root)
	{
		printf("%d\n",(*root)->nValue);
	}

	if ((*root)->pLchild)
	{
		PrintTreePreOrder(&(*root)->pLchild);
	}

	if ((*root)->pRchild)
	{
		PrintTreePreOrder(&(*root)->pRchild);
	}
	
}

void PrintTreeIneOrder(BTree * root)
{

	if ((*root)->pLchild)
	{
		PrintTreeIneOrder(&(*root)->pLchild);
	}

	if (*root)
	{
		printf("%d\n",(*root)->nValue);
	}
	
	if ((*root)->pRchild)
	{
		PrintTreeIneOrder(&(*root)->pRchild);
	}
}

void PrintTreePosteOrder(BTree * root)
{
	
	if ((*root)->pLchild)
	{
		PrintTreePosteOrder(&(*root)->pLchild);
	}
	
	if ((*root)->pRchild)
	{
		PrintTreePosteOrder(&(*root)->pRchild);
	}

	if (*root)
	{
		printf("%d\n",(*root)->nValue);
	}
	
}

void PrintLevelOrder(BTree * root)
{
	if (*root==NULL)
	{
		return;
	}

	CQueue * pQueue=new CQueue();
	

	EnQueue(*pQueue,*root);

	CQueueNode *ptempQueueNode=NULL;

	while (DeQueue(*pQueue,&ptempQueueNode))
	{
		printf("%d\n",ptempQueueNode->pNode->nValue);
		EnQueue(*pQueue,ptempQueueNode->pNode->pLchild);
		EnQueue(*pQueue,ptempQueueNode->pNode->pRchild);
	}	
}

int main()
{
	BTree root=NULL;
	AddNode(&root,20);
	AddNode(&root,10);
	AddNode(&root,30);

// 	PrintTreeIneOrder(&root);
// 	Destroy(&root);
// 	printf("\n");

	AddNode(&root,25);
	AddNode(&root,15);	
	AddNode(&root,35);

	AddNode(&root,27);
	AddNode(&root,17);	
	AddNode(&root,32);


//	PrintTreePreOrder(&root);
//	PrintTreeIneOrder(&root);
//	PrintTreePosteOrder(&root);

	PrintLevelOrder(&root);


	//printf("hello world!\n");
	return 0;
}
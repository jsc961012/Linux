#include "../include/list.h"

//创建二叉排序树
int Insert(BiTree *T, pLinkNode_t i)
{
	//printf("%d   %s\n",i->data.uiLinkId,i->data.acName);
	BiTree parent;   //表示双亲结点；
	BiTree head = (*T);
	BiTree p = (BiTree)malloc(sizeof(BiTNode));
	p->data = i->data;
	p->lchild = NULL;
	p->rchild = NULL;
	//查找需要添加的父结点，这个父结点是度为0的结点；
	while (head)
	{
		parent = head;
		if (i->data.uiLinkId < head->data.uiLinkId)   //若关键字小于结点的数据；
		{
			head = head->lchild; //在左子树上查找；
		}
		else   //若关键字大于结点的数据；
		{
			head = head->rchild;  //在右子树上查找；
		}
	}
	//判断添加到左子树还是右子树；
	if (i->data.uiLinkId< parent->data.uiLinkId)   //小于父结点；
	{
		parent->lchild = p;    //添加到左子树；
	}
	else    //大于父结点；
	{
		parent->rchild = p;     //添加到右子树；
	}
	//if (!(*T))
	//{
	//	(*T) = (BiTree)malloc(sizeof(BiTNode));//为T分配空间
	//	(*T)->data = i;//数据域赋值
	//	(*T)->lchild = (*T)->rchild = NULL;
	//	return 0;
	//}
	//if (i.uiLinkId == &(*T)->data.uiLinkId)
	//	return 0;
	//else if (i.uiLinkId < &(*T)->data.uiLinkId)
	//	Insert( &(*T)->lchild, i);
	//else
	//	Insert( &(*T)->rchild, i);
	//return 1;
}


// 创建空链表 
pLinkNode_t createLink()
{
	// 申请空间
	pLinkNode_t h = (pLinkNode_t)malloc(sizeof(linkNode_t));
	if (NULL == h)
	{
		perror("malloc:");
		return NULL;
	}

	// 初始化为空链表
	h->next = NULL;
	return h;
}


// 插入数据到链表(iPos 从0开始算)
// 0: 成功  1: 失败
int insertLink(pLinkNode_t h, int iPos, data_t x)
{
	pLinkNode_t p = h;
	int i = 0;
	// 找插入点的前驱
	while (i < iPos && p == NULL)
	{
		p = p->next;
		i++;
	}

	// 找到前驱

	pLinkNode_t q = (pLinkNode_t)malloc(sizeof(linkNode_t));
	if (NULL == q)
	{
		return -1;
	}
	q->data = x;
	q->next = p->next;
	p->next = q;

	return 0;
}

/*
void display(pLinkNode_t h)
{
	pLinkNode_t p = h->next;
	while(p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");

}*/

void writeToTxtFile(pLinkNode_t h, char* pathName)
{
	pLinkNode_t p = h->next;
	data_t msg;
	int i = 0;

	h->count = 0;
	FILE* fw = fopen(pathName, "w");
	if (NULL == fw)
	{
		perror("open fail:");
		exit(-1);
	}

	while (p != NULL)
	{
		msg = p->data;
		if (msg.uiFlag != 0)
		{
			// 有道路名称
			fprintf(fw, "%d\tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				++i, msg.usSize, msg.uiLinkId, msg.usNameSize, msg.uiClass, msg.uiBranch, msg.uiFlag, msg.acName);  // UTF-8
			h->count++;
		}
		else
		{
			fprintf(fw, "%d\tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				++i, msg.usSize, msg.uiLinkId, msg.usNameSize, msg.uiClass, msg.uiBranch, msg.uiFlag, "No Name!!!");  // UTF-8
			h->count++;
		}
		// 下一个
		p = p->next;
	}
	printf("数据长度：%d\n", h->count);
	fclose(fw);
}



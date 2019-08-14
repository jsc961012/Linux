#include <stdio.h>
#include <stdlib.h>
#include "../include/list.h"

//快速排序
void Partition(pLinkNode_t pBegin, pLinkNode_t pEnd) {
	//if (isSort(pBegin))
	//	printf("列表已排好序。。。。。。。\n");
	//else
	//{
	//	pBegin = pBegin->next;
	if (NULL == pBegin || NULL == pEnd || pBegin == pEnd)
	{
		return;
	}

	pLinkNode_t pFirst = pBegin;
	pLinkNode_t pSecond = pFirst->next;
	data_t t;



	int nMidValue = pBegin->data.uiLinkId;

	//开始排序
	while (pSecond != pEnd->next && pSecond != NULL)
	{
		if (pSecond->data.uiLinkId < nMidValue)
		{
			pFirst = pFirst->next;

			//swap(pFirst->data.uiLinkId, pSecond->data.uiLinkId);
			t = pFirst->data;
			pFirst->data = pSecond->data;
			pSecond->data = t;
		}

		pSecond = pSecond->next;
	}

	//更换支点后完成1轮排序
	//swap(pBegin->data.uiLinkId, pFirst->data.uiLinkId);
	t = pBegin->data;
	pBegin->data = pFirst->data;
	pFirst->data = t;

	Partition(pBegin, pFirst);
	Partition(pFirst->next, pEnd);//此处的next很重要,不然将导致死循环


	//printf("快速排序成功，排序后文件为文件sort.txt");
	//writeToTxtFile(msghead, "../sort.txt");
//}
}


//冒泡排序
pLinkNode_t BubbleSort(pLinkNode_t head) {
	pLinkNode_t p, q;
	data_t t;
	if (head == NULL || head->next == NULL)
		return(head);

	for (p = head->next; p != NULL; p = p->next)
	{
		for (q = p->next; q != NULL; q = q->next)
		{
			if (p->data.uiLinkId > q->data.uiLinkId)
			{

				t = p->data;
				p->data = q->data;
				q->data = t;
			}
		}
	}
	return head;

}

//插入排序
void InsertSort(pLinkNode_t head) {
	pLinkNode_t p = head->next,  q = NULL,  r = NULL; //p指向第一个数据节点

	if (p != NULL) //单链表有一个或者以上的数据节点
	
		r = p->next; //r 保存 *p节点的直接后继节点的指针
		p->next = NULL; //构造只含有一个数据节点的有序表
		p = r;
		while (p != NULL)
		{
			r = p->next; //r 保存*p节点的直接后继节点的指针
			q = head;

			while (q->next != NULL && q->next->data.uiLinkId < p->data.uiLinkId)

				q = q->next; //在有序表中查找插入*p的直接前驱节点*q的位置

			p->next = q->next; //将*p插入到*q之后

			q->next = p;

			p = r; //扫描原单链表余下的节点
		}

}



int isSort(pLinkNode_t h) {
	pLinkNode_t p = h->next;
	pLinkNode_t q = p->next;
	while (q != NULL)
	{
		if (p->data.uiLinkId > q->data.uiLinkId)
			return 0;
		p = p->next;
		q = q->next;
	}
	return 1;
}


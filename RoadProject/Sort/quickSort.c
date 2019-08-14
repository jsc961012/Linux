#include <stdio.h>
#include <stdlib.h>
#include "../include/list.h"

//��������
void Partition(pLinkNode_t pBegin, pLinkNode_t pEnd) {
	//if (isSort(pBegin))
	//	printf("�б����ź��򡣡�����������\n");
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

	//��ʼ����
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

	//����֧������1������
	//swap(pBegin->data.uiLinkId, pFirst->data.uiLinkId);
	t = pBegin->data;
	pBegin->data = pFirst->data;
	pFirst->data = t;

	Partition(pBegin, pFirst);
	Partition(pFirst->next, pEnd);//�˴���next����Ҫ,��Ȼ��������ѭ��


	//printf("��������ɹ���������ļ�Ϊ�ļ�sort.txt");
	//writeToTxtFile(msghead, "../sort.txt");
//}
}


//ð������
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

//��������
void InsertSort(pLinkNode_t head) {
	pLinkNode_t p = head->next,  q = NULL,  r = NULL; //pָ���һ�����ݽڵ�

	if (p != NULL) //��������һ���������ϵ����ݽڵ�
	
		r = p->next; //r ���� *p�ڵ��ֱ�Ӻ�̽ڵ��ָ��
		p->next = NULL; //����ֻ����һ�����ݽڵ�������
		p = r;
		while (p != NULL)
		{
			r = p->next; //r ����*p�ڵ��ֱ�Ӻ�̽ڵ��ָ��
			q = head;

			while (q->next != NULL && q->next->data.uiLinkId < p->data.uiLinkId)

				q = q->next; //��������в��Ҳ���*p��ֱ��ǰ���ڵ�*q��λ��

			p->next = q->next; //��*p���뵽*q֮��

			q->next = p;

			p = r; //ɨ��ԭ���������µĽڵ�
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

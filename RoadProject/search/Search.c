#include <stdio.h>
#include <stdlib.h>
#include "../include/fsearch.h"
#include "../include/list.h"

int treeSearch(BiTree T, int k, BiTree f, BiTree* p)
{
	if (!T)
	{
		*p = f;
		printf("未找到数据。。。。。。。。。。。。。");
		return false;
	}
	if (k == T->data.uiLinkId)
	{
		*p = T;
		printf("tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
			T->data.usSize, T->data.uiLinkId,T->data.usNameSize, T->data.uiClass, T->data.uiBranch, T->data.uiFlag,T->data.acName);
		return true;
	}
	else if (k < T->data.uiLinkId)
	{
		return treeSearch(T->lchild, k, T, p);
	}
	else if (k > T->data.uiLinkId)
		return treeSearch(T->rchild, k, T, p);
}

int findByClass(pLinkNode_t h, int class) {
	pLinkNode_t p = h->next;
	pLinkNode_t f = createLink();
	data_t msg;
	int count = 0;
	while (p != NULL) {
		msg = p->data;
		if (msg.uiClass == class) {
			insertLink(f, 0, msg);
			count++;
		}
		p = p->next;
	}
	if (count == 0) {
		printf("未查找到数据。。。\n");
	}
	else if (count <= 5) {
		pLinkNode_t f1 = f->next;
		while (f1 != NULL) {
			msg = f1->data;
			printf("size:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				msg.usSize, msg.uiLinkId, msg.usNameSize, msg.uiClass, msg.uiBranch, msg.uiFlag, msg.acName);
			f1 = f1->next;
		}
	}
	else {
		writeToTxtFile(f, "./File/findMess.txt");
		printf("数据超过五条，已放入./findMess.txt文件中\n");
	}
	return 0;
}


data_t Search_Bin(pLinkNode_t head, int key)
{
	data_t data;
	data.uiLinkId = -1;
	int length = head->count;
	int low = 1, high = length, mid = 0;
	while (low <= high) {
		mid = (low + high) / 2;
		if (key == GetElem(head, mid))
			return GetPoint(head, mid);
		else if (key < GetElem(head, mid))
			high = mid - 1;
		else low = mid + 1;
	}
	return data;
}

int GetElem(pLinkNode_t head, int i) {
	int j = 0;
	pLinkNode_t p;
	p = head;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	return p->data.uiLinkId;
}

//取得链表第i个元素的指针
data_t GetPoint(pLinkNode_t head, int i) {
	int j = 0;
	pLinkNode_t p;
	p = head;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	return p->data;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../include/Macro.h"
#include "../include/roadInfo.h"
#include "../include/list.h"
#include "../include/fRead.h"

void InOrder(BiTree T,FILE* fw);

pLinkNode_t inputFile() {
	unsigned int uiRoadInfo = 0;
	RoadMsg_t msg;
	int i = 0;

	// 创建链表 
	pLinkNode_t h = createLink();

	// 1. 打开文件 
	FILE* fr = fopen("./File/GTBL.dat", "rb");
	if (NULL == fr)
	{
		perror("open fail:");
		exit(-1);
	}
	// 2. 操作文件
	//while(fread(&usSize, sizeof(usSize), 1, fr) == 1)  // feof
	while (feof(fr) == 0)
	{
		fread(&msg.usSize, sizeof(msg.usSize), 1, fr) == 1;
		if (feof(fr) != 0)
		{
			break;
		}
		msg.usSize = MC_GET_SHORT(&msg.usSize);
		fread(&msg.uiLinkId, sizeof(msg.uiLinkId), 1, fr);
		msg.uiLinkId = MC_GET_LONG(&msg.uiLinkId);
		fread(&msg.usNameSize, sizeof(msg.usNameSize), 1, fr);
		msg.usNameSize = MC_GET_SHORT(&msg.usNameSize);
		fread(&uiRoadInfo, sizeof(uiRoadInfo), 1, fr);
		uiRoadInfo = MC_GET_LONG(&uiRoadInfo);
		// 提取道路信息
		msg.uiClass = uiRoadInfo & 0xf;  // 番号
		msg.uiBranch = (uiRoadInfo >> 4) & 0x7;  // 岔路数
		msg.uiFlag = (uiRoadInfo >> 7) & 1;  // 有无道路名称标志

		if (msg.uiFlag != 0)
		{
			fread(msg.acName, msg.usSize - 12, 1, fr);
		}
		else
		{
			strcpy(msg.acName, "No Name!!!");
		}


		// 插入链表
		insertLink(h, 0, msg);
	}
	printf("readover...\n");
	fclose(fr);

	writeToTxtFile(h, "./File/result2.txt");

	printf("write over..\n");
	return h;
}

BiTree inputFilewithTree(pLinkNode_t head) {
	pLinkNode_t h = head->next;

	//创建二叉排序树
	BiTree T = (BiTree)malloc(sizeof(BiTNode));
	T->data = h->data;
	T->lchild = NULL;
	T->rchild = NULL;

	printf("Tree read begin...................\n");

	while (h->next)
	{
		h = h->next;
		//插入二叉排序树
		Insert(&T, h);
		
	}
	FILE* fw = fopen("./File/Treesort.txt", "w");
	InOrder(T,fw);
	fclose(fw);
	printf("Tree read end...................\n");
	return T;
}


BiTree constr(pLinkNode_t head,pLinkNode_t tail)
{
	if (head == tail) return NULL;
	pLinkNode_t mid = head, temp = head;
	while (temp != tail && temp->next != tail)
	{
		temp = temp->next->next;
		mid = mid->next;
	}
	BiTree T = (BiTree)malloc(sizeof(BiTNode));
	T->data = mid->data;
	T->lchild = constr(head, mid);
	T->rchild = constr(mid->next, tail);
	return T;
}


BiTree sortedListToBST(pLinkNode_t head) {
	pLinkNode_t p = head->next;
	BiTree root = constr(p, NULL);
	return root;
}


void InOrder(BiTree T , FILE* fw)
{
	if (T)
	{
		InOrder(T->lchild,fw);
		//printf("%d ", T->data.uiLinkId);
		if (T->data.uiFlag != 0)
		{
			// 有道路名称
			fprintf(fw, "tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				T->data.usSize, T->data.uiLinkId, T->data.usNameSize, T->data.uiClass, T->data.uiBranch, T->data.uiFlag, T->data.acName);  // UTF-8
		}
		else
		{
			fprintf(fw, "tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				T->data.usSize, T->data.uiLinkId, T->data.usNameSize, T->data.uiClass, T->data.uiBranch, T->data.uiFlag, "No Name!!!");  // UTF-8
		}

		InOrder(T->rchild,fw);
	}
}

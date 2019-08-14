#pragma once
#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include "roadInfo.h"

#define bool    _Bool
#define true    1
#define false   0

typedef RoadMsg_t data_t;

// �ڵ�
typedef struct tagnode
{
	// ������
	data_t data;
	int count;
	// ָ����
	struct tagnode* next;
}linkNode_t, * pLinkNode_t;

typedef struct BiTNode
{
	data_t data;
	struct BiTNode* lchild, * rchild;
}BiTNode, * BiTree;//���ṩһ���������Ľṹ


// �����ⲿ����
pLinkNode_t inputFile();		//��ȡ�������ļ�
void replaceFile(pLinkNode_t h);
pLinkNode_t createLink();
//void display(pLinkNode_t h);
int insertLink(pLinkNode_t h, int iPos, data_t x);
void writeToTxtFile(pLinkNode_t h, char* pathName);
int isSort(pLinkNode_t h);
void InsertSort(pLinkNode_t head);

int Insert(BiTree *T, pLinkNode_t i);
BiTree inputFilewithTree(pLinkNode_t h);
BiTree sortedListToBST(pLinkNode_t head);
BiTree constr(pLinkNode_t head, pLinkNode_t tail);

#endif

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

// 节点
typedef struct tagnode
{
	// 数据域
	data_t data;
	int count;
	// 指针域
	struct tagnode* next;
}linkNode_t, * pLinkNode_t;

typedef struct BiTNode
{
	data_t data;
	struct BiTNode* lchild, * rchild;
}BiTNode, * BiTree;//先提供一个二叉树的结构


// 函数外部申明
pLinkNode_t inputFile();		//读取二进制文件
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

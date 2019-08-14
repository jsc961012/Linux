#pragma once
#ifndef FSEARCH_H_
#define FSEARCH_H_
#include "list.h"

int findByClass(pLinkNode_t h, int class);
data_t Search_Bin(pLinkNode_t head, int key);
int GetElem(pLinkNode_t head, int i);
data_t GetPoint(pLinkNode_t head, int i);
int treeSearch(BiTree T, int k, BiTree f, BiTree* p);

#endif // !FSEARCH_H_


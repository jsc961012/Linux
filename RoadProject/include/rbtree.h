#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define MAXSIZE 1000
typedef enum color_t
{
	RED = 0,
	BLACK = 1
}COLOR_T;
typedef struct rbnode
{
	data_t key;
	COLOR_T color;

	struct rbnode* left;
	struct rbnode* right;
	struct rbnode* parent;
}RBNODE, * PRBNODE;

typedef struct RB_tree
{
	PRBNODE root;
	PRBNODE nil;	//统一的空节点
}RBTREE, * PRBTREE;

int leftRotate(PRBTREE ptree, PRBNODE x);
int rightRotate(PRBTREE ptree, PRBNODE y);

PRBTREE CreateRBtree(void);
PRBNODE Insert_RBtree(PRBTREE ptree, data_t a);
int  Delete_RBtree(PRBTREE ptree, PRBNODE z, data_t* dval);
void Traverse_RBtree(PRBTREE ptree);
void RBInsertFixup(PRBTREE ptree, PRBNODE z);
void RBDeleteFixup(PRBTREE ptree, PRBNODE x);

PRBNODE TreeSuccessor(PRBTREE ptree, PRBNODE x);
PRBNODE RBmaximum(PRBTREE ptree, PRBNODE t);
PRBNODE RBminmum(PRBTREE ptree, PRBNODE t);
PRBNODE RBnext(PRBTREE ptree, PRBNODE t);
PRBNODE RBprior(PRBTREE ptree, PRBNODE t);
int midTraverse(PRBTREE ptree, PRBNODE t,FILE* fw);
PRBNODE RBLocate(PRBTREE ptree, int a);
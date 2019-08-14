#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include "../include/Macro.h"
#include "../include/roadInfo.h"
#include "../include/list.h"
#include "../include/fsearch.h"
#include "../include/rbtree.h"


void searchSeen(pLinkNode_t hList , BiTree T , PRBTREE ptree1);
void sortSeen(pLinkNode_t hList, int count);
void InOrder(BiTree T,FILE* fw);
PRBTREE creatrbTree(pLinkNode_t hList);

int main(int argc, const char* argv[])
{
	int choice;
	double dur;
	clock_t start;
	clock_t end;
	pLinkNode_t hList =NULL;	//读入文件后生成的无序链表
	BiTree T =NULL;
	int count = 0;
	int i;
	PRBTREE ptree1 = NULL;
	FILE* fw = fopen("../BBTsort.txt", "w");
	while (1) {
		printf("/************************************************************/\n");
		printf("/*****               电子地图管理系统                   *****/\n");
		printf("/************************************************************/\n");
		printf("请选择服务功能:\n");
		printf("1,导入二进制文件，并以链表存至result2.txt文件\t\n");
		printf("2,排序\t\n");
		printf("3,检索\t\n");
		printf("4,更新\t\n");
		printf("5,生成二叉排序树\t\n");
		printf("6,生成红黑树\t\n");
		printf("7,退出\t\n");
		printf("选择服务功能:");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			hList = inputFile();
			count = hList->count;
			break;
		case 2:
			if (hList)
				sortSeen(hList, count);
			else
				printf("未进行文件读入。。。。。。。\n");
			break;
		case 3:
			if (hList)
				searchSeen(hList, T ,ptree1);
			else
				printf("未进行文件读入。。。。。。。\n");
			break;
		case 4:
			if (hList)
				replaceFile(hList);
			else
				printf("未进行文件读入。。。。。。。\n");
			break;
		case 5:
			if (!hList)
			{
				printf("未进行文件读入。。。。。。。\n");
				continue;
			}
			if (isSort(hList))
			{
				start = clock();
				printf("数据已完成排序，故进行平衡二叉树建立。。。。。。。。。。\n");
				T = sortedListToBST(hList);
				end = clock();
				InOrder(T, fw);
				printf("平衡二叉树生成成功，排序后文件为文件BBTsort.txt\n");
				fclose(fw);
			}
			else
			{
				start = clock();
				T = inputFilewithTree(hList);
				end = clock();
				printf("二叉排序树生成成功，排序后文件为文件Treesort.txt\n");
			}
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("耗时：%f\n", dur);
			break;
		case 6:
			if (!hList)
			{
				printf("未进行文件读入。。。。。。。\n");
				continue;
			}
			start = clock();
			ptree1 = creatrbTree(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("耗时：%f\n", dur);
			break;
		default:
			printf("退出系统。。。。。。");
			return 0;
			break;
		}
	}

}

void sortSeen(pLinkNode_t hList,int count) {
	printf("\n请选择排序方式：\n");
	printf("1.快速排序\n");
	printf("2.冒泡排序\n");
	printf("3.插入排序\n");
	printf("4.返回\n");
	printf("请选择:\n");
	int choice;
	double dur;
	clock_t start;
	clock_t end;
	scanf("%d", &choice);
	switch (choice) {
	case 1:
		if (isSort(hList) == 0){
			start = clock();
			Partition(hList->next, hList + count);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("快速排序成功，排序后文件为文件quicksort.txt\n");
			printf("耗时：%f\n", dur);
			writeToTxtFile(hList, "./File/quicksort.txt");
		}
		else
			printf("列表已排好序。。。。。。。\n");
		break;
	case 2:
		if (isSort(hList) == 0) {
			start = clock();
			hList = BubbleSort(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("冒泡排序成功，排序后文件为文件bubblesort.txt\n");
			printf("耗时：%f\n", dur);
			writeToTxtFile(hList, "./File/bubblesort.txt");
		}
		else
			printf("列表已排好序。。。。。。。\n");
		break;
	case 3:
		if (isSort(hList) == 0) {
			start = clock();
			InsertSort(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("插入排序成功，排序后文件为文件insertsort.txt\n");
			printf("耗时：%f\n", dur);
			writeToTxtFile(hList, "./File/insertsort.txt");
		}
		else
			printf("列表已排好序。。。。。。。\n");
		break;
	case 4:
		return;
		break;
	}
}

void searchSeen(pLinkNode_t hList,BiTree T , PRBTREE ptree1) {
	printf("\n请选择检索方式：\n");
	printf("1.指定 Class  的顺序查找\n");
	printf("2.指定 LinkId 的折半查找\n");
	printf("3.指定 LinkID 二叉排序树查找\n");
	printf("4.指定 LinkID 红黑树查找\n");
	printf("5.返回\n");
	printf("请选择:\n");
	int choice;				//用户选择
	int sLinkId;			//根据id查找，id输入
	int sClass;				//根据番号查找，番号输入
	data_t opinter;			//折半查找返回的数据
	BiTree p;				//二叉树
	PRBNODE prbnode = NULL;		//红黑树查找返回值
	scanf("%d", &choice);
	switch (choice) {
	case 1:
		printf("输入番号class：");
		scanf("%d", &sClass);
		findByClass(hList,sClass);
		break;
	case 2:
		if (isSort(hList) == 0)
			printf("未排序的链表无法进行折半查找，请先进行排序操作。。。。。。。。。。。。\n");
		else
		{
			printf("输入LinkId：");
			scanf("%d", &sLinkId);
			opinter = Search_Bin(hList, sLinkId);
			if (opinter.uiLinkId != -1)
				printf("tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
					opinter.usSize, opinter.uiLinkId, opinter.usNameSize, opinter.uiClass, opinter.uiBranch, opinter.uiFlag, opinter.acName);
			else
				printf("未找到数据。。。。。。。。\n");
		}
		
		break;
	case 3:
		if (T)
		{
			printf("输入LinkId：");
			scanf("%d", &sLinkId);
			treeSearch(T, sLinkId, NULL, &p);
		}
		else
			printf("二叉排序树未生成，请先生成。。。。。。\n");
		break;
	case 4:
		if (ptree1)
		{
			printf("输入LinkId：");
			scanf("%d", &sLinkId);
			prbnode = RBLocate(ptree1, sLinkId);
			if (prbnode == ptree1->nil)
			{
				printf("未找到数据。。。。。。。。\n");
			}
			else
			{
				printf("tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\t color:%d\n",
					prbnode->key.usSize, prbnode->key.uiLinkId, prbnode->key.usNameSize, prbnode->key.uiClass, 
					prbnode->key.uiBranch, prbnode->key.uiFlag, prbnode->key.acName , prbnode->color);
			}
		}
		else
			printf("红黑树未生成，请先生成。。。。。。\n");
		break;
	default:
		return;
		break;
	}
}

PRBTREE creatrbTree(pLinkNode_t hList) {
	FILE* fw = fopen("./File/rbTree.txt", "w");
	PRBTREE ptree1 = CreateRBtree();
	PRBNODE pnode = ptree1->nil;
	pLinkNode_t head = hList->next;
	while (head)
	{
		Insert_RBtree(ptree1, head->data);
		head = head->next;
	}
	midTraverse(ptree1, ptree1->root,fw);
	printf("红黑树生成成功，文件已写入rbTree.txt..............\n");
	fclose(fw);
	return  ptree1;
}


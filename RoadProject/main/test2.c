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
	pLinkNode_t hList =NULL;	//�����ļ������ɵ���������
	BiTree T =NULL;
	int count = 0;
	int i;
	PRBTREE ptree1 = NULL;
	FILE* fw = fopen("../BBTsort.txt", "w");
	while (1) {
		printf("/************************************************************/\n");
		printf("/*****               ���ӵ�ͼ����ϵͳ                   *****/\n");
		printf("/************************************************************/\n");
		printf("��ѡ�������:\n");
		printf("1,����������ļ��������������result2.txt�ļ�\t\n");
		printf("2,����\t\n");
		printf("3,����\t\n");
		printf("4,����\t\n");
		printf("5,���ɶ���������\t\n");
		printf("6,���ɺ����\t\n");
		printf("7,�˳�\t\n");
		printf("ѡ�������:");
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
				printf("δ�����ļ����롣������������\n");
			break;
		case 3:
			if (hList)
				searchSeen(hList, T ,ptree1);
			else
				printf("δ�����ļ����롣������������\n");
			break;
		case 4:
			if (hList)
				replaceFile(hList);
			else
				printf("δ�����ļ����롣������������\n");
			break;
		case 5:
			if (!hList)
			{
				printf("δ�����ļ����롣������������\n");
				continue;
			}
			if (isSort(hList))
			{
				start = clock();
				printf("������������򣬹ʽ���ƽ�������������������������������\n");
				T = sortedListToBST(hList);
				end = clock();
				InOrder(T, fw);
				printf("ƽ����������ɳɹ���������ļ�Ϊ�ļ�BBTsort.txt\n");
				fclose(fw);
			}
			else
			{
				start = clock();
				T = inputFilewithTree(hList);
				end = clock();
				printf("�������������ɳɹ���������ļ�Ϊ�ļ�Treesort.txt\n");
			}
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("��ʱ��%f\n", dur);
			break;
		case 6:
			if (!hList)
			{
				printf("δ�����ļ����롣������������\n");
				continue;
			}
			start = clock();
			ptree1 = creatrbTree(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("��ʱ��%f\n", dur);
			break;
		default:
			printf("�˳�ϵͳ������������");
			return 0;
			break;
		}
	}

}

void sortSeen(pLinkNode_t hList,int count) {
	printf("\n��ѡ������ʽ��\n");
	printf("1.��������\n");
	printf("2.ð������\n");
	printf("3.��������\n");
	printf("4.����\n");
	printf("��ѡ��:\n");
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
			printf("��������ɹ���������ļ�Ϊ�ļ�quicksort.txt\n");
			printf("��ʱ��%f\n", dur);
			writeToTxtFile(hList, "./File/quicksort.txt");
		}
		else
			printf("�б����ź��򡣡�����������\n");
		break;
	case 2:
		if (isSort(hList) == 0) {
			start = clock();
			hList = BubbleSort(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("ð������ɹ���������ļ�Ϊ�ļ�bubblesort.txt\n");
			printf("��ʱ��%f\n", dur);
			writeToTxtFile(hList, "./File/bubblesort.txt");
		}
		else
			printf("�б����ź��򡣡�����������\n");
		break;
	case 3:
		if (isSort(hList) == 0) {
			start = clock();
			InsertSort(hList);
			end = clock();
			dur = (double)(end - start) / CLOCKS_PER_SEC;
			printf("��������ɹ���������ļ�Ϊ�ļ�insertsort.txt\n");
			printf("��ʱ��%f\n", dur);
			writeToTxtFile(hList, "./File/insertsort.txt");
		}
		else
			printf("�б����ź��򡣡�����������\n");
		break;
	case 4:
		return;
		break;
	}
}

void searchSeen(pLinkNode_t hList,BiTree T , PRBTREE ptree1) {
	printf("\n��ѡ�������ʽ��\n");
	printf("1.ָ�� Class  ��˳�����\n");
	printf("2.ָ�� LinkId ���۰����\n");
	printf("3.ָ�� LinkID ��������������\n");
	printf("4.ָ�� LinkID ���������\n");
	printf("5.����\n");
	printf("��ѡ��:\n");
	int choice;				//�û�ѡ��
	int sLinkId;			//����id���ң�id����
	int sClass;				//���ݷ��Ų��ң���������
	data_t opinter;			//�۰���ҷ��ص�����
	BiTree p;				//������
	PRBNODE prbnode = NULL;		//��������ҷ���ֵ
	scanf("%d", &choice);
	switch (choice) {
	case 1:
		printf("���뷬��class��");
		scanf("%d", &sClass);
		findByClass(hList,sClass);
		break;
	case 2:
		if (isSort(hList) == 0)
			printf("δ����������޷������۰���ң����Ƚ����������������������������������\n");
		else
		{
			printf("����LinkId��");
			scanf("%d", &sLinkId);
			opinter = Search_Bin(hList, sLinkId);
			if (opinter.uiLinkId != -1)
				printf("tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
					opinter.usSize, opinter.uiLinkId, opinter.usNameSize, opinter.uiClass, opinter.uiBranch, opinter.uiFlag, opinter.acName);
			else
				printf("δ�ҵ����ݡ���������������\n");
		}
		
		break;
	case 3:
		if (T)
		{
			printf("����LinkId��");
			scanf("%d", &sLinkId);
			treeSearch(T, sLinkId, NULL, &p);
		}
		else
			printf("����������δ���ɣ��������ɡ�����������\n");
		break;
	case 4:
		if (ptree1)
		{
			printf("����LinkId��");
			scanf("%d", &sLinkId);
			prbnode = RBLocate(ptree1, sLinkId);
			if (prbnode == ptree1->nil)
			{
				printf("δ�ҵ����ݡ���������������\n");
			}
			else
			{
				printf("tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\t color:%d\n",
					prbnode->key.usSize, prbnode->key.uiLinkId, prbnode->key.usNameSize, prbnode->key.uiClass, 
					prbnode->key.uiBranch, prbnode->key.uiFlag, prbnode->key.acName , prbnode->color);
			}
		}
		else
			printf("�����δ���ɣ��������ɡ�����������\n");
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
	printf("��������ɳɹ����ļ���д��rbTree.txt..............\n");
	fclose(fw);
	return  ptree1;
}


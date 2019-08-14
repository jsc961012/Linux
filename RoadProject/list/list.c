#include "../include/list.h"

//��������������
int Insert(BiTree *T, pLinkNode_t i)
{
	//printf("%d   %s\n",i->data.uiLinkId,i->data.acName);
	BiTree parent;   //��ʾ˫�׽�㣻
	BiTree head = (*T);
	BiTree p = (BiTree)malloc(sizeof(BiTNode));
	p->data = i->data;
	p->lchild = NULL;
	p->rchild = NULL;
	//������Ҫ���ӵĸ���㣬���������Ƕ�Ϊ0�Ľ�㣻
	while (head)
	{
		parent = head;
		if (i->data.uiLinkId < head->data.uiLinkId)   //���ؼ���С�ڽ������ݣ�
		{
			head = head->lchild; //���������ϲ��ң�
		}
		else   //���ؼ��ִ��ڽ������ݣ�
		{
			head = head->rchild;  //���������ϲ��ң�
		}
	}
	//�ж����ӵ�������������������
	if (i->data.uiLinkId< parent->data.uiLinkId)   //С�ڸ���㣻
	{
		parent->lchild = p;    //���ӵ���������
	}
	else    //���ڸ���㣻
	{
		parent->rchild = p;     //���ӵ���������
	}
	//if (!(*T))
	//{
	//	(*T) = (BiTree)malloc(sizeof(BiTNode));//ΪT����ռ�
	//	(*T)->data = i;//������ֵ
	//	(*T)->lchild = (*T)->rchild = NULL;
	//	return 0;
	//}
	//if (i.uiLinkId == &(*T)->data.uiLinkId)
	//	return 0;
	//else if (i.uiLinkId < &(*T)->data.uiLinkId)
	//	Insert( &(*T)->lchild, i);
	//else
	//	Insert( &(*T)->rchild, i);
	//return 1;
}


// ���������� 
pLinkNode_t createLink()
{
	// ����ռ�
	pLinkNode_t h = (pLinkNode_t)malloc(sizeof(linkNode_t));
	if (NULL == h)
	{
		perror("malloc:");
		return NULL;
	}

	// ��ʼ��Ϊ������
	h->next = NULL;
	return h;
}


// �������ݵ�����(iPos ��0��ʼ��)
// 0: �ɹ�  1: ʧ��
int insertLink(pLinkNode_t h, int iPos, data_t x)
{
	pLinkNode_t p = h;
	int i = 0;
	// �Ҳ�����ǰ��
	while (i < iPos && p == NULL)
	{
		p = p->next;
		i++;
	}

	// �ҵ�ǰ��

	pLinkNode_t q = (pLinkNode_t)malloc(sizeof(linkNode_t));
	if (NULL == q)
	{
		return -1;
	}
	q->data = x;
	q->next = p->next;
	p->next = q;

	return 0;
}

/*
void display(pLinkNode_t h)
{
	pLinkNode_t p = h->next;
	while(p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");

}*/

void writeToTxtFile(pLinkNode_t h, char* pathName)
{
	pLinkNode_t p = h->next;
	data_t msg;
	int i = 0;

	h->count = 0;
	FILE* fw = fopen(pathName, "w");
	if (NULL == fw)
	{
		perror("open fail:");
		exit(-1);
	}

	while (p != NULL)
	{
		msg = p->data;
		if (msg.uiFlag != 0)
		{
			// �е�·����
			fprintf(fw, "%d\tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				++i, msg.usSize, msg.uiLinkId, msg.usNameSize, msg.uiClass, msg.uiBranch, msg.uiFlag, msg.acName);  // UTF-8
			h->count++;
		}
		else
		{
			fprintf(fw, "%d\tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\n",
				++i, msg.usSize, msg.uiLinkId, msg.usNameSize, msg.uiClass, msg.uiBranch, msg.uiFlag, "No Name!!!");  // UTF-8
			h->count++;
		}
		// ��һ��
		p = p->next;
	}
	printf("���ݳ��ȣ�%d\n", h->count);
	fclose(fw);
}


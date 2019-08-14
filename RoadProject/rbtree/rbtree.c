#include "../include/rbtree.h"

PRBTREE CreateRBtree(void)
{
	PRBTREE ptree = (PRBTREE)malloc(sizeof(RBTREE));
	if (NULL == ptree)
	{
		printf("malloc is failure\n");
		return NULL;
	}
	ptree->nil = (PRBNODE)malloc(sizeof(RBNODE));
	if (NULL == ptree->nil)
	{
		printf("malloc is failure\n");
		return NULL;
	}

	ptree->nil->color = BLACK;
	ptree->nil->left = NULL;
	ptree->nil->right = NULL;
	ptree->nil->parent = NULL;

	ptree->root = ptree->nil;

	return ptree;
}

PRBNODE Insert_RBtree(PRBTREE ptree, data_t a)
{
	PRBNODE z = (PRBNODE)malloc(sizeof(RBNODE));
	if (NULL == z)
	{
		printf("malloc is failure\n");
		return;
	}
	z->key = a;
	z->parent = ptree->nil;
	z->left = ptree->nil;
	z->right = ptree->nil;
	z->color = RED;

	PRBNODE x = ptree->root;	//�Ӹ���ʼ���Ҳ����λ��
	PRBNODE y = ptree->nil;	    //��y��¼����ǰɨ��ڵ��˫�׽ڵ㡣yָ��x��parent

	while (x != ptree->nil)
	{
		y = x;			//yʼ��ָ��x��˫��
		if (z->key.uiLinkId < x->key.uiLinkId)
		{
			x = x->left;      //z����x�����
		}
		else
		{
			x = x->right;     //z����x���ұ�
		}
	}	//z��Ϊy�ĺ��Ӳ���
	z->parent = y;		//Z��˫����y

	if (y == ptree->nil)	//z�������һ�ÿ���
	{
		ptree->root = z;    //z�Ǹ�
	}
	else
	{
		if (z->key.uiLinkId < y->key.uiLinkId)
		{
			y->left = z;	//z��Ϊy�����Ӳ���
		}
		else
		{
			y->right = z;	//Z��Ϊy���Һ��Ӳ���
		}
	}
	//	z->left = ptree->nil;	//Z�����Һ��Ӷ�Ϊ��
	//	z->right = ptree->nil;	//z������Ҷ�ӽڵ��λ��
	RBInsertFixup(ptree, z);
	return z;
}

/*
 *����������˼�룺
 *ͨ����ת�͸ı���ɫ�����¶��ϵ�������z�������ݣ�
 *ʹ������������Ҫ�󣬳�Ϊһ����Ч�ĺ����
 *case1~case3��z��˫��p[z]�������游p[p[z]]������
 *case4~case6��z��˫��p[z]�������游p[p[z]]���Һ���
 */
void RBInsertFixup(PRBTREE ptree, PRBNODE z)
{
	PRBNODE y = ptree->nil;
	//��zΪ������p[z]��nil����ɫΪblack��������ѭ��
	//��p[z]Ϊ��ɫ�����z����ͻ����������е���
	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left)//z��˫�������游������
		{
			y = z->parent->parent->right;	//y��z������
			if (y->color == RED)	//case1 z�к���
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;		//z���游��죬���ϴ���
			}
			else    //CASE2 CASE3 ,yΪ��ɫ
			{
				if (z == z->parent->right)  //CASE2  z��˫�׵�����=>CASE3
				{
					z = z->parent;		//������˫��
					leftRotate(ptree, z); //˫������
				}
				//������case3,z��˫�׵�����
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				rightRotate(ptree, z->parent->parent);
			}
		}
		else  //z��˫�������游���Һ���
		{
			y = z->parent->parent->left;	//y��z������
			if (y->color == RED)	//case1 z�к���
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;		//z���游��죬���ϴ���
			}
			else    //CASE2 CASE3 ,yΪ��ɫ
			{
				if (z == z->parent->left)  //CASE2  z��˫�׵�����=>CASE3
				{
					z = z->parent;		//������˫��
					rightRotate(ptree, z); //˫������
				}
				//������case3,z��˫�׵�����
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				leftRotate(ptree, z->parent->parent);
			}
		}
	}
	//��������Ҫ��1.z���������2.case1��ʹ�ú촫���˸���
	ptree->root->color = BLACK;

	return;
}

//�ٶ�right[x] != nil���ڵ���˫�ס�
int leftRotate(PRBTREE ptree, PRBNODE x)
{
	//step1
	PRBNODE y = x->right;

	//step2
	x->right = y->left;
	if (y->left != ptree->nil)
	{
		y->left->parent = x;
	}
	//step3��yȡ��x
	y->parent = x->parent;
	if (x->parent == ptree->nil)
	{
		ptree->root = y;
	}
	else
	{
		if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
	}
	//step4
	y->left = x;
	x->parent = y;

	return 0;
}

//�ٶ�left[y] != nil���ڵ���˫�ס�
int rightRotate(PRBTREE ptree, PRBNODE y)
{
	PRBNODE x = y->left;

	if (y->left == ptree->nil)
	{
		return -1;
	}
	//step1
	y->left = x->right;	//1.

	//step2 Bȡ��y
	if (x->right != ptree->nil)
	{
		x->right->parent = y;
	}
	//step3:xȡ��y
	x->parent = y->parent;
	if (y->parent == ptree->nil)
	{
		ptree->root = x;
	}
	else
	{
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	//step4:xȡ��B
	x->right = y;
	y->parent = x;

	return 0;
}

int  Delete_RBtree(PRBTREE ptree, PRBNODE z, data_t* dval)
{
	PRBNODE x = ptree->nil;  //
	PRBNODE y = ptree->nil;  //

	if (z == ptree->nil)
	{
		return -1;
	}
	//CASE1��CASE2
	if ((z->left == ptree->nil) || (z->right == ptree->nil))
	{
		y = z;     	//����ʵ��ɾ������y������ɾ����
	}
	else           //z�������ӷǿ�
	{
		y = TreeSuccessor(ptree, z); //y��z��������
	}
	//��ʱyͳһ����x��˫�׽ڵ㣬Ҳ��Ҫɾ���Ľڵ�
	//x��Ҫ���ӵ�p[y]�Ľڵ㣬����Ҫȷ��x
	//x����y��Ψһ�ķǿպ���
	if (y->left != ptree->nil)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}
	//��������xȡ��y��y��˫�׶�·if p[y]����

	x->parent = y->parent;

	if (y->parent == ptree->nil)	//y�Ǹ�
	{
		ptree->root = x;   //��ָ��ָ��x
	}
	else
	{
		if (y == y->parent->left)    //y��˫�׵�����
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	//store z's key
	*dval = z->key;
	//copy y�Ĺؼ��ֺ�������z
	if (y != z)  //CASE3
	{
		z->key = y->key;
	}
	if (y->color == BLACK)   //y����ɫΪ��ɫ���������1.���ı�ڸ� 2.���������ں�ɫ��3.�����Ǹ�
	{
		RBDeleteFixup(ptree, x);
	}
	free(y); //ʵ��ɾ������y�ڵ�
	return 0;
}
PRBNODE TreeSuccessor(PRBTREE ptree, PRBNODE x)
{
	return RBminmum(ptree, x->right);
}

void RBDeleteFixup(PRBTREE ptree, PRBNODE x)
{
	PRBNODE w = ptree->nil;	//��������x���ֵ�
	while (x != ptree->root && (x->color == BLACK))	//x�Ǹ���˫�ڣ���ɾ�ڵ�y�ĺ�ɫͿ��x����
	{
		if (x == x->parent->left)	//x��˫�׵����ӣ�case1 ~ CASE4
		{
			w = x->parent->right;	//w��x���ֵ�
			if (w->color == RED)
			{
				w->color = BLACK;	//case1Ŀ�꣺��case1תΪcase2,3,4����
				x->parent->color = RED;  //x���ֵܼ�˫�ױ�ɫ
				leftRotate(ptree, x->parent);
				w = x->parent->right;
			}//��Ϊcase2��case3��case4 �µ��ֵ�wΪ��ɫ

			if ((w->left->color == BLACK) && (w->right->color == BLACK))
			{						//CASE2 x���ֵ�w����Ϊ��
				w->color = RED;		//x��w�ϵĺ�ɫͼ��p[X]�ϡ�
				x = x->parent;	//P[X]Ϊ�µ�x����p[x]ԭΪ�ڣ���˫�ڼ���ѭ����������ֹ
			}
			else      //case3,case4
			{
				if (w->right->color == BLACK)	//CASE3 W�Һ���Ϊ�ڣ����ӱ�Ϊ��
				{
					w->left->color = BLACK;
					w->color = RED;			//w�����ӱ�ɫ
					rightRotate(ptree, w);
					w = x->parent->right; //Wָ���µ��ֵ�
				}	//CASE3������Ϊcase4��w�Һ���Ϊ�죬���ӿɺ�ɺ�

				w->color = x->parent->color;	//P[X]����ɫͿ��w��
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(ptree, x->parent);

				x = ptree->root;	//�˳�ѭ��
			}	//END CASE3��CASE4
		}
		else    //x����˫�׵��Һ��ӣ�case5~case8������Գ�
		{
			w = x->parent->left;	//w��x���ֵ�
			if (w->color == RED)
			{
				w->color = BLACK;	//case1
				x->parent->color = RED;  //x���ֵܼ�˫�ױ�ɫ
				rightRotate(ptree, x->parent);
				w = x->parent->left;
			}//��Ϊcase2��case3��case4 �µ��ֵ�wΪ��ɫ

			if ((w->right->color == BLACK) && (w->left->color == BLACK))
			{						//CASE2 x���ֵ�w����Ϊ��
				w->color = RED;		//x��w�ϵĺ�ɫͼ��p[X]�ϡ�
				x = x->parent;	//P[X]Ϊ�µ�x����p[x]ԭΪ�ڣ���˫�ڼ���ѭ����������ֹ
			}
			else      //case3,case4
			{
				if (w->left->color == BLACK)	//CASE3 W�Һ���Ϊ�ڣ����ӱ�Ϊ��
				{
					w->right->color = BLACK;
					w->color = RED;			//w�����ӱ�ɫ
					leftRotate(ptree, w);
					w = x->parent->left; //Wָ���µ��ֵ�
				}	//CASE3������Ϊcase4��w�Һ���Ϊ�죬���ӿɺ�ɺ�

				w->color = x->parent->color;	//P[X]����ɫͿ��w��
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(ptree, x->parent);

				x = ptree->root;	//�˳�ѭ��
			}	//END CASE3��CASE4
		}
	}
	x->color = BLACK;
	return;
}


//������tΪ�������������ֵ�����t��nil�򷵻�nil
PRBNODE RBmaximum(PRBTREE ptree, PRBNODE t)
{
	/*	if (t == ptree->nil)
		{
			return NULL;
		}*/
	while (t->right != ptree->nil)
	{
		t = t->right;		//��������ֵ�����ģ�����һֱ���Ҳ�ѯ
	}
	return t;
}

//������tΪ������������Сֵ�����tΪnil������nil
PRBNODE RBminmum(PRBTREE ptree, PRBNODE t)
{
	while (t->left != ptree->nil)
	{
		t = t->left;
	}
	return t;
}
/*
 *���ҽڵ�t�ĺ�̽ڵ㣬����ָ��ýڵ��ָ�룬���û�к���򷵻�nil
 *�ڲ��ҹ����У����t����������Ϊ�գ���ô��������������С�ڵ㼴��
 *���t��������Ϊ�գ�t�ĺ�̽ڵ�Ϊĳһ���ȵĸ��ڵ㣬
 *���Ҹ����Ƚڵ�����Ϊ�丸�׽ڵ�����ӡ�
 * */
 //���ҽڵ�t�ĺ�̽ڵ㣬����ָ��ýڵ��ָ�룬���û�к���򷵻�nil
PRBNODE RBnext(PRBTREE ptree, PRBNODE t)
{
	PRBNODE p = ptree->nil;

	if (t->right == ptree->nil)	//�Һ���Ϊ��ʱ
	{
		p = t->parent;		//until p->left == t;
		while (p != ptree->nil && p->right == t)//��̽ڵ�Ϊp->left == tʱ��p
		{
			t = p;
			p = t->parent;
		}
	}
	else
	{
		p = t->right;
		while (p->left != ptree->nil)
		{
			p = p->left;
		}
	}
	return p;
}

/*
 *���ҽڵ�t��ǰ���ڵ㣬����ָ��ýڵ��ָ�룬���ûǰ���򷵻�nil
 *�ڲ��ҹ����У����t����������Ϊ�գ���ô���������������ڵ㼴��
 *���t��������Ϊ�գ�t��ǰ���ڵ�Ϊĳһ���ȵĸ��ڵ㣬
 *���Ҹ����Ƚڵ�����Ϊ�丸�׽ڵ���Һ��ӡ�
 * */
 //���ҽڵ�t��ǰ���ڵ�
 //
PRBNODE RBprior(PRBTREE ptree, PRBNODE t)
{
	PRBNODE p = ptree->nil;
	if (t->left != ptree->nil)
	{
		p = t->parent;
		while (p != ptree->nil && p->left == t)
		{
			t = p;
			p = t->parent;
		}
	}
	else
	{
		p = t->left;
		while (p->right != ptree->nil)
		{
			p = p->right;
		}
	}
	return p;
}

// �������,��tΪ������������
int midTraverse(PRBTREE ptree, PRBNODE t,FILE* fw)
{
	if (t == ptree->nil)
	{
		return 0;
	}
	//	putchar('(');
	midTraverse(ptree, t->left,fw);
	//	putchar(')');

	//printf("%d", t->key.uiLinkId);
	fprintf(fw, "tsize:%d\t linkid:%d\t nameSize:%d\t class:%d\t branch:%d\t flag:%d\t name:%s\t ",
		t->key.usSize, t->key.uiLinkId, t->key.usNameSize, t->key.uiClass, t->key.uiBranch, t->key.uiFlag, t->key.acName);  // UTF-8
	if (t->color == BLACK)
	{
		fprintf(fw,"color:B\n");
	}
	else
	{
		fprintf(fw, "color:R\n");
	}
	//	putchar('(');
	midTraverse(ptree, t->right,fw);
	//	putchar(')');
}

PRBNODE RBLocate(PRBTREE ptree, int a)
{
	PRBNODE p = ptree->root;
	while (p != ptree->nil)
	{
		if (a > p->key.uiLinkId)
		{
			p = p->right;
		}
		else if (a < p->key.uiLinkId)
		{
			p = p->left;
		}
		else
		{
			break;
		}
	}
	return p;
}

/////////////////////////////////////////////////////////////////////////////



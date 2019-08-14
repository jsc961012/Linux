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

	PRBNODE x = ptree->root;	//从根开始查找插入的位置
	PRBNODE y = ptree->nil;	    //用y记录：当前扫描节点的双亲节点。y指向x的parent

	while (x != ptree->nil)
	{
		y = x;			//y始终指向x的双亲
		if (z->key.uiLinkId < x->key.uiLinkId)
		{
			x = x->left;      //z插入x的左边
		}
		else
		{
			x = x->right;     //z插入x的右边
		}
	}	//z作为y的孩子插入
	z->parent = y;		//Z的双亲是y

	if (y == ptree->nil)	//z插入的是一棵空树
	{
		ptree->root = z;    //z是根
	}
	else
	{
		if (z->key.uiLinkId < y->key.uiLinkId)
		{
			y->left = z;	//z作为y的左孩子插入
		}
		else
		{
			y->right = z;	//Z作为y的右孩子插入
		}
	}
	//	z->left = ptree->nil;	//Z的左右孩子都为空
	//	z->right = ptree->nil;	//z插入在叶子节点的位置
	RBInsertFixup(ptree, z);
	return z;
}

/*
 *调整的整体思想：
 *通过旋转和改变颜色，自下而上调整。（z进行上溯）
 *使树满足红黑树的要求，成为一棵有效的红黑树
 *case1~case3：z的双亲p[z]是它的祖父p[p[z]]的左孩子
 *case4~case6：z的双亲p[z]是它的祖父p[p[z]]的右孩子
 */
void RBInsertFixup(PRBTREE ptree, PRBNODE z)
{
	PRBNODE y = ptree->nil;
	//若z为根，则p[z]是nil，颜色为black，不进入循环
	//若p[z]为黑色，与红z不冲突，亦无须进行调整
	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left)//z的双亲是其祖父的左孩子
		{
			y = z->parent->parent->right;	//y是z的叔叔
			if (y->color == RED)	//case1 z有红叔
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;		//z的祖父变红，往上传播
			}
			else    //CASE2 CASE3 ,y为黑色
			{
				if (z == z->parent->right)  //CASE2  z是双亲的右子=>CASE3
				{
					z = z->parent;		//上溯至双亲
					leftRotate(ptree, z); //双亲左旋
				}
				//以下是case3,z是双亲的左孩子
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				rightRotate(ptree, z->parent->parent);
			}
		}
		else  //z的双亲是其祖父的右孩子
		{
			y = z->parent->parent->left;	//y是z的叔叔
			if (y->color == RED)	//case1 z有红叔
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;		//z的祖父变红，往上传播
			}
			else    //CASE2 CASE3 ,y为黑色
			{
				if (z == z->parent->left)  //CASE2  z是双亲的右子=>CASE3
				{
					z = z->parent;		//上溯至双亲
					rightRotate(ptree, z); //双亲左旋
				}
				//以下是case3,z是双亲的左孩子
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				leftRotate(ptree, z->parent->parent);
			}
		}
	}
	//有两处需要：1.z插入空树，2.case1，使得红传到了根。
	ptree->root->color = BLACK;

	return;
}

//假定right[x] != nil，节点有双亲。
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
	//step3：y取代x
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

//假定left[y] != nil，节点有双亲。
int rightRotate(PRBTREE ptree, PRBNODE y)
{
	PRBNODE x = y->left;

	if (y->left == ptree->nil)
	{
		return -1;
	}
	//step1
	y->left = x->right;	//1.

	//step2 B取代y
	if (x->right != ptree->nil)
	{
		x->right->parent = y;
	}
	//step3:x取代y
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
	//step4:x取代B
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
	//CASE1，CASE2
	if ((z->left == ptree->nil) || (z->right == ptree->nil))
	{
		y = z;     	//后面实际删除的是y（物理删除）
	}
	else           //z的两孩子非空
	{
		y = TreeSuccessor(ptree, z); //y是z的中序后继
	}
	//此时y统一的是x的双亲节点，也是要删除的节点
	//x是要连接到p[y]的节点，以下要确定x
	//x保存y的唯一的非空孩子
	if (y->left != ptree->nil)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}
	//以下是用x取代y与y的双亲短路if p[y]存在

	x->parent = y->parent;

	if (y->parent == ptree->nil)	//y是根
	{
		ptree->root = x;   //根指针指向x
	}
	else
	{
		if (y == y->parent->left)    //y是双亲的左孩子
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
	//copy y的关键字和数据域到z
	if (y != z)  //CASE3
	{
		z->key = y->key;
	}
	if (y->color == BLACK)   //y的颜色为红色无须调整，1.不改变黑高 2.不会有相邻红色。3.不会是根
	{
		RBDeleteFixup(ptree, x);
	}
	free(y); //实际删除的是y节点
	return 0;
}
PRBNODE TreeSuccessor(PRBTREE ptree, PRBNODE x)
{
	return RBminmum(ptree, x->right);
}

void RBDeleteFixup(PRBTREE ptree, PRBNODE x)
{
	PRBNODE w = ptree->nil;	//用来保存x的兄弟
	while (x != ptree->root && (x->color == BLACK))	//x非根，双黑，被删节点y的黑色涂到x上了
	{
		if (x == x->parent->left)	//x是双亲的左孩子，case1 ~ CASE4
		{
			w = x->parent->right;	//w是x的兄弟
			if (w->color == RED)
			{
				w->color = BLACK;	//case1目标：将case1转为case2,3,4处理
				x->parent->color = RED;  //x的兄弟及双亲变色
				leftRotate(ptree, x->parent);
				w = x->parent->right;
			}//变为case2，case3，case4 新的兄弟w为黑色

			if ((w->left->color == BLACK) && (w->right->color == BLACK))
			{						//CASE2 x的兄弟w两子为黑
				w->color = RED;		//x及w上的黑色图到p[X]上。
				x = x->parent;	//P[X]为新的x，若p[x]原为黑，则双黑继续循环，否则终止
			}
			else      //case3,case4
			{
				if (w->right->color == BLACK)	//CASE3 W右孩子为黑，左孩子必为红
				{
					w->left->color = BLACK;
					w->color = RED;			//w及左孩子变色
					rightRotate(ptree, w);
					w = x->parent->right; //W指向新的兄弟
				}	//CASE3处理后变为case4，w右孩子为红，左孩子可红可黑

				w->color = x->parent->color;	//P[X]的颜色涂到w上
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(ptree, x->parent);

				x = ptree->root;	//退出循环
			}	//END CASE3，CASE4
		}
		else    //x是其双亲的右孩子，case5~case8与上面对称
		{
			w = x->parent->left;	//w是x的兄弟
			if (w->color == RED)
			{
				w->color = BLACK;	//case1
				x->parent->color = RED;  //x的兄弟及双亲变色
				rightRotate(ptree, x->parent);
				w = x->parent->left;
			}//变为case2，case3，case4 新的兄弟w为黑色

			if ((w->right->color == BLACK) && (w->left->color == BLACK))
			{						//CASE2 x的兄弟w两子为黑
				w->color = RED;		//x及w上的黑色图到p[X]上。
				x = x->parent;	//P[X]为新的x，若p[x]原为黑，则双黑继续循环，否则终止
			}
			else      //case3,case4
			{
				if (w->left->color == BLACK)	//CASE3 W右孩子为黑，左孩子必为红
				{
					w->right->color = BLACK;
					w->color = RED;			//w及左孩子变色
					leftRotate(ptree, w);
					w = x->parent->left; //W指向新的兄弟
				}	//CASE3处理后变为case4，w右孩子为红，左孩子可红可黑

				w->color = x->parent->color;	//P[X]的颜色涂到w上
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(ptree, x->parent);

				x = ptree->root;	//退出循环
			}	//END CASE3，CASE4
		}
	}
	x->color = BLACK;
	return;
}


//返回以t为根的子树的最大值，如果t是nil则返回nil
PRBNODE RBmaximum(PRBTREE ptree, PRBNODE t)
{
	/*	if (t == ptree->nil)
		{
			return NULL;
		}*/
	while (t->right != ptree->nil)
	{
		t = t->right;		//右子树的值是最大的，所以一直往右查询
	}
	return t;
}

//返回以t为根的子树的最小值，如果t为nil怎返回nil
PRBNODE RBminmum(PRBTREE ptree, PRBNODE t)
{
	while (t->left != ptree->nil)
	{
		t = t->left;
	}
	return t;
}
/*
 *查找节点t的后继节点，返回指向该节点的指针，如果没有后继则返回nil
 *在查找过程中，如果t的右子树不为空，那么返回右子树的最小节点即可
 *如果t的右子树为空，t的后继节点为某一祖先的父节点，
 *而且该祖先节点是作为其父亲节点的左孩子。
 * */
 //查找节点t的后继节点，返回指向该节点的指针，如果没有后继则返回nil
PRBNODE RBnext(PRBTREE ptree, PRBNODE t)
{
	PRBNODE p = ptree->nil;

	if (t->right == ptree->nil)	//右孩子为空时
	{
		p = t->parent;		//until p->left == t;
		while (p != ptree->nil && p->right == t)//后继节点为p->left == t时的p
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
 *查找节点t的前驱节点，返回指向该节点的指针，如果没前驱则返回nil
 *在查找过程中，如果t的左子树不为空，那么返回左子树的最大节点即可
 *如果t的右子树为空，t的前驱节点为某一祖先的父节点，
 *而且该祖先节点是作为其父亲节点的右孩子。
 * */
 //查找节点t的前驱节点
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

// 中序遍历,以t为根的子树遍历
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



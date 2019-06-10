#include<cstdio>
#include<iostream>
#include"RBTree.h"
using namespace std;


/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */
template<class T>
void RBTree<T>::leftRotate(RBTNode<T>* &root, RBTNode<T>* x)//这个引用是必须的，删了会报错！
{
	RBTNode<T>* y = x->right;
	x->right = y->left;
	if (y->left != nullptr)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == nullptr)
		root = y;
	else
	{
		if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行右旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */
template<class T>
void RBTree<T>::rightRotate(RBTNode<T>* &root, RBTNode<T>* y)
{
	RBTNode<T>* x = y->left;
	y->left = x->right;
	if (x->right != nullptr)
		x->right->parent = y;
	x->parent = y->parent;
	if (y->parent == nullptr)
		root = x;
	else
	{
		if (y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}

template<class T>
void RBTree<T>::insert(RBTNode<T>* &root, RBTNode<T>* node)
{
	RBTNode<T>* y = nullptr;
	RBTNode<T>* x = root;

	while (x != nullptr)
	{
		y = x;
		if (node->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y != nullptr)
	{
		if (node->key < y->key)
			y->left = node;
		else
			y->right = node;
	}
	else
		root = node;

	node->color = RED;
	
	insertFixUp(root, node);
}

template<class T>
void RBTree<T>::insertFixUp(RBTNode<T>* &root, RBTNode<T>* node)
{
	RBTNode<T> *parent;
	RBTNode<T> *gparent;

	while ((parent = rb_parent(node)) && rb_is_red(parent))//while循环的条件是父节点为红。由于红黑树的根必为黑，所以父不可能是根，因而其上必仍存在点即祖父点
	{
		gparent = rb_parent(parent);

		if (parent == gparent->left)
		{
			RBTNode<T> *uncle = gparent->right;

			//Case1条件：叔叔节点是红色
			if (uncle && rb_is_red(uncle))//在红黑树中，叶子节点后面的null，也算是黑色节点，因此即使uncle是nullptr，也可以是黑色的节点
			{
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(gparent);
				node = gparent;
				continue;
			}

			//Case2条件：叔叔是黑色，且当前节点是右孩子
			if ((!uncle || rb_is_black(uncle)) && (parent->right == node))
			{
				RBTNode<T> *tmp;
				leftRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
				continue;
			}

			//Case3条件：叔叔是黑色，且当前节点是左孩子
			if ((!uncle || rb_is_black(uncle)) && (parent->left == node))
			{
				rb_set_black(parent);
				rb_set_red(gparent);
				rightRotate(root, gparent);
				continue;
			}
		}

		else//父节点是祖父节点的右孩子  //很多对称反向操作
		{
			RBTNode<T>* uncle = gparent->left;
			//Case1条件：树树节点是红色
			if (uncle && rb_is_red(uncle))
			{
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(gparent);
				node = gparent;
				continue;
			}

			//Case2条件：叔叔是黑色，且当前节点是左孩子
			if ((!uncle || rb_is_black(uncle)) && (parent->left == node))//反向操作
			{
				RBTNode<T>* tmp;
				rightRotate(root, parent);//反向操作
				tmp = parent;
				parent = node;
				node = tmp;
				continue;
			}

			//Case3条件：叔叔是黑色，且当前节点是右孩子
			if ((!uncle || rb_is_black(uncle)) && (parent->right == node))//反向操作
			{
				rb_set_black(parent);
				rb_set_red(gparent);
				leftRotate(root, gparent);//反向操作
				continue;
			}
		}
	}

	rb_set_black(root);//被插入的节点是根节点则直接涂黑；被插入的节点的父节点是黑色则什么也不用管即可
}


template<class T>
void RBTree<T>::remove(RBTNode<T>* &root, RBTNode<T>* node)
{
	RBTNode<T>* child, *parent;
	RBTColor color;

	if ((node->left != nullptr) && (node->right != nullptr))// 被删除节点的"左右孩子都不为空"的情况。
	{
		RBTNode<T>* replace = node;

		replace = replace->right;
		while (replace->left != nullptr)
			replace = replace->left;

		if (rb_parent(node))
		{
			if (rb_parent(node)->left == node)
				rb_parent(node)->left = replace;
			else
				rb_parent(node)->right = replace;
		}
		else
			root = replace;

		child = replace->right;//这里的child是replace的后一个节点
		parent = rb_parent(replace);
		color = rb_color(replace);

		if (parent == node)
			parent = replace;//parent最终存储的是child的父节点
		else
		{
			if (child)
				rb_set_parent(child, parent);
			parent->left = child;

			replace->right = node->right;
			rb_set_parent(node->right, replace);
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if (color == BLACK)//正如下行注释所述，“消失”的是replace节点，color存储的是“消失”的节点的颜色
			removeFixUp(root, child, parent);//通过RB - DELETE算法，我们知道：删除节点y之后，x占据了原来节点y的位置。在这里node节点被replace替换了，实际“消失”的是replace节点，child节点占据了原replace节点的老位置

		delete node;
		return;
	}

	if (node->left != nullptr)
		child = node->left;//这里的child是node的后一个节点
	else
		child = node->right;

	parent = node->parent;
	color = node->color;//color存储的是“消失”的节点的颜色

	if (child)
		child->parent = parent;

	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root = child;

	if (color == BLACK)//如果删除的是红色节点，那么原红黑树的性质依旧保持，此时不用做修正操作
		removeFixUp(root, child, parent); //通过RB - DELETE算法，我们知道：删除节点y之后，x占据了原来节点y的位置。在这里，“消失”的就是node，child节点占据了node节点的老位置

	delete node;
}

template<class T>
void RBTree<T>::removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent)//只有“消失”黑色节点的时候才会调用该函数，即“消失的黑色”即为攻略所指的“额外一层的黑色”
{
	RBTNode<T>* other;

	while ((!node || rb_is_black(node)) && node != root)
	{
		if (parent->left == node)
		{
			other = parent->right;

			//Case1：x的兄弟w是红色的
			if (other && rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				leftRotate(root, parent);
				other = parent->right;
			}
			//Case2：x的兄弟w是黑色的，且w的两个孩子也都是黑色的
			if ((!other || rb_is_black(other)) && (!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right)))
			{
				rb_set_red(other);
				node = parent;//node和下面的parent都往上提了一级，故两者的相对父子位置关系仍保持不变
				parent = rb_parent(node);
				continue;
			}
			//Case3：x的兄弟w是黑色的，并且w的左孩子是红色，右孩子是黑色
			if ((!other || rb_is_black(other)) && (other->left && rb_is_red(other->left)) && (!other->right || rb_is_black(other->right)))
			{
				rb_set_black(other->left);
				rb_set_red(other);
				rightRotate(root, other);
				other = parent->right;
				continue;
			}
			//Case4：x的兄弟w是黑色的，并且w的右孩子是红色的，左孩子任意
			if ((!other || rb_is_black(other)) && (other->right && rb_is_red(other->right)))
			{
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->right);
				leftRotate(root, parent);
				node = root;
				break;//另一个攻略说，到这里，算法应该结束了正常
			}
			
		}
		else//很多对称反向操作
		{
			other = parent->left;//反向操作
			//Case1：x的兄弟w是红色的
			if (other && rb_is_red(other))
			{
				rb_set_black(other);
				rb_set_red(parent);
				rightRotate(root, parent);//反向操作
				other = parent->left;//反向操作
				continue;
			}
			//Case2：x的兄弟w是黑色，且w的两个孩子也都是黑色的
			if ((!other || rb_is_black(other)) && (!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
				continue;
			}
			//Case3：x的兄弟w是黑色的，并且w的左孩子是红色的，右孩子为黑色的
			if ((!other || rb_is_black(other)) && (other->left && rb_is_red(other->left)) && (!other->right || rb_is_black(other->right)))
			{
				rb_set_black(other->right);//反向操作
				rb_set_red(other);
				leftRotate(root, other);//反向操作
				other = parent->left;//反向操作
				continue;
			}
			//Case4：x的兄弟w是黑色的，并且w的右孩子是红色的，左孩子任意颜色
			if ((!other || rb_is_black(other)) && (other->right && rb_is_red(other->right)))
			{
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->left);//反向操作
				rightRotate(root, parent);//反向操作
				node = root;
				break;
			}
			
		}
	}

	if (node)//如果node是红色，直接把node涂成黑色即可；如果node是黑色且是跟，无需任何操作即可
		rb_set_black(node);
}

template<class T>
RBTree<T>::RBTree():mRoot(nullptr){}

template<class T>
RBTree<T>::~RBTree()
{
	destroy();
}

template<class T>
void RBTree<T>::preOrder(RBTNode<T>* tree)const
{
	if (tree != nullptr)
	{
		cout << tree->key << " ";
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template<class T>
void RBTree<T>::inOrder(RBTNode<T>* tree)const
{
	if (tree != nullptr)
	{
		inOrder(tree->left);
		cout << tree->key << " ";
		inOrder(tree->right);
	}
}

template<class T>
void RBTree<T>::postOrder(RBTNode<T>* tree)const
{
	if (tree != nullptr)
	{
		postOrder(tree->left);
		postOrder(tree->right);
		cout << tree->key << " ";
	}
}

/*
* (递归实现)查找"红黑树x"中键值为key的节点
*/
template<class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* x, T key)const
{
	if (x == nullptr || x->key == key)
		return x;
	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}

/*
* (非递归实现)查找"红黑树x"中键值为key的节点
*/
template<class T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x, T key)const
{
	while ((x != nullptr) && (x->key != key))
	{
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

/*
* 查找最小结点：返回tree为根结点的红黑树的最小结点。
*/
template<class T>
RBTNode<T>* RBTree<T>::minimum(RBTNode<T>* tree)
{
	if (tree == nullptr)
		return nullptr;
	while (tree->left != nullptr)
		tree = tree->left;
	return tree;
}

/*
* 查找最大结点：返回tree为根结点的红黑树的最大结点。
*/
template<class T>
RBTNode<T>* RBTree<T>::maximum(RBTNode<T>* tree)
{
	if (tree == nullptr)
		return nullptr;
	while (tree->right != nullptr)
		tree = tree->right;
	return tree;
}

/*
* 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
*/
template<class T>
RBTNode<T>* RBTree<T>::successor(RBTNode<T>* x)
{
	if (x->right != nullptr)
		return minimum(x->right);
	// 如果x没有右孩子。则x有以下两种可能：
	// (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
	// (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
	RBTNode<T>* y = x->parent;
	while ((y != nullptr) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}
	return y;
}

/*
* 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
*/
template<class T>
RBTNode<T>* RBTree<T>::predecessor(RBTNode<T>* x)
{
	if (x->left != nullptr)
		return maximum(x->left);

	// 如果x没有左孩子。则x有以下两种可能：
	// (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
	// (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
	RBTNode<T>* y = x->parent;
	while ((y != nullptr) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}
	return y;
}

template<class T>
void RBTree<T>::destroy(RBTNode<T>* &tree)
{
	if (tree == nullptr)
		return;
	if (tree->left != nullptr)
		destroy(tree->left);//我不放return，没道理放！！
	if (tree->right != nullptr)
		destroy(tree->right);

	delete tree;
	tree = nullptr;
}

/*
* 打印"二叉查找树"
*
* key        -- 节点的键值
* direction  --  0，表示该节点是根节点;
*               -1，表示该节点是它的父结点的左孩子;
*                1，表示该节点是它的父结点的右孩子。
*/
template<class T>
void RBTree<T>::print(RBTNode<T>* tree, T key, int direction)
{
	if (tree != nullptr)
	{
		if (direction == 0)
			cout << tree->key << "(B) is root" << endl;
		else
			cout << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << "is" << key << "'s" << (direction == 1 ? "right child" : "left child") << endl;

		print(tree->left, tree->key, -1);
		print(tree->right, tree->key, 1);
	}
}


//测试程序
int main()
{
	int a[] = { 10,40,30,60,90,70,20,50,80 };
	int len = sizeof(a) / sizeof(a[0]);//比sizeof(int)要好得多
	RBTree<int>* tree = new RBTree<int>();//tree类只是一堆函数的集合，真正的树节点类是RBTNode

	cout << "==原始数据：";
	for (int i = 0; i < len; ++i)
		cout << a[i] << " ";
	cout << endl;

	for (int i = 0; i < len; ++i)
	{
		RBTNode<int>* z=new RBTNode<int>(a[i],RED,nullptr,nullptr,nullptr);//我想写RED
		tree->insert(tree->mRoot, z);
		cout << "==添加节点：" << a[i] << endl;
		cout << "==树的详细信息：" << endl;
		tree->print(tree->mRoot,tree->mRoot->key,0);
		cout << endl;
	}
	
	cout << "==前序遍历：";
	tree->preOrder(tree->mRoot);

	cout << "\n==中序遍历：";
	tree->inOrder(tree->mRoot);

	cout << "\n==后序遍历：";
	tree->postOrder(tree->mRoot);
	cout << endl;

	cout << "==最小值：" << tree->minimum(tree->mRoot)->key << endl;
	cout << "==最大值：" << tree->maximum(tree->mRoot)->key << endl;
	cout << "==树的详细信息：" << endl;
	tree->print(tree->mRoot, tree->mRoot->key, 0);

	for (int i = 0; i < len; ++i)
	{
		//RBTNode<int>* z = new RBTNode<int>(a[i], RED, nullptr, nullptr, nullptr);//新建的点，左右父都是nullptr，根本不是现有树中的节点！
		RBTNode<int>* node;
		if ((node = tree->search(tree->mRoot, a[i])) != nullptr)
		{
			tree->remove(tree->mRoot, node);
			cout << "==删除节点：" << a[i] << endl;
			cout << "==树的详细信息：" << endl;
			if(tree->mRoot!=nullptr)
				tree->print(tree->mRoot, tree->mRoot->key, 0);
			cout << endl;
		}
	}

	tree->destroy(tree->mRoot);

	return 0;
}
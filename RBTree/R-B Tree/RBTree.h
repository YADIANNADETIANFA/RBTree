#pragma once
enum RBTColor{RED,BLACK};

template<class T>
class RBTNode
{
public:
	RBTColor color;
	T key;
	RBTNode *left;
	RBTNode *right;
	RBTNode *parent;

	RBTNode(T value,RBTColor c,RBTNode *p,RBTNode *l, RBTNode *r):key(value),color(c),parent(p),left(l),right(r){}
};

template<class T>
class RBTree
{

public:
	RBTNode<T>* mRoot;//根节点
	RBTree();
	~RBTree();

	void preOrder(RBTNode<T>* tree) const;//前序遍历红黑树
	void inOrder(RBTNode<T>* tree) const;//中序遍历红黑树
	void postOrder(RBTNode<T>* tree) const;//后序遍历红黑树

	RBTNode<T>* search(RBTNode<T>* x, T key) const;//递归实现查找红黑树中键值为key的节点
	RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;//迭代实现查找红黑树中键值为key的节点
	RBTNode<T>* minimum(RBTNode<T>* tree);//查找最小点
	RBTNode<T>* maximum(RBTNode<T>* tree);//查找最大点
	RBTNode<T>* successor(RBTNode<T>* x);//找到节点x的后继节点，即查找“红黑树中数据值大于该节点”的“最小节点”
	RBTNode<T>* predecessor(RBTNode<T>* x);//找节点x的前驱节点，即查找“红黑树中数据值小于该节点”的“最大节点”

	void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);//左旋
	void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);//右旋
	void insert(RBTNode<T>* &root, RBTNode<T>* node);//插入函数
	void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);//插入修正函数
	void remove(RBTNode<T>* &root, RBTNode<T>* node);//删除函数
	void removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent);//删除修正函数

	void destroy(RBTNode<T>* &tree);//销毁红黑树
	void print(RBTNode<T>* tree, T key, int direction);//打印红黑树

#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r) ((r)->color==RED)
#define rb_is_black(r) ((r)->color==BLACK)//;
#define rb_set_black(r) do{(r)->color=BLACK;} while(0)
#define rb_set_red(r) do{(r)->color=RED;} while(0)
#define rb_set_parent(r,p) do{(r)->parent=(p);} while(0)
#define rb_set_color(r,c) do{(r)->color=(c);} while(0)
};

/*
红黑树性质：
1、节点为红色或黑色
2、根节点是黑色
3、每个叶子的节点都是黑色的nullptr
4、每个红色节点的两个子节点都是黑色
5、从任意节点到其每个叶子的所有路径都包括相同的黑色节点数

红黑树特点：
1、任意两条路径长度满足：length（P）小于等于 2*length（Q）
2、插入和删除操作后，节点颜色的改变是沿着从插入或删除节点到根的方向进行的，因此时间O(log n)，每次插入/删除操作后，最多一次旋转即可恢复平衡，因为旋转和颜色改变需要时间O(1)，所以对数耗时
*/
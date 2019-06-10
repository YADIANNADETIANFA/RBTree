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
	RBTNode<T>* mRoot;//���ڵ�
	RBTree();
	~RBTree();

	void preOrder(RBTNode<T>* tree) const;//ǰ����������
	void inOrder(RBTNode<T>* tree) const;//������������
	void postOrder(RBTNode<T>* tree) const;//������������

	RBTNode<T>* search(RBTNode<T>* x, T key) const;//�ݹ�ʵ�ֲ��Һ�����м�ֵΪkey�Ľڵ�
	RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;//����ʵ�ֲ��Һ�����м�ֵΪkey�Ľڵ�
	RBTNode<T>* minimum(RBTNode<T>* tree);//������С��
	RBTNode<T>* maximum(RBTNode<T>* tree);//��������
	RBTNode<T>* successor(RBTNode<T>* x);//�ҵ��ڵ�x�ĺ�̽ڵ㣬�����ҡ������������ֵ���ڸýڵ㡱�ġ���С�ڵ㡱
	RBTNode<T>* predecessor(RBTNode<T>* x);//�ҽڵ�x��ǰ���ڵ㣬�����ҡ������������ֵС�ڸýڵ㡱�ġ����ڵ㡱

	void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);//����
	void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);//����
	void insert(RBTNode<T>* &root, RBTNode<T>* node);//���뺯��
	void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);//������������
	void remove(RBTNode<T>* &root, RBTNode<T>* node);//ɾ������
	void removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent);//ɾ����������

	void destroy(RBTNode<T>* &tree);//���ٺ����
	void print(RBTNode<T>* tree, T key, int direction);//��ӡ�����

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
��������ʣ�
1���ڵ�Ϊ��ɫ���ɫ
2�����ڵ��Ǻ�ɫ
3��ÿ��Ҷ�ӵĽڵ㶼�Ǻ�ɫ��nullptr
4��ÿ����ɫ�ڵ�������ӽڵ㶼�Ǻ�ɫ
5��������ڵ㵽��ÿ��Ҷ�ӵ�����·����������ͬ�ĺ�ɫ�ڵ���

������ص㣺
1����������·���������㣺length��P��С�ڵ��� 2*length��Q��
2�������ɾ�������󣬽ڵ���ɫ�ĸı������ŴӲ����ɾ���ڵ㵽���ķ�����еģ����ʱ��O(log n)��ÿ�β���/ɾ�����������һ����ת���ɻָ�ƽ�⣬��Ϊ��ת����ɫ�ı���Ҫʱ��O(1)�����Զ�����ʱ
*/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <deque>
#include <iomanip>
#include <sstream>
#define SENTINEL -100000
using namespace std;

typedef struct interval {
	int low;
	int high; 
}Interval;

typedef struct node {
	struct node* p;
	struct node* left;
	struct node* right;
	int num;//课程编号
	char name[32];//课程名称
	int max;
	Interval interval;
	int color;//0黑色，1红色
}Node;

Node* nil = NULL; //定义nil结点
Node* T = NULL;


//找最小值
Node* TreeMin(Node* x)
{
	while (x->left != nil) x = x->left;
	return x;
}

//找最大值
Node* TreeMax (Node* x)
{
	while (x->right != nil) x = x->right;
	return x;
}

int Max (int a, int b, int c)
{
	int d;
    d = a>=b?a:b;
    return d>=c?d:c;
}


//左旋
void LeftRotate(Node* x)
{
	if (x->right != nil)
	{
		Node* y = x->right; //定义y为x的右结点

		x->right = y->left; //y的左结点变成x的右结点
		if (y->left != nil)
			y->left->p = x; //y左结点的父节点变为x

		y->p = x->p;  //y的父节点变成x的父节点
		if (x->p == nil) //如果x是根结点
			T = y;
		else if (x == x->p->left) //如果x是左结点
				x->p->left = y; //y也成为左结点
		else x->p->right = y; //y成为右结点

		y->left = x; //y的左结点变成x
		x->p = y; //x的父结点变成y

		y->max = x->max;
		x->max = Max(x->interval.high, x->left->max, x->right->max);
	}
	else printf("右结点为空不能左旋！\n");
}

//右旋
void RightRotate(Node* x)
{
	if (x->left != nil)
	{
		Node* y = x->left; //定义y为x的左结点

		x->left = y->right; //y的右结点变成x的左结点
		if (y->right != nil)
			y->right->p = x; //y右结点的父节点变为x

		y->p = x->p;  //y的父节点变成x的父节点
		if (x->p == nil) //如果x是根结点
			T = y;
		else if (x == x->p->right) //如果x是右结点
				x->p->right = y; //y也成为右结点
		else x->p->left = y; //y成为左结点

		y->right = x; //y的右结点变成x
		x->p = y; //x的父结点变成y

		y->max = x->max;
		x->max = Max(x->interval.high, x->left->max, x->right->max);
	}
	else printf("左结点为空不能右旋！\n");
}

//插入调整
/*1. 父结点红，叔叔结点红
2. 父结点红，叔叔结点黑，是父结点的右子
3. 父结点红，叔叔结点黑，是父结点的左子
*/
void InsertFixup(Node* z)
{
	Node* y;
	while (z->p->color == 0) //当父结点为红色
	{
		if (z->p == z->p->p->left) //z.p是左孩子
		{
			y = z->p->p->right; //这个右孩子就是叔叔y

			//情况1：叔叔和父结点都变成黑色，祖父变红
			if (y->color == 0) 
			{
				z->p->color = 1;
				y->color = 1;
				z->p->p->color = 0;
				z = z->p->p;
			}
			else if (z == z->p->right) 
			{
				z = z->p; //父结点成为新的当前结点
				LeftRotate(z); //以父结点左旋
			}
			else 
			{
				z->p->color = 1; //父结点变黑
				z->p->p->color = 0; //祖父变红
				RightRotate(z->p->p); //以祖父右旋
			}
			
		}
		else //z.p是右孩子
		{
			y = z->p->p->left; //这个左孩子就是叔叔y

			//情况1：叔叔和父结点都变成黑色，祖父变红
			if (y->color == 0) 
			{
				z->p->color = 1;
				y->color = 1;
				z->p->p->color = 0;
				z = z->p->p;//?
			}
			else if (z == z->p->left) 
			{
				z = z->p; //父结点成为新的当前结点
				RightRotate(z); //以父结点右旋
			}
			else
			{
				z->p->color = 1; //父结点变黑
				z->p->p->color = 0; //祖父变红
				LeftRotate(z->p->p); //以祖父左旋
			} 
		}
	}
	T->color = 1;//根结点为黑
}


//插入
void Insert(int num, char* name, Interval i)
{
	if (T == NULL)
	{
		//如果根尚不存在，那么new一个新结点给根和一个新结点给nil
		T = (Node*)malloc(sizeof(Node));
		nil = (Node*)malloc(sizeof(Node));
		 
		strcpy(T->name, name);
		T->num = num;

		T->left = T->right = T->p = nil;
		T->interval.low = i.low;
		T->interval.high = i.high;
		T->max = i.high;
		T->color = 1;

		nil->max = 0;
		nil->color = 1; //nil设为黑
		nil->interval.low = nil->interval.high = SENTINEL;
	}
	else {
		//二叉树查找
		Node* y = nil;
		Node* x = T;

		while (x != nil) {
			/*if(i.low==x->interval.low)
            {
                cout<<"键值重复，请输入不同的键值！！"<<endl;
                return;
            }*/
            x->max = Max(x->max, i.high, 0);//i.high即z->max
			y = x;
			if (i.low < x->interval.low){
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		Node* z = (Node*)malloc(sizeof(Node)); //新结点
		z->p = y;

		if (y == nil) {
			T = z;
			z->color = 1; //根结点为黑色
		}
		else if (i.low < y->interval.low)
			y->left = z;
		else y->right = z;

		z->left = nil;
		z->right = nil;
		z->color = 0; //红色
		z->interval.low = i.low;
		z->interval.high = i.high;
		z->max = i.high;
		z->num = num;
		strcpy(z->name, name);

		InsertFixup(z);
	}
}

//用另一棵子树v，替换一棵子树u并成为其双亲的孩子结点
void Transplant(Node* u, Node* v)
{
	if (u->p == nil) //u是根结点
		T = v;
	else if (u == u->p->left) //u是左结点
		u->p->left = v;
	else u->p->right = v; //u是右结点

	v->p = u->p;
}



/*当y是黑结点是进行调整
1. x的兄弟结点w是红色：改变w和x.p的颜色，对x.p左旋
2. x的兄弟结点w是黑色，w子节点都为黑：x黑w红
3. x的兄弟结点w是黑色，w左孩子红，右孩子黑：交换w和w.left的颜色，对w右旋
4. x的兄弟结点w是黑色，w右孩子红：x.p左旋
*/
//删除调整
void DeleteFixup(Node* x)
{
	Node* w;
	while (x != T && x->color == 1) //将额外的黑色沿树上移
	{
		if (x == x->p->left)//如果x是左子树
		{
			w = x->p->right; //兄弟w就是右子树

			//情况1
			if (w->color == 0) 
			{
				w->color = 1;
				x->p->color = 0;
				LeftRotate(x->p);
				w = x->p->right;
			}
			//情况2
			if (w->left->color == 1 && w->right->color == 1)
			{
				w->color = 0;
				x = x->p;
			}
			//情况3
			else 
			{
				if (w->right->color == 1)
				{
					w->left->color = 1;
					w->color = 0;
					RightRotate(w);
					w = x->p->right;
				}
				//情况4
				w->color = x->p->color;
				x->p->color = 1;
				w->right->color = 1;
				LeftRotate(x->p);
				x = T;	
			}
		}
		else 
		{
			w = x->p->left; //兄弟w就是左子树

			//情况1
			if (w->color == 0) 
			{
				w->color = 1;
				x->p->color = 0;
				RightRotate(x->p);
				w = x->p->left;
			}
			//情况2
			if (w->right->color == 1 && w->left->color == 1)
			{
				w->color = 0;
				x = x->p;
			}
			//情况3
			else 
			{
				if (w->left->color == 1)
				{
					w->right->color = 1;
					w->color = 0;
					LeftRotate(w);
					w = x->p->left;
				}
				//情况4
				w->color = x->p->color;
				x->p->color = 1;
				w->left->color = 1;
				RightRotate(x->p);
				x = T;
			}				
		}
	}
	x->color = 1;
}

//删除
/*1. 没有儿子，直接删除
2. 只有一个儿子则，祖父指向儿子
3. 两个儿子，把右儿子中的最小元素y放到待删除位置
*/
void Delete(Node* z)
{
	Node* x;
	Node* y = z; 
	int y_color = y->color; //y原本的颜色	

	if (z->left == nil) //z没有左结点
	{
		x = z->right;
		Transplant(z, z->right); //z的右结点或者nil替代z
	}
	else if (z->right == nil) //z有左结点没右结点
	{
		x = z->left; 
		Transplant(z, z->left); //z左结点替代z
	}
	else //左右结点都有
	{
		//寻找右子树中的孩子y（且y没有左孩子）
		y = TreeMin(z->right);
		y_color = y->color;
		x = y->right; //x是y的右孩子
		if (y->p == z) //如果y是z的右孩子
			x->p = y; //仅留下y的右孩子
		else //y不是z的右孩子
		{
			Transplant(y, y->right);//先用y的右孩子替换y
			y->right = z->right;
			y->right->p = y;
		}
		Transplant(z, y); //y替换z
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}

	//对附加信息的维护
    int temp_left_max,temp_right_max;
    Node *temp;
    temp = x->p;

    while(temp != nil){
        if(temp->left != nil) temp_left_max=temp->left->max; 
        else temp_left_max=0;
        if(temp->right != nil) temp_right_max=temp->right->max;
        else temp_right_max=0;

        temp->max=Max(temp->interval.high,temp_left_max,temp_right_max);
        temp=temp->p;
    }
    if (y_color == 1)
    	DeleteFixup(x);
}

//中序遍历查找指定区间
void IntervalSearch(Node* x, Interval i)
{
	if (x != nil)
    {
		IntervalSearch(x->left, i);
		if ((x->interval.low <= i.low && i.low <= x->interval.high) 
			|| (x->interval.low <= i.high && i.high <= x->interval.high))
		{
			cout << "课程编号:" << x->num << "\t课程名称:" << x->name 
			<< "\t课程时间:" << x->interval.low << " - " << x->interval.high <<endl;
		}
		IntervalSearch(x->right, i);
	}
}



//查找重叠区间的第一个课删除
Node* Search(Node* x, Interval i)
{
	if (x  == nil || (i.low ==
	 x->interval.low && i.high == x->interval.high)){
		if (x == nil) cout<<"没有指定时间段的课程！"<<endl;
		return x;
	}
	if(i.low > x->left->max)
		return Search(x->right, i);
	else return Search(x->left, i);
}

//中序遍历找到结点是否存在
int InorderSearch(Node* x, int num, int flag)
{
	if (x != nil)
    {
		flag = InorderSearch(x->left, num, flag);
		if (x->num == num) flag = 1;
		flag = InorderSearch(x->right, num, flag);
		return flag;
	}
	return flag;
}

//中序遍历输出整个树
void InorderTreeWalk(Node* x)
{
	if (x != nil)
    {
		InorderTreeWalk(x->left);
		cout << "课程编号:" << x->num << "\t课程名称:" << x->name 
		<< "\t课程时间:" << x->interval.low << " - " << x->interval.high <<endl;
		InorderTreeWalk(x->right);
	}
}

//打印二叉树
void Print(Node* p, char ss[][64], int count, int &key, int dir) {
	if(p == nil)	return;
	count += 8;
	Print(p->right, ss, count, key, 1);

	for(int i = key+1; i < 15; i++) {
		if(dir == 1)	ss[i][count] = '|';
		else			ss[i][count] = ' ';
	}

	ss[key][count++] = '[';
	ss[key][count++] = p->interval.low;
	ss[key][count++] = ',';
	ss[key][count++] = p->interval.high;
	ss[key][count++] = ']';

	ss[key][count++] = p->max;
	ss[key][count++] = ':';
	ss[key][count++] = p->color;
	ss[key][count++] = '|';
	key++;
	if(p->left != nil)ss[key][count-1] = '|';

	count -= 9;

	Print(p->left, ss, count, key, 2);
	count -= 8;
}

void PrintTree() {
	int key = 0;
	char pic[15][64];
	for(int i = 0; i < 15; i++) 
		for(int j = 0; j < 64; j++)
			pic[i][j] = ' ';

	Print(T, pic, 0, key, 1);
	for(int i = 0; i < 15; i++)
		pic[i][4] = ' ';
	for(int i = 0; i < 15; i++) {
		for(int j = 0; j < 64; j++){
			if (pic[i][j] == ' ' || pic[i][j] == '-' || pic[i][j] == '|' || pic[i][j] == ':' 
				|| pic[i][j] == ',' || pic[i][j]=='[' || pic[i][j] ==']')
				printf("%c", pic[i][j]);
			else printf("%d", pic[i][j]);
		}
		printf("\n");
	}

}

int main()
{
	Interval i;
	char choose;
	int num;
	Interval time;
	char name[32]={""};

	time.low=19; time.high=20;	strcpy(name,"Chi"); Insert(8, name, time);
	time.low=0; time.high=3; 	strcpy(name,"math"); Insert(1, name, time);
	time.low=10; time.high=24;	strcpy(name,"Net"); Insert(5, name, time);
	time.low=5; time.high=8;	strcpy(name,"alg"); Insert(2, name, time);
	time.low=16; time.high=21;	strcpy(name,"Phy"); Insert(6, name, time);
	time.low=6; time.high=10; 	strcpy(name,"Eng"); Insert(3, name, time);
	time.low=10; time.high=23;	strcpy(name,"Exp"); Insert(10, name, time);
	time.low=8; time.high=9;	strcpy(name,"Sec"); Insert(4, name, time);
	time.low=17; time.high=19;	strcpy(name,"Che"); Insert(7, name, time);
	time.low=22; time.high=24;	strcpy(name,"PE"); Insert(9, name, time);

	while (1)
	{
		cout<<"\n------------课表管理系统----------"<<endl;
		cout<<"插入课程--I"<<endl;
		cout<<"删除课程--D"<<endl;
		cout<<"查询课程--S"<<endl;
		cout<<"显示当前课程信息--P"<<endl;
		cout<<"打印二叉树--R"<<endl;
		cout<<"退出系统--Q"<<endl;
		cout<<"请选择操作: ";
		cin>>choose;
		switch (choose) 
		{
			case 'I': cout<<"\n输入课程编号: ";
					cin>>num;
					if (InorderSearch(T,num,0)){
						cout<<"课程编号已存在！"<<endl;
						break;
					}
					cout<<"输入课程名字: ";
					cin>>name;
					cout<<"输入上课区间, 空格隔开(0-24): ";
					cin>>time.low>>time.high;
					if (time.low >= time.high || time.high > 24 || time.low < 0 ) {
						cout<<"课程时间不符合规则！"<<endl;
						break;
					}
					Insert(num, name, time);
					break;
			case 'D': cout<<"输入上课区间, 空格隔开(0-24): ";
					cin>>time.low>>time.high;
					if (time.low >= time.high || time.high > 24 || time.low < 0 ) {
						cout<<"课程时间不符合规则！"<<endl;
						break;
					}
					if (Search(T,time) != nil)
						Delete(Search(T, time));
					break;
			case 'S': cout<<"输入上课区间, 空格隔开(0-24): ";
					cin>>time.low>>time.high;
					if (time.low >= time.high || time.high > 24 || time.low < 0 ) {
						cout<<"课程时间不符合规则！"<<endl;
						break;
					}
					IntervalSearch(T, time);
					break;
			case 'P': InorderTreeWalk(T);break;
			case 'R': PrintTree();break;
					  //PrintPretty(T,2,3);break;
			case 'Q': exit(0);break;
		}
	}
}

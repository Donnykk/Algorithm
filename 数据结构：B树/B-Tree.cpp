#include<iostream>
#include<cstdio>
#include<cmath>
#include<queue>
#include<algorithm>
#include<graphics.h>
using namespace std;

static int m;
static int MIN;

//节点的构造
typedef class node {
public:
	int keynum;//当前节点关键字个数
	int* key = new int[m + 1];//关键字数组,key[m]正常境况下不存储
	node* parent;
	node** ptr = new node * [m + 1];//孩子节点指针数组
	node() {
		keynum = 0;
		parent = nullptr;
		for (int i = 0; i <= m; i++)
		{
			ptr[i] = nullptr;
		}
	}
};

class Btree {
public:
	node* result;
	int tar;
	bool tag;
	node* root;

	//在节点p上查找k,并返回第一个大于等于k的位置
	int search(node* p, int k)
	{
		int i = 0;
		while (i<p->keynum && k>p->key[i])
			i++;
		return i;
	}

	//在整个B树上进行查找
	void searchBtree(int k)
	{
		int i = 0;
		bool found = false;
		node* p = root;
		node* q = nullptr;
		while (p != nullptr && !found)
		{
			i = search(p, k);
			if (i < p->keynum && p->key[i] == k)
				found = true;
			else
			{
				q = p;
				p = p->ptr[i];
			}
		}
		if (found)
		{
			result = p;
			tar = i;
			tag = true;
		}
		else
		{
			result = q;
			tar = i;
			tag = false;
		}
	}

	//分裂操作,将q节点一分为二，前半部分留在q节点，后半部分存在新建节点ap
	void spilt(node*& q, node*& ap)
	{
		int i;
		int j;
		int n = q->keynum;
		int start = n % 2 == 0 ? n / 2 : (n + 1) / 2;
		ap = new node();
		ap->ptr[0] = q->ptr[start];
		for (i = start, j = 0; i < n; i++, j++)
		{
			ap->key[j] = q->key[i];
			ap->ptr[j + 1] = q->ptr[i + 1];
		}
		ap->keynum = n / 2;
		ap->parent = q->parent;
		for (i = 0; i <= ap->keynum; i++)
		{
			if (ap->ptr[i] != nullptr)
				ap->ptr[i]->parent = ap;
		}
		q->keynum = n - ap->keynum - 1;
	}

	//创建新的root节点
	void createRoot(node*& root, node* p, int x, node* ap)
	{
		root = new node();
		root->keynum = 1;
		root->key[0] = x;
		root->ptr[0] = p;
		root->ptr[1] = ap;
		if (p)
			p->parent = root;
		if (ap)
			ap->parent = root;
		root->parent = nullptr;
	}

	//插入到节点上
	void insertNode(node*& q, int x, int i, node* ap)//关键字x和新节点指针插入到q->key[i]和q->ptr[i]上
	{
		int j;
		int n = q->keynum;
		for (j = n; j > i; j--)
		{
			q->key[j] = q->key[j - 1];
			q->ptr[j + 1] = q->ptr[j];
		}
		q->key[i] = x;
		q->ptr[i + 1] = ap;
		if (ap != nullptr)
			ap->parent = q;
		q->keynum++;
	}

	//进行插入操作
	void insert(int val)
	{
		//先查找该值的位置,更新result,tar,tag
		searchBtree(val);
		if (tag)
		{
			cout << "WARNING:该值已经存在" << endl;
		}
		else
		{
			node* ap = nullptr;
			bool success = false;
			bool needRoot = false;
			node* q = result;
			int x = tar;
			if (q == nullptr)
				createRoot(root, nullptr, val, nullptr);
			else
			{
				while (!success && !needRoot)
				{
					insertNode(q, val, x, ap);
					if (q->keynum < m + 1)
					{
						success = true;
					}
					else
					{
						//一定要先更新值
						int cur = q->keynum % 2 == 0 ? q->keynum / 2 - 1 : q->keynum / 2;
						val = q->key[cur];//要插入的值换成q的中间值
						spilt(q, ap);
						if (q->parent != nullptr)
						{
							q = q->parent;
							x = search(q, val);//在父节点上查找新的val要插入的位置
						}
						else
						{
							needRoot = true;
						}
					}
					if (needRoot)
					{
						createRoot(root, q, val, ap);
					}
				}
			}
		}
	}

	void remove(node*& p, int x)
	{
		int n = p->keynum;
		for (int i = x; i < n - 1; i++)
		{
			p->key[i] = p->key[i + 1];
			p->ptr[i] = p->ptr[i + 1];
		}
		p->ptr[p->keynum - 1] = p->ptr[p->keynum];
		p->keynum--;
	}

	void replace(node*& q, int x)
	{
		node* son = q->ptr[x];
		while (son->ptr[son->keynum])
		{
			son = son->ptr[son->keynum];
		}
		q->key[x] = son->key[son->keynum - 1];
		q = son;
	}

	//删除后节点上的key值数量小于下限需要调整B树
	void adjustBtree(node*& p)
	{
		node* par = p->parent;
		if (par == nullptr)
		{
			root = p;
			p = p->parent; //p置空
			return; 
		}
		node* rc, * lc, * pr;
		bool finished = false;
		int r = 0;
		while (!finished)
		{
			r = 0;
			while (par->ptr[r] != p)
			{
				r++;
			}
			if (r == 0)
			{
				lc = nullptr;
				rc = par->ptr[r + 1];
			}
			else if (r == par->keynum)
			{
				lc = par->ptr[r - 1];
				rc = nullptr;
			}
			else
			{
				lc = par->ptr[r - 1];
				rc = par->ptr[r + 1];
			}
			if (r > 0 && lc != nullptr && (lc->keynum > MIN))//向左子树借关键字
			{
				p->keynum++;
				for (int i = p->keynum - 1; i > 0; i--)
				{
					p->key[i] = p->key[i - 1];
					p->ptr[i + 1] = p->ptr[i];
				}
				p->key[0] = par->key[r - 1];
				p->ptr[1] = p->ptr[0];
				p->ptr[0] = lc->ptr[lc->keynum];

				//修改父节点
				if (p->ptr[0] != nullptr)
				{
					p->ptr[0]->parent = p;
				}

				par->key[r - 1] = lc->key[lc->keynum - 1];
				lc->keynum--;//少了一个key值和一个指针
				finished = true;
				break;
			}
			else if (r < par->keynum && rc != nullptr && (rc->keynum > MIN))//向右子树借关键字
			{
				p->keynum++;
				p->key[p->keynum - 1] = par->key[r];
				p->ptr[p->keynum] = rc->ptr[0];

				if (p->ptr[p->keynum] != nullptr)
				{
					p->ptr[p->keynum]->parent = p;
				}

				par->key[r] = rc->key[0];
				rc->ptr[0] = rc->ptr[1];
				for (int i = 0; i < rc->keynum - 1; i++)
				{
					rc->key[i] = rc->key[i + 1];
					rc->ptr[i + 1] = rc->ptr[i + 2];
				}

				rc->keynum--;
				finished = 1;
				break;
			}
			//重新确定位置
			r = 0;
			while (par->ptr[r] != p)
			{
				r++;
			}
			if (r > 0 && (par->ptr[r - 1]->keynum) <= MIN)//与左子树进行合并
			{
				lc = par->ptr[r - 1];
				p->keynum++;
				for (int i = p->keynum - 1; i > 0; i--)
				{
					p->key[i] = p->key[i - 1];
					p->ptr[i + 1] = p->ptr[i];
				}
				p->key[0] = par->key[r - 1];
				p->ptr[1] = p->ptr[0];
				par->ptr[r] = lc;

				for (int i = 0; i < p->keynum; i++)
				{
					lc->key[lc->keynum + i] = p->key[i];
					lc->ptr[lc->keynum + i + 1] = p->ptr[i + 1];
				}
				if (p->ptr[0])//更改父节点
				{
					for (int i = 1; i <= p->keynum; i++)
					{
						if (p->ptr[i] != nullptr)
							p->ptr[i]->parent = lc;
					}
				}
				lc->keynum = lc->keynum + p->keynum;
				//更新父节点
				for (int i = r - 1; i < par->keynum - 1; i++)
				{
					par->key[i] = par->key[i + 1];
					par->ptr[i + 1] = par->ptr[i + 2];
				}
				par->keynum--;
				p = nullptr;
				p = lc;
			}
			else//与右子树进行合并
			{
				rc = par->ptr[r + 1];
				p->keynum++;
				p->key[p->keynum - 1] = par->key[r];
				p->ptr[p->keynum] = rc->ptr[0];
				rc->keynum = rc->keynum + p->keynum;
				par->ptr[r] = rc;
				for (int i = 0; i < rc->keynum - p->keynum; i++)
				{
					rc->key[p->keynum + i] = rc->key[i];
					rc->ptr[p->keynum + 1 + i] = rc->ptr[i + 1];
				}
				for (int i = 0; i < p->keynum; i++)
				{
					rc->key[i] = p->key[i];
					rc->ptr[i + 1] = p->ptr[i + 1];
				}
				rc->ptr[0] = p->ptr[0];
				//更改父节点
				if (p->ptr[0])
				{
					for (int i = 0; i < p->keynum; i++)
					{
						if (p->ptr[i])
							p->ptr[i]->parent = rc;
					}
				}
				for (int i = r; i < par->keynum; i++)
				{
					par->key[i] = par->key[i + 1];
					par->ptr[i] = par->ptr[i + 1];
				}
				par->keynum--;
				p = nullptr;
				p = rc;
			}
			par = par->parent;
			if (p->parent->keynum >= MIN || (par == nullptr && p->parent->keynum >= 1))
				finished = 1;
			else if (par == nullptr)
			{
				root = nullptr;
				root = p;
				root->parent = nullptr;
				finished = 1;
			}
			p = p->parent;
		}
	}

	void deleteFromBtree(node*& p, int x)
	{
		if (p->ptr[x] != nullptr)
		{
			replace(p, x);
			deleteFromBtree(p, p->keynum - 1);//删除左子树上的最大，最大的节点上的keynum-1为最大
		}
		else
		{
			remove(p, x);
			if (p->parent == nullptr && p->keynum == 0)
				cout << "删除后树为空" << endl;
			else if (p->keynum < MIN)
				adjustBtree(p);
		}
	}

	void destory(node* p)
	{
		int i = 0;
		if (p != nullptr)
		{
			while (i <= p->keynum)
			{
				destory(p->ptr[i]);
				free(p->ptr[i]);
				i++;
			}
		}
		free(p);
		p = nullptr;
	}

	void createBtree()
	{
		root = new node();
		root->keynum = 0;
		cout << "成功创建B-树" << endl;
	}

	void insertElement()
	{
		cout << "请输入要插入的值：" << endl;
		int temp;
		cin >> temp;
		TCHAR s[20];
		//查找并且更新result和tar,tag
		searchBtree(temp);
		if (tag)
		{
			settextstyle(20, 20, _T("楷体"));
			settextcolor(RGB(195, 39, 43));
			outtextxy(350, 10, _T("该值已存在"));
			Sleep(1000);
		}
		else
		{
			settextstyle(20, 20, _T("楷体"));
			settextcolor(RGB(195, 39, 43));
			outtextxy(350, 10, _T("插入："));
			swprintf_s(s, _T("%d"), temp);
			outtextxy(450, 10, s);
			Sleep(1000);
			insert(temp);
		}
	}

	void deleteElement()
	{
		cout << "请输入要删除的值" << endl;
		int temp;
		cin >> temp;
		TCHAR s[20];
		searchBtree(temp);
		if (!tag)
		{
			settextstyle(20, 20, _T("楷体"));
			settextcolor(RGB(195, 39, 43));
			outtextxy(350, 10, _T("该值不存在"));
			Sleep(1000);
		}
		else
		{
			settextstyle(20, 20, _T("楷体"));
			settextcolor(RGB(195, 39, 43));
			outtextxy(350, 10, _T("删除："));
			swprintf_s(s, _T("%d"), temp);
			outtextxy(450, 10, s);
			Sleep(1000);
			deleteFromBtree(result, tar);
		}
	}

	void graphDisplay(node* p)
	{
		if (p == nullptr)
		{
			cout << "B树还没有被创建" << endl;
			return;
		}
		else
		{
			queue<node*>q;
			queue<int>pos;
			int lastY = 120;
			cleardevice();
			setlinecolor(RGB(255, 255, 255));
			settextstyle(18, 12, _T("黑体"));
			settextcolor(RGB(255, 255, 255));
			outtextxy(10, 10, _T("1:插入"));
			outtextxy(10, 30, _T("2:删除"));
			outtextxy(10, 50, _T("3:退出"));
			int start = 400 - root->keynum * 10;
			for (int i = 0; i < p->keynum; i++)
			{
				rectangle(start + i * 20, 100, start + i * 20 + 20, 120);
				TCHAR s[20];
				swprintf_s(s, _T("%d"), root->key[i]);
				settextstyle(10, 0, _T("黑体"));
				settextcolor(RGB(255, 255, 255));
				outtextxy(start + i * 20 + 8, 105, s);
				pos.push(start + i * 20);//放入上一层要与下一层连接的点位
				if (p->ptr[i] != nullptr)
					q.push(p->ptr[i]);
			}
			//还要判断一下最后一个节点即ptr[keynum]
			if (p->ptr[p->keynum] != nullptr)
			{
				q.push(p->ptr[p->keynum]);
				pos.push(start + p->keynum * 20);
			}
			while (!q.empty())
			{
				int n = q.size();//当前层的节点个数
				start = 400 - n * (m + 1) * 10;//×(m+1)的原因，每个节点还要空出20
				for (int i = 0; i < n; i++)
				{
					node* cur = q.front();
					q.pop();
					int lastX = pos.front();
					pos.pop();
					int x = start + i * (m + 1) * 20;
					int y = lastY + 20;
					int mid = x + cur->keynum * 10;
					//开始连线
					line(lastX, lastY, mid, y);

					//开始画自己节点上的值
					for (int i = 0; i < cur->keynum; i++)
					{
						rectangle(x + i * 20, y, x + i * 20 + 20, y + 20);
						TCHAR s[20];
						swprintf_s(s, _T("%d"), cur->key[i]);//完美解决
						settextstyle(10, 0, _T("黑体"));
						outtextxy(x + i * 20 + 8, y + 5, s);
						pos.push(x + i * 20);//放入上一层要与下一层连接的点位
						if (cur->ptr[i] != nullptr)
							q.push(cur->ptr[i]);//错误的根源搞成了T->,应该为cur->
					}
					if (cur->ptr[cur->keynum] != nullptr)
					{
						q.push(cur->ptr[cur->keynum]);
						pos.push(x + cur->keynum * 20);
					}
				}
				lastY += 40;
			}
			Sleep(500);
		}
	}
};
int main()
{
	cout << "请输入B树的关键字个数:" << endl;
	int M, N;
	cin >> M;
	cout << "请输入B树的阶：" << endl;
	cin >> N;
	int* keys = new int[M];
	cout << "请输入" << M << "个关键字" << endl;
	for (int i = 0; i < M; i++)
		cin >> keys[i];
	m = N - 1;
	MIN = N % 2 == 0 ? N / 2 - 1 : (N + 1) / 2 - 1;
	Btree b;
	b.createBtree();
	for (int i = 0; i < M; i++)
	{
		b.insert(keys[i]);
	}

	//设置画布，并设置背景
	initgraph(800, 600);
	setbkcolor(RGB(0, 0, 0));
	b.graphDisplay(b.root);
	while (1)
	{
		cout << "请输入要进行的操作: 1:插入 2：删除 3：退出" << endl;
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			closegraph();
			b.insertElement();
			initgraph(800, 600);
			setbkcolor(RGB(0, 0, 0));
			b.graphDisplay(b.root);
			break;
		case 2:
			closegraph();
			b.deleteElement();
			initgraph(800, 600);
			setbkcolor(RGB(0, 0, 0));
			b.graphDisplay(b.root);
			break;
		case 3:
			closegraph();
			return 0;
			break;
		}
	}
	return 0;
}
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;
char passage[1000000]; //文章
string word[10000]; //停词表

class ListNode 
{
public:
	char* word;
	int freq;
	ListNode* next;

	ListNode(char* word, int freq)
	{
		this->word = new char[strlen(word) + 1];
		for (int i = 0; i < strlen(word); i++)
		{
			this->word[i] = word[i];
		}
		this->word[strlen(word)] = '\0';
		this->freq = freq;
		next = nullptr;
	}
};

void swap(ListNode* p, ListNode* q)  //交换两个结点值
{
	int temp = p->freq;
	p->freq = q->freq;
	q->freq = temp;

	char* tempw = p->word;
	p->word = q->word;
	q->word = tempw;
}

void sortList(ListNode* head, ListNode* tail) //快排降序排序
{
	if (head == tail || head == nullptr)
		return;
	ListNode *p = head, *q = head;
	int Maxx = head->freq;
	while (q != tail)
	{
		q = q->next;
		if (q->freq > Maxx)
		{
			p = p->next;
			swap(p, q);
		}
	}
	swap(head, p);
	sortList(head, p);
	sortList(p->next, tail);
}

bool Compare(string str, char* arr2)  //比较字符串是否相等
{
	if (str.size() != strlen(arr2))
		return false;
	else
	{
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] != arr2[i])
				return false;
		}
		return true;
	}
}

int main()
{
	int len_passage = 0;
	int len_words = 0;
	ListNode* head[26] = { nullptr }; //数组存放链表头指针

	fstream infile;
	infile.open("C:\\Users\\17898\\Desktop\\Don\\小王子英文原版.txt");
	while (!infile.eof()) 
	{
		infile.get(passage[len_passage]);
		if (passage[len_passage] >= 'A' && passage[len_passage] <= 'Z')  //字母转换为小写
			passage[len_passage] += 32;
		if (passage[len_passage] >= 'a' && passage[len_passage] <= 'z' || passage[len_passage] == ' ')  //过滤符号
			len_passage++;
	}
	infile.close();

	fstream fin("C:\\Users\\17898\\Desktop\\Don\\停词表.txt");
	while (!fin.eof())
	{
		fin >> word[len_words];
		len_words++;
	}
	fin.close();
	
	for (int i = 0; i < len_passage; i++)
	{
		if (passage[i] == ' ')
			continue;

		//获得单词temp
		char temp[10000] = { 0 };
		int j = 0;
		while (passage[i] != ' ')
		{
			temp[j++] = passage[i++];
		}
		
		//比对停词表
		bool exist = 0;
		for (int i = 0; i < len_words; i++)
		{
			if (Compare(word[i], temp))
			{
				exist = 1;
				break;
			}
		}

		if (exist == 0) 
		{
			int n = temp[0] - 'a';
			ListNode* tool = head[n];
			if (head[n] == nullptr)
			{
				head[n] = new ListNode(temp, 1);
			}
			else if (head[n]->next == nullptr)
			{
				if (strcmp(temp, head[n]->word) < 0)
				{
					head[n] = new ListNode(temp, 1);
					head[n]->next = tool;
				}
				else if (strcmp(temp, tool->word) > 0)
				{
					head[n]->next = new ListNode(temp, 1);
				}
				else
				{
					head[n]->freq++;
				}
			}
			else
			{
				if (strcmp(temp, head[n]->word) < 0)
				{
					head[n] = new ListNode(temp, 1);
					head[n]->next = tool;
				}
				else
				{
					bool putlast = true;
					while (tool->next != nullptr)
					{
						if (strcmp(temp, tool->word) == 0)
						{
							tool->freq++;
							putlast = false;
							break;
						}
						else if ((strcmp(temp, tool->next->word)) < 0 && (strcmp(temp, tool->word) > 0))
						{
							ListNode* newNode = tool->next;
							tool->next = new ListNode(temp, 1);
							tool->next->next = newNode;
							putlast = false;
							break;
						}
						else
							tool = tool->next;
					}
					if (putlast)
					{
						if (strcmp(temp, tool->word) == 0)
							tool->freq++;
						else
							tool->next = new ListNode(temp, 1);
					}
				}
			}
		}
	}

	for (int i = 0; i < 26; i++)  //按词频排序
	{
		ListNode* temp = head[i];
		if (temp == nullptr)
			continue;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		sortList(head[i], temp);
	}
	
	for (int i = 0; i < 26; i++)
	{
		ListNode* p = head[i];
		while (p != nullptr)
		{
			cout << p->word << " " << p->freq << endl;  //输出数据
			p = p->next;
		}
	}

	system("pause");
	return 0;
}
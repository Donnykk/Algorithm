#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
int freq[128] = { 0 }; //asc码对应字符出现频率

struct Node  //二叉树节点
{
public:
	char c;
	int num;
	Node* lchild, * rchild;
	Node(int num = 0, Node* lChild = nullptr, Node* rChild = nullptr, char c = 0) {
		this->num = num;
		this->lchild = lChild;
		this->rchild = rChild;
		this->c = c;
	}
};

class Huffman
{
private:
	vector<Node*> rootSet;  
	Node* root;  //树根
	Node* newRoot;
	string* code;  //字符到编码映射

public:
	Huffman()
	{
		newRoot = new Node();
		root = new Node();
		code = new string[128]{ "" };
	};

	void insert(vector<Node*>& Vec_root, Node* newNode)
	{
		Vec_root.push_back(newNode);
		int j = Vec_root.size() - 1;
		//升序排列
		for (; j > 0; j--)
		{
			if (newNode->num < Vec_root[j - 1]->num)
				Vec_root[j] = Vec_root[j - 1];
			else
				break;
		}
		Vec_root[j] = newNode;
	}

	void createTree(int* arr)   //生成哈夫曼树
	{
		for (int i = 0; i < 128; i++)
		{
			if (arr[i] == 0)
				continue;
			Node* newNode = new Node(arr[i], nullptr, nullptr, char(i));
			insert(rootSet, newNode);
		}
		if (rootSet.size() == 0)
			return;
		if (rootSet.size() == 1)
		{
			root->num = rootSet[0]->num;
			root->lchild = rootSet[0];
			return;
		}
		while (rootSet.size() > 2)
		{
			Node* leaf1, * leaf2;
			leaf1 = rootSet[0];
			leaf2 = rootSet[1];
			rootSet.erase(rootSet.begin());
			rootSet.erase(rootSet.begin());
			int num = leaf1->num + leaf2->num;
			Node* newNode = new Node(num, leaf1, leaf2);
			insert(rootSet, newNode);
		}
		root->num = rootSet[0]->num + rootSet[1]->num;
		root->lchild = rootSet[0];
		root->rchild = rootSet[1];
	}

	void createCode(Node* nowNode, string nowCode)  //生成对应编码
	{
		if (nowNode->c != 0)
		{
			code[int(nowNode->c)] = nowCode;
			return;
		}
		nowCode += '0';
		createCode(nowNode->lchild, nowCode);
		nowCode[nowCode.size() - 1] = '1';
		createCode(nowNode->rchild, nowCode);
	}

	void saveCode(int* arr)  //输出字符出现次数和编码
	{
		ofstream outfile;
		outfile.open("outputfile1.txt");
		for (int i = 0; i < 128; i++)
		{
			if (arr[i] > 0)
			{
				outfile << char(i) << ' ' << arr[i] << ' ' << code[i] << endl;
			}
		}
		outfile.close();
	}

	void generateHuffman(int* arr)
	{
		createTree(arr);
		createCode(root, "");
		saveCode(arr);
		return;
	}

	void coding()  //txt文件编码
	{
		ifstream input;
		input.open("inputfile2.txt");
		ofstream output;
		output.open("outputfile.dat", ios::binary);
		char ch = 0;
		int bit = 7;
		int originBytes = 1;
		int codedBytes = 0;
		char c;
		while (!input.eof())
		{
			input.get(c);
			if (c < 0)
				continue;
			else
			{
				string charCode = code[(int)c];
				for (int i = 0; i < charCode.size(); i++)
				{
					ch |= ((charCode[i] - '0') << bit);
					bit--;
					if (bit < 0)
					{
						output.write((char*)&ch, sizeof(ch));
						codedBytes++;
						ch = 0;
						bit = 7;
					}
				}
			}
			originBytes++;
		}
		if (bit != 7)
		{
			output.write((char*)&ch, sizeof(ch));
			ch = bit + 1;
			output.write((char*)&ch, sizeof(ch));
			codedBytes += 2;
		}
		else
		{
			ch = 0;
			output.write((char*)&ch, sizeof(ch));
			codedBytes++;
		}
		cout << "编码成功，压缩比为：" << double(1.0 * originBytes / codedBytes) << endl;
		input.close();
		output.close();
		return;
	}

	void decoding() //dat文件解码
	{
		ifstream input;
		input.open("outputfile.dat", ios::binary);
		ifstream infile;
		infile.open("outputfile1.txt");
		ofstream output;
		output.open("outputfile2.txt");
		char c = 0;
		input.seekg(-1, ios::end);
		input.read((char*)&c, sizeof(c));
		int remain = int(c);

		input.seekg(-2, ios::end);
		streampos pos_end = input.tellg();
		input.seekg(ios::beg);
		streampos pos_now = input.tellg();

		newRoot = new Node(0, nullptr, nullptr, 0);
		Node* temp = newRoot;
		char line[100];
		while (infile.getline(line, 100))
		{
			if (strlen(line) == 0) 
				continue;
			temp = newRoot;
			char tar = 0;
			int count = 0;
			string code = "";
			bool conti = false;
			if (line[0] == ' ' && line[1] == ' ')
				tar = ' ';
			else if (line[0] != ' ')
				tar = line[0];
			else if (line[0] == ' ' && line[1] != ' ')
				tar = '\n';
			for (int i = 3; i < 100 && line[i] != '\0'; i++)
			{
				if (conti == true || line[i - 1] == ' ')
				{
					code += line[i];
					conti = true;
				}
			}

			for (int i = 0; i < code.length() - 1; i++)
			{
				if (code[i] == '0')
				{
					if (temp->lchild == nullptr)
						temp->lchild = new Node(0, nullptr, nullptr, 0);
					temp = temp->lchild;
				}
				else if (code[i] == '1')
				{
					if (temp->rchild == nullptr)
						temp->rchild = new Node(0, nullptr, nullptr, 0);
					temp = temp->rchild;
				}
			}
			if (code[code.length() - 1] == '0')
			{
				temp->lchild = new Node(count, nullptr, nullptr, tar);
			}
			else if (code[code.length() - 1] == '1')
			{
				temp->rchild = new Node(count, nullptr, nullptr, tar);
			}
		}

		Node* nowNode = newRoot;
		while (pos_now != pos_end)
		{
			input.read((char*)&c, sizeof(c));
			for (int i = 7; i >= 0; i--)
			{
				int val = ((c & (1 << i)) != 0);
				if (val == 1)
				{
					nowNode = nowNode->rchild;
				}
				else
				{
					nowNode = nowNode->lchild;
				}
				if (nowNode != nullptr && nowNode->c != 0)
				{
					output << nowNode->c;
					nowNode = newRoot;
				}
			}
			pos_now = input.tellg();
		}
		input.read((char*)&c, sizeof(c));
		for (int i = 7; i > remain; i--)
		{
			int bitValue = ((c & (1 << i)) != 0);
			if (bitValue == 1)
			{
				nowNode = nowNode->rchild;
			}
			else
			{
				nowNode = nowNode->lchild;
			}
			if (nowNode != nullptr && nowNode->c != 0)
			{
				output << nowNode->c;
				nowNode = newRoot;
			}
		}
		input.close();
		infile.close();
		output.close();
		return;
	}

	void destroy(Node* nowNode)  //清除二叉树
	{
		if (nowNode == nullptr)
			return;
		destroy(nowNode->lchild);
		destroy(nowNode->rchild);
		delete nowNode;
	}

	void close()  //清理内存
	{
		destroy(root);
		delete[] code;
		root = nullptr;
		code = nullptr;
		return;
	}
};

int main()
{
	//读入文件，记录词频
	fstream infile;
	infile.open("inputfile1.txt");
	while (!infile.eof())
	{
		char c;
		infile.get(c);
		freq[c]++;
	}
	infile.close();

	Huffman* h = new Huffman();
	h->generateHuffman(freq);
	h->coding();
	h->decoding();
	h->close();

	system("pause");
	return 0;
}
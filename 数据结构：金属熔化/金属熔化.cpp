#include<iostream>
using namespace std;
int N = 0;  
int num_metal = 0;      //���������
int t = 0;              //ʱ��
char** originalCh;  //�����������
int ** status;   
void init(bool& illegal, int N, char** arr) 
{
	cout << "�������ʼ״̬��" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cin >> arr[i][j];
			if (arr[i][j] != '#' && arr[i][j] != '_')
			{
				cout << "�������" << endl;
				illegal = true;
				return;
			}
		}
	}
}

void filt(char** arr, int i, int j)
{
	if (i > 0 && arr[i - 1][j] == '.')
	{
		arr[i - 1][j] = '_';
		filt(arr, i - 1, j);
	}
	if (i < N - 1 && arr[i + 1][j] == '.')
	{
		arr[i + 1][j] = '_';
		filt(arr, i + 1, j);
	}
	if (j > 0 && arr[i][j - 1] == '.')
	{
		arr[i][j - 1] = '_';
		filt(arr, i, j - 1);
	}
	if (j < N - 1 && arr[i][j + 1] == '.')
	{
		arr[i][j + 1] = '_';
		filt(arr, i, j + 1);
	}
	status[i][j] = 1;
}

void process(char** arr, int N)
{ 
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (arr[i][j] == '*')
			{
				arr[i][j] = '_';  //���пձ������        
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (arr[i][j] == '_' && i > 0 && j > 0 && i < N - 1 && j < N - 1)
			{
				int n = 0; //������Χס�ı���
				int x1 = i, y1 = j, x2 = i, y2 = j;
				while (x1 >= 0)
				{
					if (arr[x1][j] == '#')
					{
						n++;
						break;
					}
					x1--;
				}
				while (y1 >= 0)
				{
					if (arr[i][y1] == '#')
					{
						n++;
						break;
					}
					y1--;
				}
				while (x2 <= N - 1)
				{
					if (arr[x2][j] == '#')
					{
						n++;
						break;
					}
					x2++;
				}
				while (y2 <= N - 1)
				{
					if (arr[i][y2] == '#')
					{
						n++;
						break;
					}
					y2++;
				}
				if (n == 4)
					arr[i][j] = '.'; //���ֿղ��ֺ�Һ��
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (arr[i][j] == '_' && status[i][j] == 0)
			{
				filt(arr, i, j);
			}
			status[i][j] = 0;
		}
	}
}

void melt(char** arr, int N) //numΪ�����������
{
	while (true)
	{
		int origin = num_metal;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				if (arr[i][j] == '#')
				{
					int current = 0;
					if (i > 0 && arr[i - 1][j] == '_')
						current++;
					if (i < N - 1 && arr[i + 1][j] == '_')
						current++;
					if (j > 0 && arr[i][j - 1] == '_')
						current++;
					if (j < N - 1 && arr[i][j + 1] == '_')
						current++;
					if (current >= 2)
					{
						arr[i][j] = ',';  //���ܺ�ǰ1s��Һ�����
						num_metal--;
					}
				}
			}
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (arr[i][j] == ',')	
					arr[i][j] = '_';
		if (num_metal == origin)
			return;
		t++;
		process(arr, N);
	}
}
int main()
{
	cout << "������ÿ���ߵĸ�������" << endl;
	cin >> N;
	originalCh = new char* [N];
	status = new int* [N];
	for (int i = 0; i < N; i++)
	{
		originalCh[i] = new char[N];
		status[i] = new int[N];
	}
	bool illegal = false;
	init(illegal, N, originalCh);
	while (illegal)
	{
		init(illegal, N, originalCh);
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (originalCh[i][j] == '#')
				num_metal++;
			status[i][j] = 0;
		}
	}
	process(originalCh, N);
	melt(originalCh, N);
	if (num_metal != 0)
		cout << "�޷�ȫ���ڻ�!" << endl;
	else
		cout << t << endl;
	for (int i = 0; i < N; i++)
	{
		delete[] originalCh[i];
		delete[] status[i];
	}
	delete[] originalCh;
	delete[] status;
	return 0;
}
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
int M, N, K;
int CompareString(char* arr1, char* arr2)
{
	for (int i = 0; i < strlen(arr1); i++)
	{
		if (arr1[i] < arr2[i])
			return -1;
		else if (arr1[i] > arr2[i])
			return 1;
		else
			continue;
	}
	return 0;
}
//����
void Process(char** arr, int start, int last)
{
	if (start > last)
	{
		return;
	}
	int index = start;
	int i = start, j = last;
	while (i != j)
	{
		while (CompareString(arr[j], arr[index]) == 1 && i < j)
		{
			j--;
		}
		while (CompareString(arr[i], arr[index]) != 1 && i < j)
		{
			i++;
		}
		if (i < j)
		{
			swap(arr[i], arr[j]);
		}
	} //��ʱi=j
	swap(arr[i], arr[index]);
	Process(arr, start, i - 1);
	Process(arr, i + 1, last);
}

int main()
{
	char** arr = { NULL };
	cout << "�������·��������" << endl;
	cin >> N; 
	cout << "�������·��������" << endl;
	cin >> M;
	cout << "������һ��" << N << "��" << M << "�еľ���" << endl;
	arr = new char*[N];
	for (int i = 0; i < N; i++)
	{
		arr[i] = new char[M + 1];
		for (int j = 0; j < M; j++)
		{
			cin >> arr[i][j];
		}
		arr[i][M] = '\0';
	}
	cout << "�����뷴ת������" << endl;
	cin >> K;
	Process(arr, 0, N - 1);

	//ͳ����С�ַ����е�0����
	int* num_zero = new int[N] {0};
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (arr[i][j] == '0')
				num_zero[i]++;
		}
	}

	//ͳ����ͬ����
	int* num_sameline = new int[N] {0};
	for (int i = 0; i < N - 1; i++)
	{
		if (num_zero[i] % 2 != K % 2 || K >= num_zero[i])  //K��num_zero��ż�Բ�ͬ���ܵ�ͨ
		{
			continue;
		}
		num_sameline[i] = 1;
		int j = 1;
		while ((i + j) < N && CompareString(arr[i], arr[i + j]) == 0)
		{
			num_sameline[i]++;
			j++;
		}
		i += j - 1;
	}
	sort(num_sameline, num_sameline + N);
	cout << "���ͨ��Ϊ��" << num_sameline[N - 1] << endl;
	for (int i = 0; i < N; i++)
		delete[] arr[i];
	delete[] arr;
	system("pause");
	return 0;
}
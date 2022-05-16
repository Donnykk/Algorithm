#include<iostream>
#include<cstring>
#include<string>
#include<cmath>
using namespace std;
int func_1(string str1, string str2, int k) //时间复杂度太高
{
	int len_1 = str1.size();
	int len_2 = str2.size();
	if (len_1 == 0 && len_2 == 0)
		return 0;
	if (len_1 == 0 && len_2 != 0)
		return len_2 * k;
	if (len_1 != 0 && len_2 == 0)
		return len_1 * k;
	//递归求解
	int num1 = abs(str1[0] - str2[0]) + func_1(str1.substr(1, len_1 - 1), str2.substr(1, len_2 - 1), k);
	return  min(num1, min(func_1(str1.substr(1, len_1 - 1), str2, k) + k, func_1(str1, str2.substr(1, len_2 - 1), k) + k));
}

int func_2(string str1, string str2, int k) //m*n二维数组空间复杂度太高
{
	int len_1 = str1.size();
	int len_2 = str2.size();
	int** val = new int* [len_1 + 1];
	for (int i = 0; i <= len_1; i++)
		val[i] = new int[len_2 + 1]{ 0 };
	//初始化空字符串距离
	for (int i = 0; i <= len_1; i++)
		val[i][0] = i * k;
	for (int i = 0; i <= len_2; i++)
		val[0][i] = i * k;
	//递归求解
	for (int i = 1; i <= len_1; i++)
	{
		for (int j = 1; j <= len_2; j++)
		{
			int num = val[i - 1][j - 1] + abs(str1[i - 1] - str2[j - 1]);
			val[i][j] = min(num, min(val[i - 1][j] + k, val[i][j - 1] + k));
		}
	}
	int distance = val[len_1][len_2];
	for (int i = 0; i <= len_1; i++)
		delete[]val[i];
	delete[]val;
	return distance;
}

int func(string str1, string str2, int k) //分治算法
{
	int len_1 = str1.size();
	int len_2 = str2.size();
	int** val = new int* [len_1 + 1];
	for (int i = 0; i <= len_1; i++)
		val[i] = new int[2];
	//初始化空字符串距离
	for (int i = 0; i <= len_1; i++)
		val[i][0] = i * k;
	//递归求解
	for (int i = 1; i <= len_2; i++)
	{
		val[0][1] = i * k;
		for (int j = 1; j <= len_1; j++)
		{
			int num = val[j - 1][0] + abs(str1[j - 1] - str2[i - 1]);
			val[j][1] = min(num, min(val[j - 1][1] + k, val[j][0] + k));
		}
		for (int j = 0; j <= len_1; j++)
			val[j][0] = val[j][1];
	}
	int distance = val[len_1][1];
	for (int i = 0; i <= len_1; i++)
		delete[]val[i];
	delete[]val;
	return distance;
}

int main()
{
	string str1, str2;
	int k = 0, distance = 0;
	cin >> str1 >> str2 >> k;
	distance = func(str1, str2, k);
	cout << distance << endl;
	system("pause");
	return 0;
}
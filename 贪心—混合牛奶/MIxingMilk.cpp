#include<iostream>
using namespace std;
void sort_Farmer(int **arr, int num) 
{
	for (int i = 0; i < num; i++)
	{
		int min = arr[i][0];
		for (int j = i + 1; j < num; j++)
		{
			if (arr[j][0] < min)
			{
				min = arr[j][0];
				int temp = arr[j][1];
				arr[j][0] = arr[i][0];
				arr[j][1] = arr[i][1];
				arr[i][0] = min;
				arr[i][1] = temp;
			}
		}
	}
}

int main()
{
	int total = 0, n = 0;
	cin >> total >> n;
	int** arr = new int*[n];
	for (int i = 0; i < n; i++)
	{
		arr[i] = new int[2];
		cin >> arr[i][0] >> arr[i][1];  //单价，产量
	}
	sort_Farmer(arr, n);
	int count = 0;
	int cost = 0;
	int i = 0;
	if (n == 0) 
	{
		cout << 0;
		return 0;
	}
	while (true)
	{
		if (i < n)
		{
			if (count + arr[i][1] >= total)
			{
				cost += (total - count) * arr[i][0];
				break;
			}
			else
			{
				count += arr[i][1];
				cost += arr[i][0] * arr[i][1];
				i++;
			}
		}
		else 
		{
			cout << "无法完成！" << endl;
			return 0;
		}
	}
	cout << cost << endl;
	system("pause");
	return 0;
}
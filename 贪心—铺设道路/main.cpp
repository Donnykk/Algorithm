#include<iostream>
using namespace std;
int main()
{
	int n; //道路长度
	cin >> n;
	int* depth = new int[n]; //下陷深度
	for (int i = 0; i < n; i++)
		cin >> depth[i];
	int time_cost = depth[0];
	for (int i = 0; i < n - 1; i++)
	{
		if (depth[i] < depth[i + 1])
		{
			time_cost += depth[i + 1] - depth[i];
		}
	}
	cout << time_cost << endl;
	system("pause");
	return 0;
}

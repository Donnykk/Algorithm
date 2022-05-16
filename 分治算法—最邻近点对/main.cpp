#include<iostream>
#include<iomanip>
#include<algorithm>
#include<math.h>
using namespace std;
class Node 
{
public:
	float x, y;
	Node(float x, float y) 
	{
		this->x = x;
		this->y = y;
	}
};

bool x_Cmp(Node* n1, Node* n2)
{
	return n1->x < n2->x;
}

float getDistance(Node* n1, Node* n2)
{
	float xDis = n1->x - n2->x;
	float yDis = n1->y - n2->y;
	return (xDis * xDis + yDis * yDis);
}

float func(Node** points, int left, int right)
{
	if (right - left == 1)  //点集中只剩2个点
		return getDistance(points[left], points[right]); 
	if (right - left == 2) //点集中只剩3个点，比较即可
	{
		float dis_1 = getDistance(points[left], points[left + 1]);
		float dis_2 = getDistance(points[left], points[right]);
		float dis_3 = getDistance(points[left + 1], points[right]);
		if (dis_1 <= dis_2 && dis_1 <= dis_3)
			return dis_1;
		if (dis_2 <= dis_1 && dis_2 <= dis_3)
			return dis_2;
		if (dis_3 <= dis_2 && dis_3 <= dis_1)
			return dis_3;
	}
	float distance_left = 0, distance_right = 0;
	int middle = (right + left) / 2;
	distance_left = func(points, left, middle);
	distance_right = func(points, middle + 1, right);
	float distance = min(distance_left, distance_right);
	int* arr = new int[right - left + 1];
	int k = 0;
	for (int i = left; i <= right; i++)
	{
		if (abs(points[i]->x - points[middle]->x) <= distance)
			arr[k++] = i;
	}
	for (int i = 0; i < k - 1; i++)
	{
		float min_y = points[arr[i]]->y;
		for (int j = i + 1; j < k; j++)
		{
			if (points[arr[j]]->y < min_y)
			{
				min_y = points[arr[j]]->y;
				swap(arr[i], arr[j]);
			}
		}
	}
	for (int i = 0; i < k; i++)
	{
		for (int j = i + 1; j < k && (points[j]->y - points[i]->y) <= distance; j++)
		{
			distance = min(distance, getDistance(points[arr[i]], points[arr[j]]));        
		}        
	}                
	return distance;
}

int main()
{
	int n; //点的数目
	cin >> n;
	Node** points = new Node * [n];
	for (int i = 0; i < n; i++)
	{
		float x, y;
		cin >> x >> y;
		points[i] = new Node(x, y);
	}
	sort(points, points + n, x_Cmp); //按x坐标排序
	cout << fixed << setprecision(2) << func(points, 0, n - 1) << endl; //输出2两位小数
	system("pause");
	return 0;
}
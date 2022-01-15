#include<iostream>
#include<algorithm>
#include<ctime>
using namespace std;
int arr[10000] = { 0 };
//判断排序是否成功
bool sortSucceed(int* result, int n)
{
	sort(arr, arr + n);
	for (int i = 0; i < n; i++) 
		if (arr[i] != result[i])
			return false;
	return true;
}

//插入排序 
void insertSort(int* arr, int n)
{
	int num_Compare = 0;
	int num_Swap = 0;
	for (int i = 0; i < n - 1; i++)
	{
		int end = i;
		int temp = arr[end + 1];
		while (end >= 0)
		{
			if (temp < arr[end])
			{
				arr[end + 1] = arr[end];
				end--;
				num_Compare++;
				num_Swap++;
			}
			else
			{
				num_Compare++;
				break;
			}
		}
		arr[end + 1] = temp;
	}
	if (sortSucceed(arr, n))
		cout << "排序成功!" << endl;
	cout << "插入排序数据比较次数为：" << num_Compare << endl
		<< "数据交换次数为：" << num_Swap << endl << "----------------------" << endl;
	return;
}

//选择排序 
void selectSort(int* arr, int n)
{
	int num_Compare = 0;
	int num_Swap = 0;
	for (int i = 0; i < n - 1; i++)
	{
		int min_index = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[min_index] > arr[j])
			{
				min_index = j;
			}
			num_Compare++;
		}
		if (min_index != i)
		{
			num_Swap++;
			swap(arr[i], arr[min_index]);
		}
	}
	if (sortSucceed(arr, n))
		cout << "排序成功!" << endl;
	cout << "选择排序数据比较次数为：" << num_Compare << endl
		<< "数据交换次数为：" << num_Swap << endl << "----------------------" << endl;
	return;
}

//堆排
void Down(int* arr, int i, int n, int& num_Compare, int& num_Swap)
{
	int parent = i;
	int child = 2 * i + 1;
	while (child < n)
	{
		if (child < n - 1 && arr[child] < arr[child + 1])
		{
			child++;
		}
		if (arr[parent] < arr[child])
		{
			swap(arr[parent], arr[child]);
			num_Swap++;
			parent = child;
		}
		num_Compare += 2;
		child = child * 2 + 1;
	}
}
void heapSort(int* arr, int n)
{
	int num_Compare = 0;
	int num_Swap = 0;
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		Down(arr, i, n, num_Compare, num_Swap);
	}
	for (int i = n - 1; i > 0; i--)
	{
		swap(arr[0], arr[i]);
		num_Swap++;
		Down(arr, 0, i, num_Compare, num_Swap);
	}
	if (sortSucceed(arr, n))
		cout << "排序成功!" << endl;
	cout << "堆排数据比较次数为：" << num_Compare << endl
		<< "数据交换次数为：" << num_Swap << endl << "----------------------" << endl;
	return;
}

//快排
void Process(int* arr, int start, int last, int& num_Compare, int& num_Swap)
{
	if (start > last)
	{
		return;
	}
	int index = start;
	int i = start, j = last;
	while (i != j)
	{
		while (arr[j] > arr[index] && i < j)
		{
			num_Compare++;
			j--;
		}
		num_Compare++;
		while (arr[i] <= arr[index] && i < j)
		{
			num_Compare++;
			i++;
		}
		num_Compare++;
		if (i < j)
		{
			swap(arr[i], arr[j]);
			num_Swap++;
		}
	} //此时i=j
	swap(arr[i], arr[index]);
	num_Swap++;
	Process(arr, start, i - 1, num_Compare, num_Swap);
	Process(arr, i + 1, last, num_Compare, num_Swap);
}
void quickSort(int* arr, int n)
{
	int num_Compare = 0;
	int num_Swap = 0;
	Process(arr, 0, n - 1, num_Compare, num_Swap);
	if (sortSucceed(arr, n))
		cout << "排序成功!" << endl;
	cout << "快排数据比较次数为：" << num_Compare << endl
		<< "数据交换次数为：" << num_Swap << endl << "----------------------" << endl;
	return;
}

//希尔排序
void shellSort(int* arr, int n)
{
	int num_Compare = 0;
	int num_Swap = 0;
	int gap = n;
	while (gap > 1)
	{
		gap = gap / 2;
		for (int i = 0; i < n - gap; i++)
		{
			int end = i;
			int tem = arr[end + gap];
			while (end >= 0)
			{
				if (tem < arr[end])
				{
					arr[end + gap] = arr[end];
					end -= gap;
					num_Compare++;
					num_Swap++;
				}
				else
				{
					num_Compare++;
					break;
				}
			}
			arr[end + gap] = tem;
		}
	}
	if (sortSucceed(arr, n))
		cout << "排序成功!" << endl;
	cout << "希尔排序数据比较次数为：" << num_Compare << endl
		<< "数据交换次数为：" << num_Swap << endl << "----------------------" << endl;
	return;
}

int main()
{
	srand((int)time(NULL));
	for (int i = 0; i < 10000; i++)
	{
		arr[i] = rand() % 100000;
	}
	int arr1[10000] = { 0 };
	int arr2[10000] = { 0 };
	int arr3[10000] = { 0 };
	int arr4[10000] = { 0 };
	int arr5[10000] = { 0 };
	for (int i = 0; i < 10000; i++)
	{
		arr1[i] = arr[i];
		arr2[i] = arr[i];
		arr3[i] = arr[i];
		arr4[i] = arr[i];
		arr5[i] = arr[i];
	}
	insertSort(arr1, 10000);
	selectSort(arr2, 10000);
	heapSort(arr3, 10000);
	quickSort(arr4, 10000);
	shellSort(arr5, 10000);

	system("pause");
	return 0;
}
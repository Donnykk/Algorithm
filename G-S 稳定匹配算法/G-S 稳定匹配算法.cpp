#include <iostream>
using namespace std;
int main()
{
    //initialize
    int n = 0;
    cin >> n;
    int** man_list = new int*[n];
    int** woman_list = new int*[n];
    bool* man_single = new bool[n];
    bool* woman_single = new bool[n];
    int* couple = new int[n] { 0 };
    int* couple_ = new int[n] { 0 };
    int* order = new int[n] { 0 };
    for (int i = 0; i < n; i++) {
        man_single[i] = true;
        man_list[i] = new int[n];
        for (int j = 0; j < n; j++) {
            cin >> man_list[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        woman_single[i] = true;
        woman_list[i] = new int[n];
        for (int j = 0; j < n; j++) {
            cin >> woman_list[i][j];
        }
    }
    //女生排名矩阵重排
    int** woman_list_ = new int* [n];
    for (int i = 0; i < n; i++) {
        woman_list_[i] = new int[n] { 0 };
        for (int j = 0; j < n; j++) {
            int num = woman_list[i][j];
            woman_list_[i][num - 1] = j;
        }
    }
    //G-S
    for (int t = 0; t < n + n; t++) {
        for (int i = 0; i < n; i++) {
            if (man_single[i]) {
                int temp = man_list[i][order[i]++] - 1;
                if (woman_single[temp]) {
                    couple[temp] = i;
                    couple_[i] = temp;
                    woman_single[temp] = false;
                    man_single[i] = false;
                }
                else {
                    int num = couple[temp];
                    if (woman_list_[temp][num] > woman_list_[temp][i]) {
                        couple[temp] = i;
                        couple_[i] = temp;
                        man_single[i] = false;
                        man_single[num] = true;
                    }
                    else {
                        continue;
                    }
                }
            }
            else {
                continue;
            }
        }
    }

    for (int i = 0; i < n; i++)
        cout << couple_[i] + 1 << " ";
    cout << endl;

    //释放空间
    for (int i = 0; i < n; i++) {
        delete[]man_list[i];
        delete[]woman_list[i];
        delete[]woman_list_[i];
    }
    delete[]man_list;
    delete[]woman_list;
    delete[]man_single;
    delete[]woman_single;
    delete[]woman_list_;
    system("pause");
    return 0;
}

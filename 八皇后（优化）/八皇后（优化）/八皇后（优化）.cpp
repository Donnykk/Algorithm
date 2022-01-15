#include<iostream>
#include<cmath>
using namespace std;
int vis[15];   //看看有没有被用过
int ans[15];   //记录下当前第cur行皇后的位置
int n;
int num;

bool _check(int x)    //第x行与前面所有皇后有没有在一条斜线上
{
    for (int i = 1; i < x; i++)
    {
        if (x - i == abs(ans[x] - ans[i]))   //dx=|dy| 如果斜率为正负一肯定在一条斜线上，x-i肯定是正的哦
            return true;
    }
    return false;

}
void dfs(int cur)
{
    if (_check(cur - 1))   //剪枝，判断前面摆的皇后可不可以，不可以就return
        return;
    if (cur > n)
    {
        num++;
        for (int i = 1; i <= n && num <= 3; i++)   //  控制输入前三次
        {
            cout << ans[i] << " ";
            if (i == n)
                cout << endl;
        }
        return;
    }
    for (int i = 1; i <= n; i++) 
    {

        if (!vis[i])
        {
            vis[i] = 1;
            ans[cur] = i;
            dfs(cur + 1);
            vis[i] = 0;
        }
    }
}

int main()
{
    cin >> n;
    dfs(1);
    cout << num;
    return 0;
}
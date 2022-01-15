#include<iostream>
#include<cmath>
using namespace std;
int vis[15];   //������û�б��ù�
int ans[15];   //��¼�µ�ǰ��cur�лʺ��λ��
int n;
int num;

bool _check(int x)    //��x����ǰ�����лʺ���û����һ��б����
{
    for (int i = 1; i < x; i++)
    {
        if (x - i == abs(ans[x] - ans[i]))   //dx=|dy| ���б��Ϊ����һ�϶���һ��б���ϣ�x-i�϶�������Ŷ
            return true;
    }
    return false;

}
void dfs(int cur)
{
    if (_check(cur - 1))   //��֦���ж�ǰ��ڵĻʺ�ɲ����ԣ������Ծ�return
        return;
    if (cur > n)
    {
        num++;
        for (int i = 1; i <= n && num <= 3; i++)   //  ��������ǰ����
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
Problem 1：
(1) 遞迴函數

```
#include <iostream>
using namespace std;

long long Ackermann(long long m, long long n)
{
    if (m == 0)
        return n + 1;

    else if (n == 0)
        return Ackermann(m - 1, 1);

    else
        return Ackermann(m - 1, Ackermann(m, n - 1));
}

int main()
{
    long long m, n;

    cout << "Enter m and n: ";
    cin >> m >> n;

    cout << "A(" << m << ", " << n << ") = "
         << Ackermann(m, n) << endl;

    return 0;
}
```
(2) 非遞迴函數
```
#include <iostream>
#include <stack>
using namespace std;

long long AckermannNonRecursive(long long m, long long n)
{
    stack<long long> s;

    s.push(m);

    while (!s.empty())
    {
        m = s.top();
        s.pop();

        if (m == 0)
        {
            n = n + 1;
        }
        else if (n == 0)
        {
            n = 1;
            s.push(m - 1);
        }
        else
        {
            n = n - 1;

            s.push(m - 1);
            s.push(m);
        }
    }

    return n;
}

int main()
{
    long long m, n;

    cout << "Enter m and n: ";
    cin >> m >> n;

    cout << "A(" << m << ", " << n << ") = "
         << AckermannNonRecursive(m, n) << endl;

    return 0;
}
```
Problem 2：

```
#include <iostream>
#include <vector>
using namespace std;

void powerSet(vector<char>& S, int index, vector<char>& current)
{
    if (index == S.size())
    {
        cout << "{ ";

        for (char x : current)
            cout << x << " ";

        cout << "}" << endl;
        return;
    }

    // 不選目前的元素
    powerSet(S, index + 1, current);

    // 選目前的元素
    current.push_back(S[index]);

    powerSet(S, index + 1, current);

    // 回到上一層之前，把元素移除
    current.pop_back();
}

int main()
{
    vector<char> S = {'a', 'b', 'c'};
    vector<char> current;

    powerSet(S, 0, current);

    return 0;
}
```

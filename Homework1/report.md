# Problem 1：
## (1) 遞迴函數
### 解題說明
這題的 Ackermann 函數本身就是用遞迴的方式來定義，所以我直接按照題目的三個條件去寫程式。
當 m = 0 時直接回傳 n + 1；如果 n = 0，就呼叫 A(m-1,1)；其他情況就再呼叫 A(m-1, A(m,n-1))。
因為函式裡面會再呼叫自己，上課有講到叫Direct Recursion。

### 解題策略
我使用 if / else if / else 分別判斷題目的三種情況，並把數學公式直接轉成 C++ 程式。
1. m == 0 時，直接回傳 n + 1。
2. n == 0 時，呼叫 Ackermann(m - 1, 1)。
3. 其他情況就呼叫 Ackermann(m - 1, Ackermann(m, n - 1))。
### 程式製作
```cpp
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
## (2) 非遞迴函數
### 解題說明
把原本的遞迴 Ackermann 函數改成非遞迴的方式。
因為遞迴函數在執行時會記住還沒有完成的函數呼叫，所以我使用 stack 來自己保存這些資料，讓程式不需要再呼叫自己。
好像有提到遞迴的缺點之一是會使用比較多記憶體，而遞迴本身是透過函式呼叫來進行。

### 解題策略
我使用 stack 來模擬原本遞迴呼叫的過程。
1. 先把 m 放進 stack。
2. 每次從 stack 取出一個 m 來判斷。
3. 如果 m == 0，就讓 n = n + 1。
4. 如果 n == 0，就把 m - 1 放進 stack，並把 n 設成 1。
5. 其他情況則把還需要處理的值放進 stack，繼續計算。
6. 當 stack 裡沒有資料時，代表計算完成，最後回傳 n。

### 程式製作
```cpp
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
# Problem 2：
### 解題說明
要找出集合 S 的所有子集合，也就是 Power Set。每一個元素都只有兩種情況，就是「選這個元素」或「不選這個元素」，所以可以使用遞迴的方式，把每一種可能都跑一次。
這裡也是函式自己再呼叫自己，所以和上課講的 Direct Recursion 是一樣的概念。

### 解題策略
使用 index 來表示目前處理到集合中的第幾個元素，current 用來存目前已經選到的元素。
1. 先選擇「不放入目前元素」，遞迴處理下一個元素。
2. 再選擇「放入目前元素」，把元素加入 current。
3. 再次遞迴處理下一個元素。
4. 回到上一層時，使用 pop_back() 把剛剛加入的元素移除。
5. 當 index == S.size() 時，代表所有元素都處理完，就輸出目前的子集合。

### 程式製作
```cpp
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

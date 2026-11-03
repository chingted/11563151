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
有提到遞迴的缺點之一是會使用比較多記憶體，而遞迴本身是透過函式呼叫來進行。

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
要找出集合 S 的所有子集合，也就是 Power Set。
我使用遞迴的方式來產生不同大小的子集合，先產生 0 個元素的集合，再依序產生 1 個、2 個到全部元素的集合。
generate 函式裡面會再呼叫自己，所以和上課講的 Direct Recursion 是一樣的概念。

### 解題策略
使用 k 表示目前要產生幾個元素的子集合，current 用來存目前已經選到的元素。
1. 先從 k = 0 開始，依序產生 0、1、2 到全部元素的子集合。
2. 使用 start 表示目前可以從集合的哪個位置開始選擇。
3. 每次把目前的元素放入 current，再遞迴選擇下一個元素。
4. 當 current.size() == k 時，代表已經選到需要的元素數量，就輸出目前的子集合。
5. 回到上一層時使用 pop_back() 移除剛剛加入的元素，再繼續選擇其他元素。

### 程式製作
```cpp
#include <iostream>
#include <vector>
using namespace std;

void generate(vector<char>& S, int start, int k, vector<char>& current)
{
    // 如果已經選到 k 個元素，就輸出這個子集合
    if (current.size() == k)
    {
        cout << "{ ";

        for (char x : current)
            cout << x << " ";

        cout << "}" << endl;
        return;
    }

    // 從 start 開始選擇元素
    for (int i = start; i < S.size(); i++)
    {
        current.push_back(S[i]);          // 選擇目前元素
        generate(S, i + 1, k, current);  // 遞迴選下一個元素
        current.pop_back();               // 回到上一層
    }
}

void powerSet(vector<char>& S)
{
    vector<char> current;

    // 依照子集合大小 0、1、2、3... 產生
    for (int k = 0; k <= S.size(); k++)
    {
        generate(S, 0, k, current);
    }
}

int main()
{
    vector<char> S = {'a', 'b', 'c'};

    powerSet(S);

    return 0;
}
```

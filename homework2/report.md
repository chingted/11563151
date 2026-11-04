# 11563151

**Homework 2：Polynomial**

## 1.解題說明

### 問題描述
這次作業主要是使用 C++ 的 class 來實作多項式運算，題目分成兩個部分。

第一題需要建立 Polynomial 類別，完成以下功能：

Polynomial()：建立一個多項式。

Add()：計算兩個多項式相加的結果。

Mult()：計算兩個多項式相乘的結果。

Eval()：將指定的 x 值代入多項式進行計算。

第二題則是使用運算子重載，讓程式可以透過 >> 輸入多項式，並使用 << 輸出多項式。

```cpp
Polynomial p;
cin >> p;
cout << p;
```

### 解題策略
我使用 Term 類別來記錄每一項的係數和指數，再使用 Polynomial 類別管理多項式。

程式的實作步驟如下：

建立資料結構：使用動態陣列儲存多項式的非零項，並用 capacity 和 terms 記錄陣列容量及目前的項數。

新增多項式的項目：使用 newTerm() 將項目按照指數由大到小排列。如果遇到相同指數，就把係數合併。如果空間不夠，就將陣列容量擴大兩倍。

實作加法：依序比較兩個多項式的指數，相同指數的係數相加，不同指數的項目則直接加入結果。

實作乘法：使用兩層 for 迴圈，將兩個多項式的每一項互相相乘，再合併相同指數的項目。

實作代入計算：使用 for 迴圈和 pow() 計算每一項的數值，最後將結果加總。

處理輸入與輸出：透過 operator>> 讀取係數和指數，再利用 operator<< 將多項式按照一般數學式的方式輸出。
## 2.程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

// 儲存多項式的其中一項
class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial {
private:
    Term* termArray; // 儲存多項式的動態陣列
    int capacity;    // 陣列容量
    int terms;       // 目前有幾個非零項

    // 陣列空間不夠時，容量增加兩倍
    void expand() {
        if (terms < capacity)
            return;

        int newCapacity = capacity * 2;
        Term* temp = new Term[newCapacity];

        for (int i = 0; i < terms; i++)
            temp[i] = termArray[i];

        delete[] termArray;
        termArray = temp;
        capacity = newCapacity;
    }

    // 將一項加入陣列最後面
    void appendTerm(float c, int e) {
        if (c == 0)
            return;

        expand();

        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
    }

public:
    // 建構子
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    // 複製建構子
    Polynomial(const Polynomial& other)
        : capacity(other.capacity), terms(other.terms) {

        termArray = new Term[capacity];

        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
    }

    // 指派運算子
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other)
            return *this;

        Term* temp = new Term[other.capacity];

        for (int i = 0; i < other.terms; i++)
            temp[i] = other.termArray[i];

        delete[] termArray;

        termArray = temp;
        capacity = other.capacity;
        terms = other.terms;

        return *this;
    }

    // 解構子
    ~Polynomial() {
        delete[] termArray;
    }

    // 新增一項，按照指數由大到小排列
    void newTerm(float theCoef, int theExp) {
        if (theCoef == 0 || theExp < 0)
            return;

        int pos = 0;

        while (pos < terms && termArray[pos].exp > theExp)
            pos++;

        // 如果指數相同，係數相加
        if (pos < terms && termArray[pos].exp == theExp) {
            termArray[pos].coef += theCoef;

            // 合併後係數為 0，就移除這一項
            if (termArray[pos].coef == 0) {
                for (int i = pos; i < terms - 1; i++)
                    termArray[i] = termArray[i + 1];

                terms--;
            }

            return;
        }

        expand();

        // 將後面的項目往右移
        for (int i = terms; i > pos; i--)
            termArray[i] = termArray[i - 1];

        termArray[pos].coef = theCoef;
        termArray[pos].exp = theExp;
        terms++;
    }

    // 第一題：多項式加法
    Polynomial Add(Polynomial poly) const {
        Polynomial result;

        int i = 0;
        int j = 0;

        while (i < terms && j < poly.terms) {

            if (termArray[i].exp > poly.termArray[j].exp) {
                result.appendTerm(
                    termArray[i].coef,
                    termArray[i].exp
                );
                i++;
            }
            else if (termArray[i].exp < poly.termArray[j].exp) {
                result.appendTerm(
                    poly.termArray[j].coef,
                    poly.termArray[j].exp
                );
                j++;
            }
            else {
                float c = termArray[i].coef
                    + poly.termArray[j].coef;

                result.appendTerm(c, termArray[i].exp);

                i++;
                j++;
            }
        }

        // 加入第一個多項式剩下的項
        while (i < terms) {
            result.appendTerm(
                termArray[i].coef,
                termArray[i].exp
            );
            i++;
        }

        // 加入第二個多項式剩下的項
        while (j < poly.terms) {
            result.appendTerm(
                poly.termArray[j].coef,
                poly.termArray[j].exp
            );
            j++;
        }

        return result;
    }

    // 第一題：多項式乘法
    Polynomial Mult(Polynomial poly) const {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {

                // 係數相乘
                float c = termArray[i].coef
                    * poly.termArray[j].coef;

                // 指數相加
                int e = termArray[i].exp
                    + poly.termArray[j].exp;

                result.newTerm(c, e);
            }
        }

        return result;
    }

    // 第一題：代入數值計算
    float Eval(float f) const {
        float result = 0;

        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * static_cast<float>(pow(f, termArray[i].exp));
        }

        return result;
    }

    // 第二題：重載 >>，輸入多項式
    friend istream& operator>>(istream& in, Polynomial& poly) {
        int count;

        if (!(in >> count))
            return in;

        if (count < 0) {
            in.setstate(ios::failbit);
            return in;
        }

        Polynomial temp;

        for (int i = 0; i < count; i++) {
            float c;
            int e;

            if (!(in >> c >> e))
                return in;

            if (e < 0) {
                in.setstate(ios::failbit);
                return in;
            }

            temp.newTerm(c, e);
        }

        poly = temp;

        return in;
    }

    // 第二題：重載 <<，輸出多項式
    friend ostream& operator<<(ostream& out, const Polynomial& poly) {

        if (poly.terms == 0) {
            out << 0;
            return out;
        }

        for (int i = 0; i < poly.terms; i++) {

            float c = poly.termArray[i].coef;
            int e = poly.termArray[i].exp;

            // 處理正負號
            if (i == 0) {
                if (c < 0)
                    out << '-';
            }
            else {
                if (c < 0)
                    out << " - ";
                else
                    out << " + ";
            }

            // 取得係數的絕對值
            float absCoef = (c < 0) ? -c : c;

            // x 的係數為 1 時可以省略
            if (e == 0 || absCoef != 1)
                out << absCoef;

            // 處理 x 和指數
            if (e > 0) {
                out << 'x';

                if (e > 1)
                    out << '^' << e;
            }
        }

        return out;
    }
};

int main() {
    Polynomial p, q;
    float x;

    // 輸入第一個多項式
    cout << "Input P: count, then coefficient and exponent for each term\n";

    if (!(cin >> p)) {
        cout << "Invalid P input\n";
        return 1;
    }

    // 輸入第二個多項式
    cout << "Input Q: count, then coefficient and exponent for each term\n";

    if (!(cin >> q)) {
        cout << "Invalid Q input\n";
        return 1;
    }

    // 輸入要代入的 x
    cout << "Input x for Eval:\n";

    if (!(cin >> x)) {
        cout << "Invalid x input\n";
        return 1;
    }

    // 顯示結果
    cout << "P(x) = " << p << '\n';
    cout << "Q(x) = " << q << '\n';

    cout << "P + Q = " << p.Add(q) << '\n';
    cout << "P * Q = " << p.Mult(q) << '\n';

    cout << "P(" << x << ") = " << p.Eval(x) << '\n';
    cout << "Q(" << x << ") = " << q.Eval(x) << '\n';

    return 0;
}
```
## 3.效能分析

假設第一個多項式有 (m) 項，第二個多項式有 (n) 項。

### 時間複雜度

Add()：(O(m+n))

加法會依序比較兩個多項式的指數，將相同指數的係數相加，所以時間複雜度為 $(O(m+n))$。

Mult()：(O(m^2n^2))

乘法使用兩層迴圈，讓每一項互相相乘。因為 newTerm() 還需要尋找插入位置、合併相同指數及移動陣列元素，所以最壞情況的時間複雜度為 $(O(m^2n^2))$。

Eval()：(O(m))

使用一層迴圈計算每一項的數值，再將結果加總。將 pow() 視為固定成本時，時間複雜度為 $(O(m))$。

### 空間複雜度

Add()：(O(m+n))

需要建立一個新的多項式來儲存相加的結果，最多有 $(m+n)$ 項。

Mult()：(O(mn))

乘法結果最多可能有 $(m\times n)$ 個不同指數的項目，因此需要 $(O(mn))$ 的空間。

Eval()：(O(1))

只需要使用變數儲存計算結果，不需要另外建立陣列。

## 4.測試與驗證
$$
P(x) = 3x^2 + 2x + 1
$$

$$
Q(x) = x - 1
$$

Input P: count, then coefficient and exponent for each term

然後我輸入

```text
3

3 2

2 1

1 0
```

Input Q: count, then coefficient and exponent for each term

輸入

```text
2

1 1

-1 0
```

Input x for Eval:

輸入

```text
2
```

這邊將x帶入後

顯示:

```text
P(x) = 3x^2 + 2x + 1

Q(x) = x - 1

P + Q = 3x^2 + 3x

P * Q = 3x^3 - x^2 - x - 1

P(2) = 17

Q(2) = 1
```

## 5.申論及開發報告

這次作業我使用 Term 類別來儲存多項式每一項的係數和指數，再用 Polynomial 類別管理這些資料。

(1) 資料結構的選擇

我使用動態陣列來存放多項式，因為一開始不一定知道多項式會有幾項。如果使用固定大小的陣列，可能會有空間不足的問題。

所以我讓 capacity 一開始設為 2，當空間不夠時，就把容量增加兩倍。另外，只儲存係數不為 0 的項目，避免浪費空間。

(2) 演算法的設計

我讓多項式按照指數由大到小排列，這樣在進行加法時，可以依序比較兩個多項式的指數。

乘法則使用兩層 for 迴圈，讓兩個多項式的每一項互相相乘，再使用 newTerm() 合併相同指數的項目。

Eval() 則是使用 for 迴圈，將每一項代入指定的 x 值，再把結果加總。

(3) 開發過程

在 Visual Studio 測試時，我遇到 Term 的 private 成員無法存取的錯誤。後來發現 operator<< 雖然寫在 Polynomial 裡面，但它是友誼函式，不是成員函式，所以需要另外宣告權限。

另外，pow() 回傳的型別是 double，也出現轉換成 float 的警告，後來使用 static_cast<float> 處理。

修改後，程式可以正常編譯，也能完成多項式的加法、乘法及代入計算。

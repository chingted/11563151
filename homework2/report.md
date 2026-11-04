# homework 2：Polynomial

## 1.解題說明

### 問題描述
這次作業要求實作一個Polynomial類別，並依題目提供的ADT與私有資料成員來設計程式。

每個多項式由多個 Term 組成，每個 Term 包含係數（coef）與指數（exp）。Polynomial 類別需要完成:相加,相乘,可將指定數值帶入運算,最後在輸出

Add：將兩個多項式相加。

Mult：將兩個多項式相乘。

Eval：將指定的數值代入多項式進行計算。

運算子多載：使用 >> 和 << 完成多項式的輸入與輸出。

```cpp
Polynomial p;
cin >> p;
cout << p;
```

### 解題策略
我使用 Term 類別儲存每一項的係數與指數，再使用動態陣列 termArray 來管理多項式的所有非零項。

建立 Polynomial 類別，使用 capacity 記錄陣列容量，terms 記錄目前的非零項數。

1.當陣列空間不足時，將 capacity 擴大兩倍，並將原本的資料複製到新陣列。

2.Add 使用相同指數的項目進行係數相加。

3.Mult 使用雙層迴圈，將兩個多項式的每一項互相相乘，係數相乘、指數相加，最後合併相同指數的項目。

4.Eval 使用迴圈逐項計算，將指定數值代入後加總結果。

5.使用 >> 讀取多項式資料，並使用 << 輸出計算結果。
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

這次程式主要有加法、乘法和代入計算三種功能。

加法 Add()：因為多項式已經按照指數排序，所以只要依序比較兩個多項式的項目，時間複雜度是 (O(m+n))。

乘法 Mult()：使用兩層 for 迴圈，讓兩個多項式的每一項互相相乘，再透過 newTerm() 合併相同指數。因為合併時還需要尋找和移動項目，所以最壞時間複雜度是 (O(m^2n^2))。

代入計算 Eval()：使用一層 for 迴圈，把每一項的計算結果加起來。若把 pow() 視為固定成本運算，時間複雜度是 (O(m))。

其中 (m) 和 (n) 分別代表兩個多項式的非零項數。

### 時間複雜度

### 空間複雜度

## 4.測試與驗證
$$
P(x) = 3x^2 + 2x + 1
$$

$$
Q(x) = x - 1
$$

Input P: count, then coefficient and exponent for each term

然後我輸入

3

3 2

2 1

1 0

Input Q: count, then coefficient and exponent for each term

輸入

2

1 1

-1 0

Input x for Eval:

輸入

2

這邊將x帶入後

顯示:

P(x) = 3x^2 + 2x + 1

Q(x) = x - 1

P + Q = 3x^2 + 3x

P * Q = 3x^3 - x^2 - x - 1

P(2) = 17

Q(2) = 1


## 5.申論及開發報告

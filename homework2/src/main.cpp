#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

// 儲存多項式的其中一項
class Term {
    friend class Polynomial;

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
            result += termArray[i].coef
                    * pow(f, termArray[i].exp);
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

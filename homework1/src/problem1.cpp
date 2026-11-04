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

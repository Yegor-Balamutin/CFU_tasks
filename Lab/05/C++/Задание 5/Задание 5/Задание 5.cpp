

#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    float x0, v0, t, xt, s, a = 9.8;
    cout << "Введите: x0, v0, t" << endl;
    cin >> x0 >> v0 >> t;
    cout << endl;
    xt = x0 + v0 * t + a * t * t / 2;
    s = abs(xt - x0);
    cout << "Объект преодолел расстояние: " << s;
    return 0;
}


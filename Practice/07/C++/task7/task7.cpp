
#include <iostream>
using namespace std;

int main()
{
    int k, x1, x2, x3, y1, y2, y3;
    double s, p, a, b, c;
    bool f;
    setlocale(LC_ALL, "Russian");
    cout << "Расчёт площади треугольника" << endl ;
    cout << "Выберете способ вычисления площади:" << endl ;
    cout << "1 - через длины сторон" << endl;
    cout << "2 - через координаты вершин" << endl;
    f = true;
    while (f)
    {
        cin >> k;
        if ((k == 1) || (k == 2)) {
            f = false;
        }
        else {
            cout << "Неверный ввод! Попробуйте ещё раз:" << endl;
        }
    }
    if (k == 1) {
        cout << "a = ";
        cin >> a;
        cout << "b = ";
        cin >> b;
        cout << "c = ";
        cin >> c;
        p = (a + b + c) / 2;
        s = sqrt(p * (p - a) * (p - b) * (p - c));
    }
    else {
        cout << "Введите x1 y1: ";
        cin >> x1 >> y1;
        cout << "Введите x2 y2: ";
        cin >> x2 >> y2;
        cout << "Введите x1 y1: ";
        cin >> x3 >> y3;
        s = abs((x1 - x3) * (y2 - y3) - (y1 - y3) * (x2 - x3)) / 2;
    }
    cout << "Площадь треугольника равна: " << s;
    return 0;
}
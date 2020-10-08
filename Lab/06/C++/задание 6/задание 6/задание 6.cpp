
#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    int a, b, c;
    float d, x, x1, x2;
    cout << "a = " ;
    cin >> a;
    cout << "b = " ;
    cin >> b;
    cout << "c = " ;
    cin >> c ;
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                cout << "x - любое число" ;
            }
            else {
                cout << "нет корней" ;
            }
        }
        else {
            x = -c / b ;
            cout << "x = " << x ;
        }
    }
    else {
        d = b * b - 4 * a * c;
        if (d > 0) {
            x1 = (-b - (sqrt(d))) / (2 * a);
            x2 = (-b + (sqrt(d))) / (2 * a);
            cout << "x1 = " << x1 << endl;
            cout << "x2 = " << x2;
        }
        else {
            if (d == 0) {
                x = -b / (2 * a);
                cout << "x = " << x;
            }
            else {
                cout << "нет действительных корней";
            }
        }
    }
    return 0;
}
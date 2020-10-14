
#include <iostream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    double a, b;
    char d; 
    cout << "Калькулятор\n";
    cin >> a >> d >> b;
    if (d == '+') {
        cout << (a + b);
    }
    else {
        if (d == '-') {
            cout << (a - b);
        }
        else {
            if (d == '*') {
                cout << (a * b);
            }
            else {
                if (d == '/') {
                    cout << (a / b);
                }
                else {
                    cout << "Неверный ввод";
                }
            }
        }
    }
    return 0;
}
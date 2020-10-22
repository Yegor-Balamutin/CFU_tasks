

#include <iostream>
using namespace std;

int main()
{
	int a, b, t, variant;  // Переменная variant обозначает, каким способом будут поменяны значения a и b
	variant = 0;
	cout << "a = "; cin >> a;
	cout << "b = "; cin >> b;
	if (variant == 0) {
		t = a;
		a = b;
		b = t;
	}
	else
	{
		a = a + b;
		b = a - b;
		a = a - b;
	}
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	return 0;
}


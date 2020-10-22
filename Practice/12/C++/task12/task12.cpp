#include<iostream>;

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int a, rez = 1;
	cout << "Рассчитать факториал числа: ";
	cin >> a;
	for (int i = 2; i <= a; i++) {
		rez = rez * i;
	}
	cout << "результат: " << rez;
	return 0;
}
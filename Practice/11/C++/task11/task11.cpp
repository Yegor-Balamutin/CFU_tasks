#include<iostream>;

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int a, b, rez = 1;
	cout << "Возвести число: ";
	cin >> a;
	cout << "в степень: ";
	cin >> b;
	for (int i = 1; i <= b; i++) {
		rez = rez * a;
	}
	cout << "результат: " << rez;
	return 0;
}
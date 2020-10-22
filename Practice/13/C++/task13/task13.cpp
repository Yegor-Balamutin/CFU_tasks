#include<iostream>;

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int a;
	bool rez = true;
	cout << "Определить, является ли число простым: ";
	cin >> a;
	for (int i = 2; i < a; i++) {
		if (a % i == 0) {
			rez = false;
			break;
		}
	}
	if (rez) {
		cout << "Число простое";
	}
	else {
		cout << "Число составное";
	}
	return 0;
}
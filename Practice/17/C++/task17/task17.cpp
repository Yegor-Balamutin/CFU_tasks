#include <iostream>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int n[37] { 0 };
	int k[37] { 0 };
	int red = 0, black = 0, s, max;
	cin >> s;
	while (s > 0)
	{
		n[s] = n[s] + 1;
		k[s] = 12;
		if ((s == 1) or (s == 3) or (s == 5) or (s == 7) or (s == 9) or (s == 12) or (s == 14) or (s == 18) or (s == 19) or (s == 21) or (s == 23) or (s == 25) or (s == 27) or (s == 30) or (s == 32) or (s == 34) or (s == 36)) {
			red++;
		}
		else {
			black++;
		}
		for (int i = 0; i < 37; i++) {
			k[i] = k[i] - 1;
		}
		max = 0;
		for (int i = 1; i < 37; i++) {
			if (n[i] > max) {
				max = n[i];
			}
		}
		cout << "Больше всего выпадали: ";
		for (int i = 1; i < 37; i++) {
			if (n[i] == max) {
				cout << i << " ";
			}
		}
		cout << endl << "Не выпадали последние 12 игр: ";
		for (int i = 1; i < 37; i++) {
			if (k[i] < 0) { cout << i << " "; }
		}
		cout << endl << "Выпало красных: " << red << endl;
		cout << "Выпало чёрных: " << black << endl << endl;
		cin >> s;
	}
	return 0;
}
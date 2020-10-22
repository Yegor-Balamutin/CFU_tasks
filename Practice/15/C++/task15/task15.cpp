#include <iostream>
#include <ctime>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	int s, a, x;
	bool f;
	s = 1;
	while (s == 1) {
		a = (rand() % 100) + 1;
		cout << "Загадано число от 1 до 100. Попробуйте угадать!\n";
		f = true;
		for (int i = 1; i <= 5; i++) {
			cout << i << " попытка: ";
			cin >> x;
			if (x == a) {
				cout << "Поздравляю, вы победили!\n";
				f = false;
				break;
			}
			else {
				if (x < a) {
					cout << "загаданное число больше\n";
				}
				else {
					cout << "загаданное число меньше\n";
				}
			}
		}
		if (f) {
			cout << "Вы проиграли! Было загадано: " << a << endl;
		}
		cout << "Хотите попробовать снова? (1 - Да)\n";
		cin >> s;
	}
	return 0;
}
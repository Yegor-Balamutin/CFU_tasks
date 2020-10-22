#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	vector<string> s;
	int k = 0, n;
	string s1;
	cout << "Количество билетов: ";
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cout << i << " билет: ";
		cin >> s1;
		if ((s1[0] == 'a') && (s1[4] == '5') && (s1[5] == '5') && (s1[6] == '6') && (s1[7] == '6') && (s1[8] == '1')) {
			k = k + 1;
			s.push_back(s1);
		}
	}
	if (k == 0) {
		cout << "Билетов Суслика нет.";
	}
	else {
		cout << "Билетов Суслика: " << k << endl;
		for (int i = 0; i <= k-1; i++) {
			cout << i + 1 << ": " << s[i] << endl;
		}
	}
	return 0;
}
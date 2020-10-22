#include<iostream>;
#include<math.h>;

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int h1, m1, h2, m2, t1, t2;
	char a;
	cout << "Встреча\n";
	cin >> h1 >> a >> m1;
	cin >> h2 >> a >> m2;
	t1 = h1 * 60 + m1;
	t2 = h2 * 60 + m2;
	if (abs(t1 - t2) <= 15) {
		cout << "Встреча состоится";
	}
	else {
		cout << "Встреча не состоится";
	}
	return 0;
}
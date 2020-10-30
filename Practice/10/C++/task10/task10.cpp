#include<iostream>;

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int s, l1, r1, l2, r2, x1, x2;
	cin >> s >> l1 >> r1 >> l2 >> r2;
	x1 = l1;
	x2 = s - x1;
	if ((x2 >= l2) && (x2 <= r2)) {
		cout << x1 << " " << x2;
	}
	else {
		if (x2 < l2) {
			cout << -1;
		}
		else {
			x2 = r2;
			x1 = s - x2;
			if ((x1 >= l1) && (x1 <= r1)) {
				cout << x1 << x2;
			}
			else {
				cout << -1;
			}
		}
	}
	return 0;
}
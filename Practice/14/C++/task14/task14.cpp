#include <iostream>;

using namespace std;

int main() {
	int a = 1, i = 0, n;
	cout << "n = ";
	cin >> n;
	while (a <= n) {
		a = a * 2;
		i++;
	}
	cout << i;
	return 0;
}
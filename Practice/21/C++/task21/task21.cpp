#include <iostream>

using namespace std;

double BMI(double weight, double height)
{
	return (weight / (height * height));
}

void printBMI(double BMI)
{
	if (BMI < 18.5) {
		cout << "Недостаточная масса";
	}
	else {
		if (BMI < 25.0) {
			cout << "Норма";
		}
		else {
			if (BMI < 30.0) {
				cout << "Избыточная масса";
			}
			else {
				cout << "Ожирение";
			}
		}
	}
}

int main()
{
	double w, h;
	setlocale(LC_ALL, "Russian");
	cin >> w >> h;
	h /= 100;
	printBMI(BMI(w, h));
	return 0;
}

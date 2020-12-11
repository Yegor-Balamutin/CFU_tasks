#include <iostream>
#include <vector>
#include <map>

void print_factorization(unsigned int n) {
	// составление вектора простых чисел (от 2 до 999)
	std::vector<int> prime;
	bool p;
	for (int i = 2; i < 1000; i++) {
		p = true;
		for (int j = 2; j < (i / 2) + 1; j++) {
			if (i % j == 0) {
				p = false;
				break;
			}
		}
		if (p) prime.push_back(i);
	}
	// обнуление значений словаря
	std::map<int, int> fact;
	for (int i : prime) {
		fact[i] = 0;
	}
	// заполнение словаря
	int n1 = n;
	for (int i : prime) {
		while (n1 % i == 0) {
			fact[i]++;
			n1 = n1 / i;
		}
	}
	// удаление нулевых элементов словаря
	for (int i : prime) {
		if (fact[i] == 0) fact.erase(i);
	}
	// вывод результата
	std::cout << n << " = ";
	int x = 0;
	for (auto i : fact) {
		if (i.second == 1) std::cout << i.first;
		else std::cout << i.first << "^" << i.second;
		x++;
		if (x < size(fact)) std::cout << " * ";
	}
	std::cout << std::endl;
}

int main()
{
	int n;
	std::cout << "n = ";
	std::cin >> n;
	print_factorization(n);
	return 0;
}
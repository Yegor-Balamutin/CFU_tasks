#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

std::vector<int> BozoSort(std::vector<int> arr, bool ascend = true) {
	srand(time(0));
	int a, b;
	bool sorted = false;
	while (!sorted) {
		if (ascend) {
			sorted = true;
			for (int i = 0; i < size(arr) - 1; i++) {
				if (arr[i] > arr[i + 1]) {
					sorted = false;
					break;
				}
			}
		}
		else {
			sorted = true;
			for (int i = 0; i < size(arr) - 1; i++) {
				if (arr[i] < arr[i + 1]) {
					sorted = false;
					break;
				}
			}
		}
		if (!sorted) {
			a = rand() % size(arr);
			b = rand() % size(arr);
			std::swap(arr[a], arr[b]);
		}
	}
	return arr;
}

std::vector<int> BozoSort(std::vector<std::vector<int>> arr, bool ascend = true) {
	std::vector<int> array;
	for (int i = 0; i < size(arr); i++) {
		for (int j = 0; j < size(arr[i]); j++) {
			array.push_back(arr[i][j]);
		}
	}
	array = BozoSort(array, ascend);
	return array;
}

std::vector<int> BozoSort(int a, int b, int c, bool ascend = true) {
	std::vector<int> array = { a, b, c };
	array = BozoSort(array, ascend);
	return array;
}

int main()
{
	int n, i = 0;
	std::vector<int> arr;
	std::string s, a, num = "";
	std::cin >> n;
	// Ввод элементов вектора в одну сткоку через пробел
	std::cin >> a;
	std::getline(std::cin, s);
	s = a + s + " ";
	while (i < size(s)) {
		if (s[i] != ' ') {
			num = num + s[i];
		}
		else {
			arr.push_back(std::stoi(num));
			num = "";
		}
		i++;
	}
	// Конец ввода элементов вектора

	// Составление двумерного вектора из одномерного
	std::vector<std::vector<int>> mat;
	std::vector<int> lin;
	int d = 0;
	for (int i = 0; i < sqrt(n); i++) {
		for (int j = 0; j < sqrt(n); j++) {
			lin.push_back(arr[j + d]);
		}
		mat.push_back(lin);
		d += sqrt(n);
		for (int j = 0; j < sqrt(n); j++) {
			lin.pop_back();
		}
	}
	// Конец составления двумерного вектора

	// Вызов функций и вывод результата
	std::cout << std::endl;
	std::vector<int> result;
	result = BozoSort(arr);
	for (int i = 0; i < n; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	result = BozoSort(arr, false);
	for (int i = 0; i < n; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	result = BozoSort(mat);
	for (int i = 0; i < n; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	result = BozoSort(mat, false);
	for (int i = 0; i < n; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	result = BozoSort(arr[0], arr[1], arr[2]);
	for (int i = 0; i < 3; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	result = BozoSort(arr[0], arr[1], arr[2], false);
	for (int i = 0; i < 3; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

template <typename T>
std::vector<T> BozoSort(std::vector<T> arr, bool ascend = true) {
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

template <typename T>
std::vector<T> BozoSort(std::vector<std::vector<T>> arr, bool ascend = true) {
	std::vector<T> array;
	for (int i = 0; i < size(arr); i++) {
		for (int j = 0; j < size(arr[i]); j++) {
			array.push_back(arr[i][j]);
		}
	}
	array = BozoSort(array, ascend);
	return array;
}

template <typename T>
std::vector<T> BozoSort(T a, T b, T c, bool ascend = true) {
	std::vector<T> array = { a, b, c };
	array = BozoSort(array, ascend);
	return array;
}

template <typename T>
void print(std::vector<T> vec) {
	for (int i = 0; i < size(vec); i++) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}

int main()
{
	int n, i = 0;
	std::vector<double> arr;
	std::string s, a, num = "";
	std::cin >> n;
	// Ввод элементов вектора в одну сткоку через пробел (как double)
	std::cin >> a;
	std::getline(std::cin, s);
	s = a + s + " ";
	while (i < size(s)) {
		if (s[i] != ' ') {
			num = num + s[i];
		}
		else {
			arr.push_back(std::stod(num));
			num = "";
		}
		i++;
	}
	// Конец ввода элементов вектора

	// Ввод элементов вектора в одну сткоку через пробел (как string)
	std::cin >> n;
	std::vector<std::string> arr_str;
	i = 0;
	num = "";
	std::cin >> a;
	std::getline(std::cin, s);
	s = a + s + " ";
	while (i < size(s)) {
		if (s[i] != ' ') {
			num = num + s[i];
		}
		else {
			arr_str.push_back(num);
			num = "";
		}
		i++;
	}
	// Конец ввода элементов вектора

	// Составление двумерного вектора из одномерного (для double)
	std::vector<std::vector<double>> mat;
	std::vector<double> lin;
	int d = 0;
	for (int i = 0; i < sqrt(n); i++) {
		for (int j = 0; j < sqrt(n); j++) {
			lin.push_back(arr[j + d]);
		}
		mat.push_back(lin);
		d += sqrt(n);
		lin.clear();
	}
	// Конец составления двумерного вектора

	// Составление двумерного вектора из одномерного (для string)
	std::vector<std::vector<std::string>> mat_str;
	std::vector<std::string> lin_str;
	d = 0;
	for (int i = 0; i < sqrt(n); i++) {
		for (int j = 0; j < sqrt(n); j++) {
			lin_str.push_back(arr_str[j + d]);
		}
		mat_str.push_back(lin_str);
		d += sqrt(n);
		lin_str.clear();
	}
	// Конец составления двумерного вектора
	
	// Вызов функций и вывод результата
	// для double
	std::cout << std::endl;
	std::vector<double> result;
	result = BozoSort(arr);
	print(result);
	result = BozoSort(arr, false);
	print(result);
	result = BozoSort(mat);
	print(result);
	result = BozoSort(mat, false);
	print(result);
	result = BozoSort(arr[0], arr[1], arr[2]);
	print(result);
	result = BozoSort(arr[0], arr[1], arr[2], false);
	print(result);
	// для string
	std::vector<std::string> result_str;
	result_str = BozoSort(arr_str);
	print(result_str);
	result_str = BozoSort(arr_str, false);
	print(result_str);
	result_str = BozoSort(mat_str);
	print(result_str);
	result_str = BozoSort(mat_str, false);
	print(result_str);
	result_str = BozoSort(arr_str[0], arr_str[1], arr_str[2]);
	print(result_str);
	result_str = BozoSort(arr_str[0], arr_str[1], arr_str[2], false);
	print(result_str);
	return 0;
}
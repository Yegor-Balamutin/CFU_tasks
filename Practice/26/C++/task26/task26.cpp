#include <iostream>
#include <vector>

template<typename T>
T BozoSort(T arr, bool asc = true) {
	return arr;
}

int main()
{
	int n = 4;
	std::vector<int> arr = { 1, 2, 3, 4 };
	std::vector<int> res;
	res = BozoSort<std::vector<int>>(arr);
	for (int i = 0; i < n; i++) {
		std::cout << res[i] << " ";
	}
	return 0;
}
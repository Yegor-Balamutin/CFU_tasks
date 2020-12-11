#include <iostream>
#include <vector>
#include <algorithm>

void print(std::vector<int> arr) {
	for (auto a : arr) {
		std::cout << a << " ";
	}
	std::cout << std::endl;
}

int main()
{
	std::vector<int> min5;
	int x, n;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> x;
		if (size(min5) < 5) {
			min5.push_back(x);
			std::sort(min5.begin(), min5.end(), std::greater<int>());
		}
		else {
			if (min5[0] > x) {
				min5[0] = x;
				std::sort(min5.begin(), min5.end(), std::greater<int>());
			}
		}
		print(min5);
	}
	return 0;
}
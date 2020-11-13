﻿#include <iostream>
#include <iomanip>
#include "fact.h"
#include "teylor.h"
#include "combin.h"

int main()
{
	const double pi = 3.14159265;
	double x = 0;
	std::cout << "n\t n!\n";
	for (int n = 1; n < 11; n++) {
		std::cout << n << "\t " << fac(n) << "\n";
	}
	std::cout << std::endl;
	std::cout << "x\t sin(x)\n";
	for (int i = 1; i < 47; i++) {
		std::cout << std::steprecision(4) << x << "\t " << sinx(x,5) << "\n";
		x += pi / 180;
	}
	return 0;
}
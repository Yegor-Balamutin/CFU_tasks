#pragma once
#include <cmath>
#include "fact.h"

double sinx(double x, int k) {
	double rez = 0;
	for (int i = 0; i <= k; i++) {
		rez += (pow(-1, i)) * (pow(x,2*i+1)) / (fac(2*i+1));
	}
	return rez;
}
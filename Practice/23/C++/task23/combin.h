#pragma once
#include "fact.h"

int comb(int k, int n) {
	int rez;
	rez = fac(n) / (fac(k) * fac(n - k));
	return rez;
}
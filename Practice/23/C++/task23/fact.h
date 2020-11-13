#ifndef FACT
#define FACT

int fac(int n) {
	int rez = 1;
	for (int i = 1; i <= n; i++) {
		rez *= i;
	}
	return rez;
}

#endif
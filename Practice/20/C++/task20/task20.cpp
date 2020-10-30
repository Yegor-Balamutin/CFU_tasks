#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	vector<string> name;
	vector<int> price;
	vector<int> volume;
	vector<int> purch;
	vector<int> order;
	map<int, double> d;
	string na;
	int k, money, money0, pr, vo, m, j, a = 0, v = 0;
	bool f = true;
	cout << "Деньги: ";
	cin >> money;
	money0 = money;
	cout << "Количество видов напитков в магазине: ";
	cin >> k;
	for (int i = 0; i < k; i++) {   // цикл ввода данных и рассчёта цены за литр
		cout << i + 1 << " напиток: ";
		cin >> na >> pr >> vo;
		name.push_back(na);
		price.push_back(pr);
		volume.push_back(vo);
		purch.push_back(0);
		d[i] = pr / vo;
	}
	while (size(d) > 0) {   // цикл составления списка порядка покупок
		m = 1000000;
		for (auto i: d) {
			if (i.second < m) {
				m = i.second;
				j = i.first;
			}
		}
		order.push_back(j);
		d.erase(j);
	}
	while (f) {   // цикл составления списка покупок
		if (money >= price[order[a]]) {
			money -= price[order[a]];
			purch[order[a]] += 1;
		}
		else {
			if (a < k - 1) {
				a += 1;
			}
			else {
				f = false;
			}
		}
	}
	if (money < money0) {
		cout << "Надо купить:\n";
		for (int i = 0; i < k; i++) {   // цикл вывода покупок и расчёта суммарного объёма
			v = v + purch[i] * volume[i];
			if (purch[i] > 0) {
				cout << name[i] << ": " << purch[i] << " шт.\n";
			}
		}
		cout << "Суммарный объём: " << v << endl;
		cout << "Осталось денег: " << money;
	}
	else {
		cout << -1;
	}
	return 0;
}
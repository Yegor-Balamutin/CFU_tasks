#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	string words = "hallo" + string("klempner") + "das" + "ist" + "fantastisch" + "fluggegecheimen" ;
	map <char, double> p;
	string stop;
	double prob = 1.0;
	for (char a = 'a'; a <= 'z'; a++) {
		p[a] = 0;
	}
	for (int i = 0; i < size(words); i++) {
		p[words[i]] += 1;
	}
	for (char a = 'a'; a <= 'z'; a++) {
		p[a] = p[a] / size(words);
	}
	cout << "Стоп-слово: ";
	cin >> stop;
	for (int i = 0; i < size(stop); i++) {
		prob = prob * p[stop[i]];
	}
	cout << " Вероятность равна: " << prob << endl;
	return 0;
}
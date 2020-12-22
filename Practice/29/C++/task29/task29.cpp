#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

struct Student {
	std::string name;
	int group;
	std::map<std::string, int> exams;
};

std::string adding_spc(std::string name) {
	while (size(name) < 11) {
		name += " ";
	}
	return name;
}

std::ostream& operator<< (std::ostream& out, Student stud) {
	out << "+------------+-------+------+------+------+------+\n";
	out << "| Name       | Group | Math | Phys | Hist | Prog |\n";
	out << "+------------+-------+------+------+------+------+\n";
	out << "| " << adding_spc(stud.name)
		<< "| " << stud.group << "     "
		<< "| " << stud.exams["mathematics"] << "    "
		<< "| " << stud.exams["physics"] << "    "
		<< "| " << stud.exams["history"] << "    "
		<< "| " << stud.exams["programming"] << "    |\n"
		<< "+------------+-------+------+------+------+------+\n";
	return out;
}

std::ostream& operator<< (std::ostream& out, std::vector<Student> vec) {
	std::cout << "+------------+-------+------+------+------+------+\n";
	std::cout << "| Name       | Group | Math | Phys | Hist | Prog |\n";
	std::cout << "+------------+-------+------+------+------+------+\n";
	for (auto a : vec) {
		std::cout << "| " << adding_spc(a.name)
		<< "| " << a.group << "     "
		<< "| " << a.exams["mathematics"] << "    "
		<< "| " << a.exams["physics"] << "    "
		<< "| " << a.exams["history"] << "    "
		<< "| " << a.exams["programming"] << "    |\n"
		<< "+------------+-------+------+------+------+------+\n";
	}
	return out;
}

bool operator> (Student a, Student b) {
	if (a.name > b.name) {
		return true;
	}
	else {
		return false;
	}
}

bool operator< (Student a, Student b) {
	if (a.name < b.name) {
		return true;
	}
	else {
		return false;
	}
}

template <typename T>
std::vector<T> BozoSort(std::vector<T> arr, bool ascend = true) {
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

int main()
{
	// Блок рандомного заполнения вектора Students
	srand(time(0));
	std::vector<std::string> human_names = { "Alex", "Devid", "Bob", "Wilson", "Steve", "Ben", "Robert", "Alice", "Mary", "Windy", "Katy", "Emma" };
	std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<int> marks = { 2, 3, 3, 3, 4, 4, 4, 4, 5, 5 };
	std::vector<Student> Students;
	Student new_stud;
	for (int i = 0; i < 10; i++) {
		new_stud.name = human_names[rand() % size(human_names)] + " " + letters[rand() % size(letters)];
		new_stud.group = (rand() % 9) + 1;
		new_stud.exams["mathematics"] = marks[rand() % size(marks)];
		new_stud.exams["physics"] = marks[rand() % size(marks)];
		new_stud.exams["history"] = marks[rand() % size(marks)];
		new_stud.exams["programming"] = marks[rand() % size(marks)];
		Students.push_back(new_stud);
	}

	std::vector<Student> bad;
	for (Student stud : Students) {
		for (auto mark : stud.exams) {
			if (mark.second == 2) {
				bad.push_back(stud);
				break;
			}
		}
	}
	if (size(bad) == 0) {
		std::cout << "Not found";
	}
	else {
		bad = BozoSort(bad);
		std::cout << bad << std::endl;
		std::cout << "Expulsion" << std::endl;
		std::cout << bad[rand() % size(bad)];
	}
	return 0;
}
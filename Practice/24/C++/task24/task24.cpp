#include <iostream>
#include <fstream>
#include "json.hpp"
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using json = nlohmann::json;

int main()
{
	json input;
	json output;
	std::ifstream file1("in.json");
	file1 >> input;
	std::map <int, int> users_tasks;
	int id;
	for (auto& i : input.items()) {
		id = i.value()["userId"];
		if ((users_tasks.find(id)) == users_tasks.end()) {
			users_tasks[id] = 0;
		}
		if (i.value()["completed"]) {
			users_tasks[id] += 1;
		}
	}
	int n = 0;
	output = json::array();
	for (auto i : users_tasks) {
		output.push_back(json::object());
		output[n].push_back({ "userId",i.first });
		output[n].push_back({ "task_completed",i.second });
		n++;
	}
	std::ofstream file2("out.json");
	file2 << std::setw(2) << output;
	return 0;
}
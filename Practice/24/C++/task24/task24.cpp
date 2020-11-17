#include <iostream>
#include <fstream>
#include "json.hpp"
#include <string>

using json = nlohmann::json;

int main()
{
	// тренируюсь работать с json
	json ar = { {"key1",5},{"key2",8},{"key3","aaaaa"} };
	return 0;
}
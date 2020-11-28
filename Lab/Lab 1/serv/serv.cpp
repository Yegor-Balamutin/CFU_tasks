#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <cpp_httplib/httplib.h>
#include <iomanip>
using namespace httplib;
using json = nlohmann::json;

void gen_response(const Request& req, Response& res);
void gen_response_raw(const Request& req, Response& res);
int t_request();
json w_request();
int check_cache(json cashe, int utime);
bool check_res(httplib::Result& res);
std::string repl_val(std::string wid, std::string w_des, std::string icon, std::string temp);


int main()
{
	Server svr;                    // Создаём сервер (пока-что не запущен)
	svr.Get("/", gen_response);    // Вызвать функцию gen_response если кто-то обратиться к корню "сайта"
	svr.Get("/raw", gen_response_raw);
	std::cout << "Start server... OK\n";
	svr.listen("localhost", 3000); // Запускаем сервер на localhost и порту 3000
}

// В этой функции формируем ответ сервера на запрос в "/"
void gen_response(const Request& req, Response& res) {
	std::cout << "receuved a request on \"/\"\n";
	int ctime = t_request();
	json weather;
	std::ifstream file1("cache.json");
	if (not(file1.is_open())) {   // если кеш отсутствует то запрашиваем и помещаем ответ в wether
		std::cout << "cache is empty\n";
		weather = w_request();
	}
	else {
		std::cout << "cache is normal\n";
		file1 >> weather;                    // в wether помещаем значение из cache.json
		file1.close();
	}
	int t = check_cache(weather, ctime);
	if (t == 48) {      // если t == 48 то кеш устарел
		std::cout << "cache is old\n";
		weather = w_request();
		t = check_cache(weather, ctime);
	}
	if (t > -1) {            // если t == -1 значит небыло получено время
		std::ifstream file("widget.html");
		std::string widget = "";
		if (file.is_open()) {
			std::cout << "creating response... ";
			getline(file, widget, '\0');   // Читаем все пока не встретим символ '\0'
			file.close();
			std::string w_des = weather["hourly"][t]["weather"][0]["description"];
			std::string icon = weather["hourly"][t]["weather"][0]["icon"];
			std::string temp = std::to_string(int(round(double(weather["hourly"][t]["temp"]))));
			widget = repl_val(widget, w_des, icon, temp);
			std::cout << "OK\n";
			res.set_content(widget, "text/html");
			std::cout << "response sent\n";
		}
		else std::cout << "failed open html file\n";
	}
	std::cout << "function gen_response finished.\n\n";
}

// В этой функции формируем ответ сервера на запрос в "/raw"
void gen_response_raw(const Request& req, Response& res) {
	std::cout << "receuved a request on \"/raw\"\n";
	int ctime = t_request();
	json weather;
	std::ifstream file1("cache.json");
	if (not(file1.is_open())) {   // если кеш отсутствует то запрашиваем и помещаем ответ в wether
		std::cout << "cache is empty\n";
		weather = w_request();
	}
	else {
		std::cout << "cache is normal\n";
		file1 >> weather;                    // в wether помещаем значение из cache.json
		file1.close();
	}
	int t = check_cache(weather, ctime);
	if (t == 48) {      // если t == 48 то кеш устарел
		std::cout << "cache is old\n";
		weather = w_request();
		t = check_cache(weather, ctime);
	}
	if (t > -1) {          // если t == -1 значит небыло получено время
		std::cout << "creating response... ";
		json raw;
		raw["weather_des"] = weather["hourly"][t]["weather"][0]["description"];
		raw["temp"] = weather["hourly"][t]["temp"];
		std::cout << "OK\n";
		res.set_content(raw.dump(), "text/json");
		std::cout << "response sent\n";
	}
	std::cout << "function gen_response_raw finished.\n\n";
}

// запрашивает время и возвращает значение ["unixtime"]
int t_request() {
	std::cout << "requesting time... ";
	Client cli_t("http://worldtimeapi.org");
	auto time_req = cli_t.Get("/api/timezone/Europe/Simferopol");
	int ctime = -1;
	if (check_res(time_req)) {
		json time = json::parse(time_req->body);
		ctime = time["unixtime"];
		std::cout << "OK\n";
	}
	return ctime;
}

// запрашивает погоду и возвращает json с ответом сайта и записывает его в cache.json
json w_request() {
	std::cout << "reqesting wether from openwether.org... ";
	Client cli_w("http://api.openweathermap.org");
	auto weather_req = cli_w.Get("/data/2.5/onecall?lat=44.9572&lon=34.1108&units=metric&exclude=current,minutely,daily,alerts&lang=ru&appid=bad75ef80333e8205e98138e6836321a");
	json weather = {};
	if (check_res(weather_req)) {
		weather = json::parse(weather_req->body);
		std::ofstream file2("cache.json");
		file2 << std::setw(2) << weather;
		file2.close();
		std::cout << "OK\n";
	}
	return weather;
}

// принимает json и время и возвращает индекс с нужным значением погоды
int check_cache(json cashe, int utime) {
	std::cout << "start check_cache... ";
	int im = 48;
	if (utime > -1) { 
		for (int i = 47; i >= 0; i--) {
			if (cashe["hourly"][i]["dt"] > utime) {
				im = i;
			}
		}
		std::cout << "OK (i = " << im << ")\n";
	}
	else {
		im = -1;
		std::cout << "invalid time\n";
	}
	return im;
}

// проверяет ответ от сайта и если всё ок то возвращает true
bool check_res(httplib::Result& res)
{
	bool f = false;
	if (res) {
		if (res->status == 200) {
			f = true;
		}
		else {
			std::cout << "Status code: " << res->status << std::endl;
		}
	}
	else {
		auto err = res.error();
		std::cout << "Error code: " << err << std::endl;
	}
	return f;
}

// принимает глав строку и две подстроки; заменяет в глав строке первую подстроку на вторую
std::string rep_to(std::string main_s, std::string s1, std::string s2) {
	int a;
	a = main_s.find(s1);
	main_s.replace(a, size(s1), s2);
	return main_s;
};

// заменяет значения в шаблоне на найденные
std::string repl_val(std::string wid, std::string w_des, std::string icon, std::string temp) {
	std::string des_r = "{hourly[i].weather[0].description}";
	std::string icon_r = "{hourly[i].weather[0].icon}";
	std::string temp_r = "{hourly[i].temp}";

	wid = rep_to(wid, des_r, w_des);
	wid = rep_to(wid, icon_r, icon);
	wid = rep_to(wid, temp_r, temp);
	wid = rep_to(wid, temp_r, temp);
	return wid;
}

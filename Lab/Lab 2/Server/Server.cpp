#include <iostream>
#include <fstream>
#include <iomanip>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>

using namespace httplib;
using json = nlohmann::json;

std::ofstream logger("log.txt");

// В этой функции формируем ответ сервера на запрос
void gen_response(const Request& req, Response& res) {
    logger << u8"starting function\n";
    json conf;
    std::ifstream file1("configuration.json");
    if (file1.is_open()) {
        logger << u8"conf file is opened\n";
        file1 >> conf;
        file1.close();
    }
    else {
        logger << u8"conf file is not founded\n";
        conf["webhooks"] = json::array();
        std::ofstream file1("configuration.json");
        file1 << std::setw(2) << conf;
        file1.close();
    }
    // Выводим на экран тело запроса
    std::cout << req.body.c_str();
    // Здесь будет ответ, пока-что взят пример из документации
    std::string str = u8R"(
    {
      "response": {
        "text": "Здравствуйте! Это мы, хороводоведы.",
        "tts": "Здравствуйте! Это мы, хоров+одо в+еды.",
        "buttons": [
            {
                "title": "Надпись на кнопке",
                "payload": {},
                "url": "https://example.com/",
                "hide": true
            }
        ],
        "end_session": false
      },
      "version": "1.0"
    })";
    logger.close();
    // Отправляем ответ
    res.set_content(str, "text/json; charset=UTF-8");
}

int main() {
    Server svr;                            // Создаём сервер (пока-что не запущен)
    svr.Post("/", gen_response);           // Вызвать функцию gen_response на post запрос
    logger << u8"Start server... OK\n";    // cout использовать нельзя
    svr.listen("localhost", 1234);         // Запускаем сервер на localhost и порту 1234
}
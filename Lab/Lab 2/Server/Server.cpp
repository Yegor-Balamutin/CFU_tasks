#include <iostream>
#include <fstream>
#include <iomanip>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>

using namespace httplib;
using json = nlohmann::json;

void gen_res_webhooks(const Request& req, Response& res);
void gen_res_edit_webhooks(const Request& req, Response& res);
std::string gen_html();

int main() {
    Server svr;                                      // Создаём сервер (пока-что не запущен)
    svr.Post("/webhooks", gen_res_edit_webhooks);    // Вызвать функцию gen_response на post запрос
    svr.Get("/webhooks", gen_res_webhooks);
    std::cout << "Starting server... OK\n";
    svr.listen("localhost", 1234);                   // Запускаем сервер на localhost и порту 1234
}

// В этой функции заполняется и отправляется html шаблон с вебхуками
void gen_res_webhooks(const Request& req, Response& res) {
    std::cout << "Recived a Get-request on \"\\webhooks\"\n";
    std::string html_temp = gen_html();
    // отправка заполненного html шаблона
    res.set_content(html_temp, "text/html");
    std::cout << "response sent\n";
    std::cout << "function gen_response_webhooks finished" << std::endl << std::endl;
}

// В этой функции добавляется или удаляется вебхук и отправляется ответ
void gen_res_edit_webhooks(const Request& req, Response& res) {
    std::cout << "Recived a Post-request on \"\\webhooks\"\n";
    // Открытие конфигурационного файла
    json conf;
    std::ifstream file1("configuration.json");
    if (file1.is_open()) {
        std::cout << "conf file is opened\n";
        file1 >> conf;
        file1.close();
    }
    else {
        std::cout << "conf file is not founded\n";
        return;
    }
    // Проверка на то, поступил ли запрос на добавление или на удаление
    if (req.has_param("del")) {
        std::cout << "request for deliting \"" << req.get_param_value("del") << "\"\n";
        auto val = req.get_param_value("del");
        // удаление из файла конфигурации
        for (int i = 0; i < size(conf["webhooks"]); i++) {
            if (conf["webhooks"][i] == val) {
                conf["webhooks"].erase(conf["webhooks"].begin() + i);
                break;
            }
        }
    }
    else {
        std::cout << "request for adding \"" << req.get_param_value("set") << "\"\n";
        // добавление в файл конфигурации
        auto val = req.get_param_value("set");
        conf["webhooks"].push_back(val);
    }
    // Запись изменений в файл
    std::ofstream file2("configuration.json");
    file2 << std::setw(2) << conf;
    file2.close();
    // Формируем и отправляем ответ
    std::string str = gen_html();
    res.set_content(str, "text/html; charset=UTF-8");
    std::cout << "function gen_res_edit_webhooks finished" << std::endl << std::endl;
}

// Функция заполняет html шаблон по файлу конфигурации и возвращает его в виде строки
std::string gen_html() {
    std::cout << "Starting function gen_html\n";
    // загрузка файла конфигурации
    json conf;
    std::ifstream file1("configuration.json");
    if (file1.is_open()) {
        std::cout << "conf file is opened\n";
        file1 >> conf;
        file1.close();
    }
    else {
        std::cout << "conf file is not founded\n";
        conf["webhooks"] = json::array();
        std::ofstream file1("configuration.json");
        file1 << std::setw(2) << conf;
        file1.close();
    }
    // чтение html шаблона
    std::ifstream file2("dashboard.html");
    std::string html_temp;
    if (file2.is_open()) {
        std::getline(file2, html_temp, '\0');
    }
    else {
        std::cout << "failed open html file\n";
        std::cout << "function gen_response_webhooks finished\n";
        return "";
    }
    // заполнение html шаблона
    std::string plug = u8"{webhooks_list}";
    if (size(conf[u8"webhooks"]) == 0) {
        std::cout << "no any webhooks\n";
    }
    else {
        std::cout << "webhooks exist:\n";
        std::string webhook = u8R"(
     <div class="form-row align-items-center">
        <div class="col">
        <input type="text" value="{Webhook URL}" class="form-control mb-2" disabled>
        </div>
 
        <div class="col">
            <button type="submit" name="del" value="{Webhook URL}" class="btn btn-danger mb-2">Удалить</button>
        </div>
     </div>)";
        webhook += plug;
        std::string plug1 = u8"{Webhook URL}";
        for (std::string i : conf["webhooks"]) {
            std::cout << i << std::endl;
            html_temp = html_temp.replace(html_temp.find(plug), size(plug), webhook);
            html_temp = html_temp.replace(html_temp.find(plug1), size(plug1), i);
            html_temp = html_temp.replace(html_temp.find(plug1), size(plug1), i);
        }
    }
    html_temp = html_temp.replace(html_temp.find(plug), size(plug), "");
    return html_temp;
}
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>

using namespace httplib;
using json = nlohmann::json;

void gen_res_webhooks(const Request& req, Response& res);

// В этой функции формируем ответ сервера на запрос
void gen_res_edit_webhooks(const Request& req, Response& res) {
    std::cout << "Recived a Post-request on \"\\webhooks\"\n";
    if (req.has_param("del")) {
        std::cout << "request for deliting \"" << req.get_param_value("del") << "\"\n";
    }
    else {
        std::cout << "request for adding \"" << req.get_param_value("set") << "\"\n";
    }
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
    // Выводим на экран тело запроса
    std::cout << req.body.c_str() << std::endl;
    // Здесь будет ответ, пока-что взят пример из документации
    std::string str = "";
    // Отправляем ответ
    res.set_content(str, "text/json; charset=UTF-8");
    std::cout << "function gen_res_edit_webhooks finished" << std::endl << std::endl;
}

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
    std::ifstream file2("Dashboard.html");
    std::string html_temp;
    if (file2.is_open()) {
        std::getline(file2, html_temp, '\0');
    }
    else {
        std::cout << "failed open html file\n";
        std::cout << "function gen_response_webhooks finished\n";
        return;
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
        }
    }
    html_temp = html_temp.replace(html_temp.find(plug), size(plug), "");
    // отправка заполненного html шаблона
    res.set_content(html_temp, "text/html");
    std::cout << "response sent\n";
    std::cout << "function gen_response_webhooks finished" << std::endl << std::endl;
}
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>

using namespace httplib;
using json = nlohmann::json;

std::ofstream logger("log.txt");

void gen_res_webhooks(const Request& req, Response& res);
void gen_res_edit_webhooks(const Request& req, Response& res);
void gen_res_alice(const Request& req, Response& res);
std::string gen_html();
void send_purch(json purchase);

int main() {
    Server svr;                                      // Создаём сервер (пока-что не запущен)
    svr.Post("/webhooks", gen_res_edit_webhooks);    // Вызвать функцию gen_response на post запрос
    svr.Get("/webhooks", gen_res_webhooks);
    svr.Post("/", gen_res_alice);
    std::cout << u8"Starting server... OK\n";
    logger << u8"Starting server... OK\n";
    svr.listen("localhost", 1234);                   // Запускаем сервер на localhost и порту 1234
}

// В этой функции заполняется и отправляется html шаблон с вебхуками
void gen_res_webhooks(const Request& req, Response& res) {
    logger << u8"Recived a Get-request on \"\\webhooks\"\n";
    std::string html_temp = gen_html();
    // отправка заполненного html шаблона
    res.set_content(html_temp, "text/html");
    logger << u8"response sent\n";
    logger << u8"function gen_response_webhooks finished" << std::endl << std::endl;
}

// В этой функции добавляется или удаляется вебхук и отправляется ответ
void gen_res_edit_webhooks(const Request& req, Response& res) {
    logger << u8"Recived a Post-request on \"\\webhooks\"\n";
    // Открытие конфигурационного файла
    json conf;
    std::ifstream file1("configuration.json");
    if (file1.is_open()) {
        logger << u8"conf file is opened\n";
        file1 >> conf;
        file1.close();
    }
    else {
        logger << u8"conf file is not founded\n";
        return;
    }
    // Проверка на то, поступил ли запрос на добавление или на удаление
    if (req.has_param("del")) {
        logger << u8"request for deliting \"" << req.get_param_value("del") << "\"\n";
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
        logger << u8"request for adding \"" << req.get_param_value("set") << "\"\n";
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
    logger << u8"function gen_res_edit_webhooks finished" << std::endl << std::endl;
}

// Функция заполняет html шаблон по файлу конфигурации и возвращает его в виде строки
std::string gen_html() {
    logger << u8"Starting function gen_html\n";
    // загрузка файла конфигурации
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
    // чтение html шаблона
    std::ifstream file2("dashboard.html");
    std::string html_temp;
    if (file2.is_open()) {
        std::getline(file2, html_temp, '\0');
    }
    else {
        logger << u8"failed open html file\n";
        logger << u8"function gen_response_webhooks finished\n";
        return "";
    }
    // заполнение html шаблона
    std::string plug = u8"{webhooks_list}";
    if (size(conf[u8"webhooks"]) == 0) {
        logger << u8"no any webhooks\n";
    }
    else {
        logger << u8"existing webhooks:\n";
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
            logger << u8"   " << i << std::endl;
            html_temp = html_temp.replace(html_temp.find(plug), size(plug), webhook);
            html_temp = html_temp.replace(html_temp.find(plug1), size(plug1), i);
            html_temp = html_temp.replace(html_temp.find(plug1), size(plug1), i);
        }
    }
    html_temp = html_temp.replace(html_temp.find(plug), size(plug), "");
    return html_temp;
}

// Функкция принимает Post-запрос от Алисы и отправляет ответ
void gen_res_alice(const Request& req, Response& res) {
    logger << u8"Recived a Post - request on \"/\"\n";
    json j_req = json::parse(req.body);
    // создание макета ответа в формате json
    json j_res;
    std::string str = u8R"(
        {
            "response": {
                "text": "Здравствуйте! Я помогу вам с покупками.",
                "tts": "Здравствуйте! Я помогу вам с покупками.",
                "buttons": [
                    {
                        "title": "Молчать",
                        "payload": {},
                        "hide": true
                    },
                    {
                        "title": "Помощь",
                        "payload": {},
                        "hide": true
                    }
                ],
                "end_session": false
            },
            "session_state": {
                "talk": true,
                "help_mode": false,
                "basket": {}
            },
            "version": "1.0"
        }
        )";
    j_res = json::parse(str);
    // если сессия новая, то посылаем макет ответа без изменений
    if (j_req["session"]["new"]) {
        logger << u8"session is new\n";
    }
    else {
        logger << u8"session isn't new\n";
        // проверка сессионных данных
        if (!(j_req["state"]["session"].count("talk") && j_req["state"]["session"].count("help_mode") && j_req["state"]["session"].count("basket"))) {
            logger << u8"session data not found\n";
            j_res["response"]["text"] = u8"Произошла ошибка";
            j_res["response"]["tts"] = u8"произошла ошибка";
            j_res["response"]["buttons"] = json::array();
            j_res["response"]["end_session"] = true;
            res.set_content(j_res.dump(), "text/json; charset=UTF-8");
            logger << u8"response sent\n" << std::endl;
            return;
        }
        j_res["session_state"] = j_req["state"]["session"];
        // если режим помощи не включен
        if (!(j_req["state"]["session"]["help_mode"])) {
            if (j_res["session_state"]["talk"]) j_res["response"]["buttons"][0]["title"] = u8"Молчать";
            else j_res["response"]["buttons"][0]["title"] = u8"Говорить";
            if (j_req["request"]["nlu"]["tokens"][0] == u8"молчать") {
                logger << u8"recived a command to be quiet\n";
                j_res["session_state"]["talk"] = false;
                j_res["response"]["text"] = u8"Молчу, молчу.";
                j_res["response"]["tts"] = u8"sil <[500]>";
                j_res["response"]["buttons"][0]["title"] = u8"Говорить";
            }
            else {
                if (j_req["request"]["nlu"]["tokens"][0] == u8"говорить") {
                    logger << u8"recived a command to talk\n";
                    j_res["session_state"]["talk"] = true;
                    j_res["response"]["text"] = u8"Хорошо.";
                    j_res["response"]["tts"] = u8"хорошо";
                    j_res["response"]["buttons"][0]["title"] = u8"Молчать";
                }
                else {
                    if (j_req["request"]["nlu"]["tokens"][0] == u8"помощь") {
                        logger << u8"recived a command to help\n";
                        j_res["session_state"]["help_mode"] = true;
                        j_res["response"]["text"] = u8"Корзина. Поможет организовать покупки.\nO чём рассказать подробнее?";
                        if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"O чём рассказать подробнее?";
                        else j_res["response"]["tts"] = u8"sil <[500]>";
                        j_res["response"]["buttons"][0]["title"] = u8"Корзина";
                        j_res["response"]["buttons"][0]["payload"] = "{}";
                        j_res["response"]["buttons"][0]["hide"] = true;
                        j_res["response"]["buttons"][1]["title"] = u8"Назад";
                        j_res["response"]["buttons"][1]["payload"] = "{}";
                        j_res["response"]["buttons"][1]["hide"] = true;
                    }
                    else {
                        if ((j_req["request"]["nlu"]["tokens"][0] == u8"очистить") && (j_req["request"]["nlu"]["tokens"][1] == u8"корзину")) {
                            logger << u8"recived a command to clear basket\n";
                            j_res["session_state"]["basket"] = json::object();
                            j_res["response"]["text"] = u8"Корзина очищена.";
                            if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"корзина очищена";
                            else j_res["response"]["tts"] = u8"sil <[500]>";
                        }
                        else {
                            if ((size(j_req["request"]["nlu"]["tokens"]) == 6) && ((j_req["request"]["nlu"]["tokens"][0] == u8"добавить") && (j_req["request"]["nlu"]["tokens"][1] == u8"в") && (j_req["request"]["nlu"]["tokens"][2] == u8"корзину"))) {
                                std::string name = j_req["request"]["nlu"]["tokens"][3];
                                int price = j_req["request"]["nlu"]["entities"][0]["value"];
                                logger << u8"recived a command to add " << name << u8" " << price << u8" " << j_req["request"]["nlu"]["tokens"][5] << "\n";
                                j_res["session_state"]["basket"][name] = price;
                                j_res["response"]["text"] = u8"Товар добавлен.";
                                if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"товар добавлен";
                                else j_res["response"]["tts"] = u8"sil <[500]>";
                            }
                            else {
                                if ((size(j_req["request"]["nlu"]["tokens"]) == 3) && ((j_req["request"]["nlu"]["tokens"][0] == u8"что") && (j_req["request"]["nlu"]["tokens"][1] == u8"в") && (j_req["request"]["nlu"]["tokens"][2] == u8"корзине"))) {
                                    logger << u8"recived a command to show basket\n";
                                    if (j_res["session_state"]["basket"] == json::object()) {
                                        j_res["response"]["text"] = u8"Корзина пуста.";
                                        if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"корзина пуста";
                                        else j_res["response"]["tts"] = u8"sil <[500]>";
                                    }
                                    else {
                                        std::string s = u8"Вот что есть в корзине:\n";
                                        std::string name;
                                        int price;
                                        for (auto product = j_res["session_state"]["basket"].begin(); product != j_res["session_state"]["basket"].end(); ++product) {
                                            name = product.key();
                                            price = product.value();
                                            s = s + name + " " + std::to_string(price) + u8" руб\n";
                                        }
                                        j_res["response"]["text"] = s;
                                        if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"вот что есть в корзине";
                                        else j_res["response"]["tts"] = u8"sil <[500]>";
                                    }
                                }
                                else {
                                    if ((size(j_req["request"]["nlu"]["tokens"]) == 4) && ((j_req["request"]["nlu"]["tokens"][0] == u8"удалить") && (j_req["request"]["nlu"]["tokens"][1] == u8"из") && (j_req["request"]["nlu"]["tokens"][2] == u8"корзины"))) {
                                        logger << u8"recived a command to delite " << j_req["request"]["nlu"]["tokens"][3] << "\n";
                                        json copy_basket = json::object();
                                        std::string del_obj = j_req["request"]["nlu"]["tokens"][3];
                                        bool found = false;
                                        for (auto product : j_res["session_state"]["basket"].items()) {
                                            if (product.key() == del_obj) found = true;
                                            else copy_basket[product.key()] = product.value();
                                        }
                                        if (found) {
                                            j_res["response"]["text"] = u8"Товар удалён.\n";
                                            if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"товар удалён";
                                            else j_res["response"]["tts"] = u8"sil <[500]>";
                                            j_res["session_state"]["basket"] = copy_basket;
                                        }
                                        else {
                                            j_res["response"]["text"] = u8"В корзине нет такого товара.\n";
                                            if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"в корзине нет такого товара";
                                            else j_res["response"]["tts"] = u8"sil <[500]>";
                                        }
                                    }
                                    else {
                                        if (j_req["request"]["nlu"]["tokens"][0] == u8"сумма") {
                                            logger << u8"recived a command to count price\n";
                                            int count = 0;
                                            for (auto product : j_res["session_state"]["basket"].items()) {
                                                count += product.value();
                                            }
                                            j_res["response"]["text"] = u8"Суммарная стоимость: " + std::to_string(count) + u8" руб\n";
                                            if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"сумарная стоимость " + std::to_string(count) + u8" рублей";
                                            else j_res["response"]["tts"] = u8"sil <[500]>";
                                        }
                                        else {
                                            if ((size(j_req["request"]["nlu"]["tokens"]) == 2) && ((j_req["request"]["nlu"]["tokens"][0] == u8"покупка") && (j_req["request"]["nlu"]["tokens"][1] == u8"завершена"))) {
                                                logger << u8"recived a command to finish purchase\n";
                                                j_res["response"]["text"] = u8"Заходите ещё.";
                                                if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"заходите ещё";
                                                else j_res["response"]["tts"] = u8"sil <[500]>";
                                                j_res["response"]["end_session"] = true;
                                                if (j_res["session_state"]["basket"] == json::object()) {
                                                    logger << u8"no any products in the basket\n";
                                                }
                                                else {
                                                    json purchase = json::object();
                                                    if (j_req["session"]["user"].count("user_id")) purchase["user_id"] = j_req["session"]["user"]["user_id"];
                                                    else purchase["user_id"] = "anonymous";
                                                    purchase["check"] = json::array();
                                                    int i = 0;
                                                    for (auto product : j_res["session_state"]["basket"].items()) {
                                                        purchase["check"].push_back(json::object());
                                                        purchase["check"][i][product.key()] = product.value();
                                                        i++;
                                                    }
                                                    send_purch(purchase);
                                                }
                                            }
                                            else {
                                                logger << u8"recived an unknown command\n";
                                                j_res["response"]["text"] = u8"Я не понимаю.";
                                                if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"я не понимаю";
                                                else j_res["response"]["tts"] = u8"sil <[500]>";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // если режим помощи включен
        else {
            j_res["response"]["buttons"][0]["title"] = u8"Корзина";
            j_res["response"]["buttons"][1]["title"] = u8"Назад";
            if (j_req["request"]["nlu"]["tokens"][0] == u8"корзина") {
                logger << u8"recived a command to tell about basket\n";
                j_res["response"]["text"] = u8R"(
                Команды для корзины:
                "Добавить в корзину [название товара] [цена] рублей" - добавляет товар в корзину.
                "Удалить из корзины [название товара]" - удаляет товар из корзины.
                "Очистить корзину" - очищает корзину.
                "Что в корзине" - выводит список товаров из корзины.
                "Покупка завершена" - отсылает всем подписавшимся список покупок из корзины.
                О чём ещё рассказать?
                )";
                if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"команды для корзины";
                else j_res["response"]["tts"] = u8"sil <[500]>";
            }
            else {
                if (j_req["request"]["nlu"]["tokens"][0] == u8"назад") {
                    logger << u8"recived a command to dont help\n";
                    j_res["response"]["text"] = u8"Обращайся ещё.";
                    if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"Обращайся ещё";
                    else j_res["response"]["tts"] = u8"sil <[500]>";
                    j_res["session_state"]["help_mode"] = false;
                    if (j_res["session_state"]["talk"]) j_res["response"]["buttons"][0]["title"] = u8"Молчать";
                    else j_res["response"]["buttons"][0]["title"] = u8"Говорить";
                    j_res["response"]["buttons"][1]["title"] = u8"Помощь";
                }
                else {
                    logger << u8"recived an unknown command\n";
                    j_res["response"]["text"] = u8"Я не понимаю.";
                    if (j_res["session_state"]["talk"]) j_res["response"]["tts"] = u8"я не понимаю";
                    else j_res["response"]["tts"] = u8"sil <[500]>";
                }
            }
        }
    }
    res.set_content(j_res.dump(), "text/json; charset=UTF-8");
    logger << u8"response sent\n" << std::endl;
}

// Функция принимает json с покупками и отправляет его всем зарегестрированным вебхукам
void send_purch(json purchase) {
    logger << u8"Starting function send_purch\n";
    // загрузка файла конфигурации
    json conf;
    std::ifstream file1("configuration.json");
    if (file1.is_open()) {
        logger << u8"conf file is opened\n";
        file1 >> conf;
        file1.close();
    }
    else {
        logger << u8"conf file is not founded\n";
        return;
    }
    // цикл проходится по всем элементам массива webhooks
    for (auto item : conf["webhooks"].items()) {
        std::string webhook = item.value();
        std::string domen = "";
        std::string path = "/";
        int slash = 0;
        int i = 0;
        // цикл выделяет домен из вебхука (домен - первая часть вебхука до третьего появления символа '/'
        while (i < size(webhook)) {
            if (webhook[i] == '/') slash++;
            if (slash == 3) break;
            else domen += webhook[i];
            i++;
        }
        i++;
        // цикл выделяет оставшуюся часть вебхука
        while (i < size(webhook)) {
            path += webhook[i];
            i++;
        }
        logger << u8"webhook: " << webhook << "\n";
        if (domen[4] == 's') {
            domen.erase(4, 1);
            logger << u8"changed https to http\n";
        }
        logger << u8"domen: " << domen << "\n";
        logger << u8"path: " << path << "\n";
        // отправка json purchase на вебхук
        logger << u8"sending request on webhook... ";
        Client cli(domen.c_str());
        auto res = cli.Post(path.c_str(), purchase.dump(), "application/json");
        // res преобразуется в true, если запрос-ответ прошли без ошибок
        if (res) {
            // Проверяем статус ответа, т.к. может быть 404 и другие
            if (res->status == 200) {
                // В res->body лежит string с ответом сервера
                logger << u8"OK. Response: " << res->body << std::endl;
            }
            else {
                logger << u8"Status code: " << res->status << std::endl;
            }
        }
        else {
            auto err = res.error();
            logger << u8"Error code: " << err << std::endl;
        }
    }
    logger << u8"function send_purch finished\n";
}
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <nlohmann/json.hpp>
#include <cpp_httplib/httplib.h>

using namespace httplib;
using json = nlohmann::json;

void new_id_reg(const Request& req, Response& res);
void show_act_games(const Request& req, Response& res);
void create_game(const Request& req, Response& res);
void join_game(const Request& req, Response& res);
void exit_game(const Request& req, Response& res);
void enter_team(const Request& req, Response& res);
void quit_team(const Request& req, Response& res);
void set_ready(const Request& req, Response& res);
void deselect_ready(const Request& req, Response& res);
void do_shoot(const Request& req, Response& res);

std::string id_up(std::string id);
int abs(int a) {
    if (a < 0) return -a;
    return a;
}

class Game {
public:
    //enum ERR {};    <-- in future
    class Ship {
    public:
        int x, y, size;
        std::string orient;
        std::vector <bool> parts;
        Ship(int x, int y, int size, std::string orient = "e") : x(x), y(y), size(size), orient(orient) {
            for (int i = 0; i < size; i++) parts.push_back(true);
        }
        Ship() : x(0), y(0), size(1), orient("e") { parts.push_back(true); }
    };
    class Player {
    public:
        class Field {
        public:
            class Cell {
            public:
                int x, y;
                bool shooted = false;
                bool contains_ship = false;
                std::string ship_key = "";
                Cell(int x = 0, int y = 0) : x(x), y(y) {}
            };
            Cell battle_field[10][10];
            std::map <std::string, Ship> ships;
            Field() {
                for (int x = 0; x < 10; x++) {
                    for (int y = 0; y < 10; y++) {
                        battle_field[x][y] = Cell(x, y);
                    }
                }
            }
            bool check_area(int x, int y) {
                if (battle_field[x][y].contains_ship) return false;
                if (x < 9 && battle_field[x + 1][y].contains_ship) return false;
                if (x < 9 && y > 0 && battle_field[x + 1][y - 1].contains_ship) return false;
                if (y > 0 && battle_field[x][y - 1].contains_ship) return false;
                if (x > 0 && y > 0 && battle_field[x - 1][y - 1].contains_ship) return false;
                if (x > 0 && battle_field[x - 1][y].contains_ship) return false;
                if (x > 0 && y < 9 && battle_field[x - 1][y + 1].contains_ship) return false;
                if (y < 9 && battle_field[x][y + 1].contains_ship) return false;
                if (x < 9 && y < 9 && battle_field[x + 1][y + 1].contains_ship) return false;
                return true;
            }
            bool check_for_add(Ship ship) {
                if (ship.orient == "e") {
                    if ((ship.x + ship.size - 1) < 10) {
                        for (int i = 0; i < ship.size; i++) {
                            if (!(this->check_area(ship.x + i, ship.y))) return false;
                        }
                    }
                    else return false;
                }
                if (ship.orient == "n") {
                    if ((ship.y - ship.size + 1) >= 0) {
                        for (int i = 0; i < ship.size; i++) {
                            if (!(this->check_area(ship.x, ship.y - i))) return false;
                        }
                    }
                    else return false;
                }
                if (ship.orient == "w") {
                    if ((ship.x - ship.size + 1) >= 0) {
                        for (int i = 0; i < ship.size; i++) {
                            if (!(this->check_area(ship.x - i, ship.y))) return false;
                        }
                    }
                    else return false;
                }
                if (ship.orient == "s") {
                    if ((ship.y + ship.size - 1) < 10) {
                        for (int i = 0; i < ship.size; i++) {
                            if (!(this->check_area(ship.x, ship.y + i))) return false;
                        }
                    }
                    else return false;
                }
                return true;
            }
            bool add(Ship ship) {
                if (!(this->check_for_add(ship))) return false;
                std::string ship_key = std::to_string(ship.x) + std::to_string(ship.y);
                this->ships[ship_key] = ship;
                if (ship.orient == "e") {
                    for (int i = 0; i < ship.size; i++) {
                        battle_field[ship.x + i][ship.y].contains_ship = true;
                        battle_field[ship.x + i][ship.y].ship_key = ship_key;
                    }
                }
                if (ship.orient == "n") {
                    for (int i = 0; i < ship.size; i++) {
                        battle_field[ship.x][ship.y - i].contains_ship = true;
                        battle_field[ship.x][ship.y - i].ship_key = ship_key;
                    }
                }
                if (ship.orient == "w") {
                    for (int i = 0; i < ship.size; i++) {
                        battle_field[ship.x - i][ship.y].contains_ship = true;
                        battle_field[ship.x - i][ship.y].ship_key = ship_key;
                    }
                }
                if (ship.orient == "s") {
                    for (int i = 0; i < ship.size; i++) {
                        battle_field[ship.x][ship.y + i].contains_ship = true;
                        battle_field[ship.x][ship.y + i].ship_key = ship_key;
                    }
                }
                return true;
            }
            void shoot_around(int x, int y) {
                if (x < 9) battle_field[x + 1][y].shooted = true;
                if (x < 9 and y > 0) battle_field[x + 1][y - 1].shooted = true;
                if (y > 0) battle_field[x][y - 1].shooted = true;
                if (x > 0 and y > 0) battle_field[x - 1][y - 1].shooted = true;
                if (x > 0) battle_field[x - 1][y].shooted = true;
                if (x > 0 and y < 9) battle_field[x - 1][y + 1].shooted = true;
                if (y < 9) battle_field[x][y + 1].shooted = true;
                if (x < 9 and y < 9) battle_field[x + 1][y + 1].shooted = true;
            }
            void explode_ship(std::string ship_key) {
                Ship ship = this->ships[ship_key];
                int x = ship.x;
                int y = ship.y;
                if (ship.orient == "e") {
                    for (int i = 0; i < ship.size; i++) {
                        shoot_around(x + i, y);
                    }
                }
                if (ship.orient == "n") {
                    for (int i = 0; i < ship.size; i++) {
                        shoot_around(x, y - i);
                    }
                }
                if (ship.orient == "w") {
                    for (int i = 0; i < ship.size; i++) {
                        shoot_around(x - i, y);
                    }
                }
                if (ship.orient == "s") {
                    for (int i = 0; i < ship.size; i++) {
                        shoot_around(x, y + i);
                    }
                }
            }
        };
        std::string player_id;
        std::string player_name;
        std::string webhook;
        bool ready = false;
        int shoots = 4;
        Field field;
        Player(std::string id, std::string name, std::string webhook): player_id(id), player_name(name), webhook(webhook) {}
        
        int fill_field(std::vector<Ship> ships) {
            int count[4] = { 0,0,0,0 };
            for (Ship ship : ships) {
                if (ship.size > 0 && ship.size < 5) count[ship.size - 1]++;
                else return 1;  // incorrect ship size
            }
            if (!(count[0] == 4 && count[1] == 3 && count[2] == 2 && count[3] == 1)) return 2;  // wrong amount of ships
            for (Ship ship : ships) {
                if (!(this->field.add(ship))) {
                    this->field = Field();
                    return 3;  // wrong placement of ships
                }
            }
            return 0;
        }
        void print_field() {
            if (!ready) {
                std::cout << "no data\n";
                return;
            }
            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    if (field.battle_field[x][y].contains_ship) std::cout << "1";
                    else std::cout << "0";
                }
                std::cout << "\n";
            }
        }
    };
private:
    std::string game_id;
    std::vector <Player> players;
    std::vector <std::string> team_a{ "0", "0" };
    std::vector <std::string> team_b{ "0", "0" };
    bool started = false;
    int turn;  // 1, 2, 3, 4;
    std::string gen_id() {
        return id_up(last_id);
    }
public:
    static std::string last_id;
    Game() {
        game_id = gen_id();
        last_id = game_id;
        std::cout << "created game with ID: " << game_id << "\n";
    }
    std::string get_id() {
        return game_id;
    }
    int add_player(std::string player_id, std::string player_name, std::string player_webhook) {
        for (Player p : players) {
            if (p.player_id == player_id) return 2;
        }
        if (player_webhook[player_webhook.size()-1] == '/') player_webhook.erase(player_webhook.size()-1, 1);
        Player p(player_id, player_name, player_webhook);
        if (players.size() < 4) {
            players.push_back(p);
            return 0;
        }
        return 1;
    }
    void remove_player(std::string player_id) {
        for (std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it){
            if ((*it).player_id == player_id) {
                players.erase(it);
                break;
            }
        }
        if (this->team_a[0] == player_id) team_a[0] = "0";
        else if (this->team_a[1] == player_id) team_a[1] = "0";
        else if (this->team_b[0] == player_id) team_b[0] = "0";
        else if (this->team_b[1] == player_id) team_b[1] = "0";
    }
    int get_p_count() {
        return players.size();
    }
    std::string get_admin() {
        if (players.size()) return players[0].player_name;
        else return "";
    }
    int remove_from_team(std::string player_id) {
        for (Player& p : players) {
            if (p.player_id == player_id) {
                if (p.ready) return 1;
                break;
            }
        }
        if (team_a[0] == player_id) team_a[0] = "0";
        if (team_a[1] == player_id) team_a[1] = "0";
        if (team_b[0] == player_id) team_b[0] = "0";
        if (team_b[1] == player_id) team_b[1] = "0";
        return 0;
    }
    int add_to_team_a(std::string player_id, int num) {
        bool p_exist = false;
        for (Player& p : players) {
            if (p.player_id == player_id) {
                if (p.ready) return 4;
                p_exist = true;
                break;
            }
        }
        if (!p_exist) return 2;
        if (num > 1 || num < 0) return 3;
        this->remove_from_team(player_id);
        if (this->team_a[num] == "0") {
            this->team_a[num] = player_id;
            return 0;
        }
        return 1;
    }
    int add_to_team_b(std::string player_id, int num) {
        bool p_exist = false;
        for (Player& p : players) {
            if (p.player_id == player_id) {
                if (p.ready) return 4;
                p_exist = true;
                break;
            }
        }
        if (!p_exist) return 2;
        if (num > 1 || num < 0) return 3;
        this->remove_from_team(player_id);
        if (this->team_b[num] == "0") {
            this->team_b[num] = player_id;
            return 0;
        }
        return 1;
    }
    int set_ready(std::string player_id, std::vector<Ship> ships) {
        for (Player& p : players) {
            if (p.player_id == player_id) {
                if (player_id != team_a[0] && player_id != team_a[1]
                    && player_id != team_b[0] && player_id != team_b[1]) return 6;  // player not in team
                if (p.ready) return 5;  // player already ready
                p.field = Player::Field();
                int i = p.fill_field(ships);
                if (i == 0) p.ready = true;
                p.print_field();  ////  ////
                return i;
            }
        }
        return 4;  // no player in game
    }
    int deselect_ready(std::string player_id) {
        for (Player& p : players) {
            if (p.player_id == player_id) {
                p.ready = false;
                return 0;
            }
        }
        return 1;  // no player in game
    }
    bool all_ready() {
        return (players.size() == 4 
            && players[0].ready 
            && players[1].ready 
            && players[2].ready 
            && players[3].ready);
    }
    bool have_started() {
        return started;
    }
    void start_game() {
        started = true;
        turn = (rand() % 4) + 1;
    }
    std::vector <std::string> get_webhooks(std::string exclude = "") {
        std::vector <std::string> webhooks;
        for (Player p : players) {
            if (p.player_id != exclude) webhooks.push_back(p.webhook);
        }
        return webhooks;
    }
    json get_data_for_new_player() {
        json data = json::object();
        data["players"] = json::array();
        int i = 0;
        for (Player p : players) {
            data["players"].push_back(json::object());
            data["players"][i]["player_id"] = p.player_id;
            data["players"][i]["player_name"] = p.player_name;
            data["players"][i]["ready"] = p.ready;
            i++;
        }
        data["team_a"] = json::array();
        data["team_a"].push_back(team_a[0]);
        data["team_a"].push_back(team_a[1]);

        data["team_b"] = json::array();
        data["team_b"].push_back(team_b[0]);
        data["team_b"].push_back(team_b[1]);
        return data;
    }
    json get_teams_data() {
        json data = json::object();
        data["team_a"] = json::array();
        data["team_a"].push_back(team_a[0]);
        data["team_a"].push_back(team_a[1]);

        data["team_b"] = json::array();
        data["team_b"].push_back(team_b[0]);
        data["team_b"].push_back(team_b[1]);
        return data;
    }
    std::string get_turn() {
        switch (turn)
        {
        case (1):
            return team_a[0];
        case (2):
            return team_a[1];
        case (3):
            return team_b[0];
        case (4):
            return team_b[1];
        }
    }
    std::map <std::string, json> get_data_for_start() {  // {webhook: data}
        std::map <std::string, json> data;
        for (Player& p : players) {
            json j = json::object();
            j["friend"] = json::object();
            if (team_a[0] == p.player_id) j["friend"]["id"] = team_a[1];
            if (team_a[1] == p.player_id) j["friend"]["id"] = team_a[0];
            if (team_b[0] == p.player_id) j["friend"]["id"] = team_b[1];
            if (team_b[1] == p.player_id) j["friend"]["id"] = team_b[0];

            Player::Field friend_field;
            for (Player& fr_player : players) {
                if (fr_player.player_id == j["friend"]["id"]) {
                    friend_field = fr_player.field;
                    break;
                }
            }
            j["friend"]["ships"] = json::array();
            int i = 0;
            for (auto& ship : friend_field.ships) {
                j["friend"]["ships"].push_back(json::object());
                j["friend"]["ships"][i]["x"] = ship.second.x;
                j["friend"]["ships"][i]["y"] = ship.second.y;
                j["friend"]["ships"][i]["size"] = ship.second.size;
                j["friend"]["ships"][i]["orient"] = ship.second.orient;
                i++;
            }

            if (team_a[0] == p.player_id || team_a[1] == p.player_id) {
                j["enemy_1"] = team_b[0];
                j["enemy_2"] = team_b[1];
            }
            else {
                j["enemy_1"] = team_a[0];
                j["enemy_2"] = team_a[1];
            }
            j["turn"] = this->get_turn();
            data[p.webhook] = j;
        }
        return data;
    }
    void turn_up() {
        Player* player = nullptr;
        std::string player_id = "0";
        switch (turn)
        {
        case (1):
            turn = 3;
            player_id = team_b[0];
            break;
        case (2):
            turn = 4;
            player_id = team_b[1];
            break;
        case (3):
            turn = 2;
            player_id = team_a[1];
            break;
        case (4):
            turn = 1;
            player_id = team_a[0];
            break;
        }
        for (Player& p : players) {
            if (p.player_id == player_id) {
                player = &p;
                break;
            }
        }
        if (player == nullptr) throw 1;
        player->shoots = count_shoots(player_id);
        std::cout << "next turn: " << player_id << "\n";
        if (player->shoots == 0) turn_up();
    }
    int count_shoots(std::string player_id) {
        bool s4 = false;
        bool s3 = false;
        bool s2 = false;
        bool s1 = false;
        for (Player& p : players) {
            if (p.player_id == player_id) {
                for (auto ship : p.field.ships) {
                    switch (ship.second.size)
                    {
                    case (1):
                        if (ship.second.parts[0]) s1 = true;
                        break;
                    case (2):
                        if (ship.second.parts[0] || ship.second.parts[1]) s2 = true;
                        break;
                    case (3):
                        if (ship.second.parts[0] || ship.second.parts[1] || ship.second.parts[2]) s3 = true;
                        break;
                    case (4):
                        if (ship.second.parts[0] || ship.second.parts[1] || ship.second.parts[2] || ship.second.parts[3]) s4 = true;
                        break;
                    }
                }
                break;
            }
        }
        return int(s1) + int(s2) + int(s3) + int(s4);
    }
    int shoot(std::string player_id, std::string target_id, int x, int y) {
        std::cout << "shoot at " << x << " " << y << "\n";
        Player* player = nullptr;
        Player* target = nullptr;
        for (Player& p : players) {
            if (p.player_id == player_id) player = &p;
            if (p.player_id == target_id) target = &p;
        }
        if (player == nullptr) return 1;  // no player in game
        if (target == nullptr) return 2;  // incorrect target
        switch (turn)
        {
        case (1):
            if (team_a[0] != player_id) return 3;  // not your turn
            if (target_id != team_b[0] && target_id != team_b[1]) return 2;  // incorrect target
            break;
        case (2):
            if (team_a[1] != player_id) return 3;  // not your turn
            if (target_id != team_b[0] && target_id != team_b[1]) return 2;  // incorrect target
            break;
        case (3):
            if (team_b[0] != player_id) return 3;  // not your turn
            if (target_id != team_a[0] && target_id != team_a[1]) return 2;  // incorrect target
            break;
        case (4):
            if (team_b[1] != player_id) return 3;  // not your turn
            if (target_id != team_a[0] && target_id != team_a[1]) return 2;  // incorrect target
            break;
        }
        if (player->shoots == 0) {
            player->shoots = count_shoots(player_id);
            turn_up();
            return 4;  // player have not shoots
        }
        if (x < 0 || x > 9 || y < 0 || y > 9) return 5; // invalid coords
        
        if (target->field.battle_field[x][y].shooted) return 6;  // cell already shooted
        target->field.battle_field[x][y].shooted = true;
        if (target->field.battle_field[x][y].contains_ship) {
            std::cout << "hit: true\n";
            std::string ship_key = target->field.battle_field[x][y].ship_key;
            int dx = abs((ship_key[0] - '0') - x);  // dx = char_to_int("xy"[0]) - x
            if (dx != 0) {
                target->field.ships[ship_key].parts[dx] = false;
            }
            else {
                int dy = abs((ship_key[1] - '0') - y);  // dy = char_to_int("xy"[1]) - y
                target->field.ships[ship_key].parts[dy] = false;
            }
            bool dead = true;
            for (bool part : target->field.ships[ship_key].parts) {
                if (part) {
                    dead = false;
                    break;
                }
            }
            if (dead) {
                std::cout << "ship is dead\n";
                target->field.explode_ship(ship_key);
                //target->shoots = count_shoots(target_id);
                if (count_shoots(target_id) == 0) {
                    if (target_id == team_a[0]) {
                        if (count_shoots(team_a[1]) == 0) return -1;  // game end
                    }
                    else if (target_id == team_a[1]) {
                        if (count_shoots(team_a[0]) == 0) return -1;  // game end
                    }
                    else if (target_id == team_b[0]) {
                        if (count_shoots(team_b[1]) == 0) return -1;  // game end
                    }
                    else if (target_id == team_b[1]) {
                        if (count_shoots(team_b[0]) == 0) return -1;  // game end
                    }
                }
            }
            else std::cout << "ship isn't dead\n";
        }
        else std::cout << "hit: false\n";
        player->shoots--;
        if (player->shoots == 0) {
            //player->shoots = count_shoots(player_id);
            turn_up();
        }
        return 0;
    }
    json get_enemy_field(std::string player_id) {
        std::cout << "starting func get_field\n";  ////
        Player* player = nullptr;
        for (Player& p : players) {
            if (p.player_id == player_id) {
                player = &p;
                break;
            }
        }
        if (player == nullptr) throw 1;
        json field = json::object();
        field["battle_field"] = json::array();
        for (int x = 0; x < 10; x++) {
            field["battle_field"].push_back(json::array());
            for (int y = 0; y < 10; y++) {
                json j_cell = json::object();
                j_cell["x"] = x;
                j_cell["y"] = y;
                if (player->field.battle_field[x][y].shooted) {
                    j_cell["shooted"] = true;
                    j_cell["contains_ship"] = player->field.battle_field[x][y].contains_ship;
                }
                else {
                    j_cell["shooted"] = false;
                    j_cell["contains_ship"] = false;
                }

                field["battle_field"][x].push_back(j_cell);
            }
        }
        std::cout << "shoots field:\n";  ////
        for (int y = 0; y < 10; y++) {    ////
            for (int x = 0; x < 10; x++) {
                if (field["battle_field"][x][y]["shooted"]) std::cout << 'X';
                else std::cout << 'O';
            }
            std::cout << "\n";
        }  ////
        field["ships"] = json::array();
        for (auto& ship : player->field.ships) {
            switch (ship.second.size)
            {
            case(1):
                if (!ship.second.parts[0]) {
                    json j_ship = json::object();
                    j_ship["x"] = ship.second.x;
                    j_ship["y"] = ship.second.y;
                    j_ship["orient"] = ship.second.orient;
                    j_ship["size"] = 1;

                    field["ships"].push_back(j_ship);
                }
                break;
            case(2):
                if (!(ship.second.parts[0] || ship.second.parts[1])) {
                    json j_ship = json::object();
                    j_ship["x"] = ship.second.x;
                    j_ship["y"] = ship.second.y;
                    j_ship["orient"] = ship.second.orient;
                    j_ship["size"] = 2;

                    field["ships"].push_back(j_ship);
                }
                break;
            case(3):
                if (!(ship.second.parts[0] || ship.second.parts[1] || ship.second.parts[2])) {
                    json j_ship = json::object();
                    j_ship["x"] = ship.second.x;
                    j_ship["y"] = ship.second.y;
                    j_ship["orient"] = ship.second.orient;
                    j_ship["size"] = 3;

                    field["ships"].push_back(j_ship);
                }
                break;
            case(4):
                if (!(ship.second.parts[0] || ship.second.parts[1] || ship.second.parts[2] || ship.second.parts[3])) {
                    json j_ship = json::object();
                    j_ship["x"] = ship.second.x;
                    j_ship["y"] = ship.second.y;
                    j_ship["orient"] = ship.second.orient;
                    j_ship["size"] = 4;

                    field["ships"].push_back(j_ship);
                }
                break;
            }
        }
        std::cout << "ships: ";   ////
        for (int i = 0; i < field["ships"].size(); i++) {  ////
            std::cout << "(" << field["ships"][i]["x"] << ", " << field["ships"][i]["y"] << "), ";
        }
        std::cout << "end;\n";  ////
        return field;
    }
    std::string get_friend_webhook(std::string player_id) {
        std::string friend_id;
        if (player_id == team_a[0]) friend_id = team_a[1];
        if (player_id == team_a[1]) friend_id = team_a[0];
        if (player_id == team_b[0]) friend_id = team_b[1];
        if (player_id == team_b[1]) friend_id = team_b[0];
        for (Player& p : players) {
            if (p.player_id == friend_id) return p.webhook;
        }
        return "0";
    }
    std::vector <std::string> get_enemy_webhooks(std::string player_id) {
        std::vector<std::string> webhooks;
        if (player_id == team_a[0] || player_id == team_a[1]) {
            for (Player& p : players) {
                if (p.player_id == team_b[0] || p.player_id == team_b[1]) webhooks.push_back(p.webhook);
            }
        }
        else {
            for (Player& p : players) {
                if (p.player_id == team_a[0] || p.player_id == team_a[1]) webhooks.push_back(p.webhook);
            }
        }
        return webhooks;
    }
};
std::string Game::last_id = "00000000";

std::map <std::string, Game*> active_games;

int main() {
    srand(time(0));
    Server svr;
    svr.Get("/register", new_id_reg);
    svr.Get("/activegames", show_act_games);
    svr.Post("/create", create_game);
    svr.Post("/join", join_game);
    svr.Post("/exit", exit_game);
    svr.Post("/enterteam", enter_team);
    svr.Post("/quitteam", quit_team);
    svr.Post("/ready", set_ready);
    svr.Post("/notready", deselect_ready);
    svr.Post("/shoot", do_shoot);
    std::cout << "Starting server on \"http://localhost:1234/\" \n";
    svr.listen("localhost", 1234);
}

std::string id_up(std::string id) {
    bool finish = false;
    for (int i = 7; i >= 0; i--) {
        if (finish) break;
        switch (id[i])
        {
        case('0'):
        case('1'):
        case('2'):
        case('3'):
        case('4'):
        case('5'):
        case('6'):
        case('7'):
        case('8'):
        case('A'):
        case('B'):
        case('C'):
        case('D'):
        case('E'):
            id[i] += 1;
            finish = true;
            break;
        case('9'):
            id[i] = 'A';
            finish = true;
            break;
        case('F'):
            id[i] = '0';
        }
    }
    return id;
}

void new_id_reg(const Request& req, Response& res) {
    std::cout << "recived a request for register a new ID\n";
    std::ifstream id_reged_file("id_reged.json");
    json id_reged;
    if (id_reged_file.is_open()) {
        std::cout << "id_reged file is open\n";
        id_reged_file >> id_reged;
        id_reged_file.close();
        if (id_reged.size() > 0) {
            id_reged.push_back(json::object());
            id_reged[id_reged.size() - 1]["ID"] = id_up(id_reged[id_reged.size() - 2]["ID"]);
        }
        else {
            id_reged.push_back(json::object());
            id_reged[0]["ID"] = "00000000";
        }
        std::ofstream id_reged_file("id_reged.json");
        id_reged_file << std::setw(2) << id_reged;
        id_reged_file.close();
    }
    else {
        std::cout << "id_reged file is not open\n";
        id_reged = json::array();
        id_reged.push_back(json::object());
        id_reged[0]["ID"] = "00000000";
        std::ofstream acc_reged_file("id_reged.json");
        acc_reged_file << std::setw(2) << id_reged;
        acc_reged_file.close();
        std::cout << "new id_reged file created\n";
    }
    std::string id = id_reged[id_reged.size() - 1]["ID"];
    std::cout << "new ID added: " << id << "\n";
    res.set_content(id, "text/plain");
}

void show_act_games(const Request& req, Response& res) {
    std::cout << "req for show active games\n";
    json games = json::array();
    int i = 0;
    for (auto& game : active_games) {
        if ((*game.second).have_started()) continue;
        games.push_back(json::object());
        games[i]["admin_name"] = (*game.second).get_admin();
        games[i]["game_id"] = game.first;
        games[i]["p_count"] = std::to_string((*game.second).get_p_count());
        i++;
    }
    res.set_content(games.dump(), "text/json");
    std::cout << i << " games sent\n";
}

void create_game(const Request& req, Response& res) {
    std::cout << "req for creating game ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("player_name") && j_req.contains("webhook"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << "from " << j_req["player_id"] << "\n";
    Game* new_game = new Game;
    (*new_game).add_player(j_req["player_id"], j_req["player_name"], j_req["webhook"]);
    active_games[(*new_game).get_id()] = new_game;
    res.set_content("{\"game_id\":\"" + (*new_game).get_id() + "\"}", "text/json");
    std::cout << "game " << (*new_game).get_id() << " created\n";
}

void join_game(const Request& req, Response& res) {
    std::cout << "req for join game ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("player_name") && j_req.contains("webhook") && j_req.contains("game_id"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << j_req["game_id"] << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        int i = (*active_games[j_req["game_id"]]).add_player(j_req["player_id"], j_req["player_name"], j_req["webhook"]);
        if (i == 0) {
            res.set_content((*active_games[j_req["game_id"]]).get_data_for_new_player().dump(), "text/json");
            std::cout << "response sent\n";

            json req_on_webhooks;
            req_on_webhooks["player_id"] = j_req["player_id"];
            req_on_webhooks["player_name"] = j_req["player_name"];
            std::cout << "sending on webhooks... ";
            for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
                Client cli(webhook.c_str());
                auto res = cli.Post("/newplayer", req_on_webhooks.dump(), "application/json");
                // <- here must be check of res
            }
            std::cout << "OK\n";
        }
        if (i == 1) {
            std::cout << "game is full\n";
            res.set_content("{\"error\":\"game is full\"}", "text/json");
        }
        if (i == 2) {
            std::cout << "player already in game\n";
            res.set_content("{\"error\":\"player already in game\"}", "text/json");
        }
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void exit_game(const Request& req, Response& res) {
    std::cout << "req for exit game ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << j_req["game_id"] << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        active_games[j_req["game_id"]]->remove_player(j_req["player_id"]);
        res.set_content("ok", "text/plain");
        if ((*active_games[j_req["game_id"]]).get_p_count() == 0) {
            std::cout << "deleting game " << j_req["game_id"] << "\n";
            delete active_games[j_req["game_id"]];
            active_games.erase(j_req["game_id"]);
            return;
        }
        json req_on_webhooks;
        req_on_webhooks["player_id"] = j_req["player_id"];
        std::cout << "sending on webhooks... ";
        for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
            Client cli(webhook.c_str());
            auto res = cli.Post("/removeplayer", req_on_webhooks.dump(), "application/json");
            // <- here must be check of res
        }
        std::cout << "OK\n";
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void enter_team(const Request& req, Response& res) {
    std::cout << "req for enter team ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id") && j_req.contains("team") && j_req.contains("num"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << j_req["team"] << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        int i;
        if (j_req["team"] == "blue") i = active_games[j_req["game_id"]]->add_to_team_a(j_req["player_id"], j_req["num"]);
        else i = active_games[j_req["game_id"]]->add_to_team_b(j_req["player_id"], j_req["num"]);
        if (i == 0) {
            res.set_content("{}", "text/json");
            std::cout << "response sent\n";

            std::cout << "sending on webhooks... ";
            json req_on_webhooks = active_games[j_req["game_id"]]->get_teams_data();
            for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
                Client cli(webhook.c_str());
                auto res = cli.Post("/teamchange", req_on_webhooks.dump(), "application/json");
                // <- here must be check of res
            }
            std::cout << "OK\n";
            return;
        }
        if (i == 1) {
            std::cout << "place not free\n";
            res.set_content("{\"error\":\"place not free\"}", "text/json");
            return;
        }
        if (i == 2) {
            std::cout << "no player in game\n";
            res.set_content("{\"error\":\"no player in game\"}", "text/json");
            return;
        }
        std::cout << "error\n";
        res.set_content("{\"error\":\"error\"}", "text/json");
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void quit_team(const Request& req, Response& res) {
    std::cout << "req for quit team ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << j_req["team"] << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        int i = active_games[j_req["game_id"]]->remove_from_team(j_req["player_id"]);
        if (i == 0) {
            res.set_content("{}", "text/json");
            std::cout << "response sent\n";

            std::cout << "sending on webhooks... ";
            json req_on_webhooks = active_games[j_req["game_id"]]->get_teams_data();
            for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
                Client cli(webhook.c_str());
                auto res = cli.Post("/teamchange", req_on_webhooks.dump(), "application/json");
                // <- here must be check of res
            }
            std::cout << "OK\n";
            return;
        }
        if (i == 1) {
            std::cout << "unavailable while ready\n";
            res.set_content("{\"error\":\"unavailable while ready\"}", "text/json");
            return;
        }
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void set_ready(const Request& req, Response& res) {
    std::cout << "req for set ready ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id") && j_req.contains("ships"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        std::vector <Game::Ship> ships;
        for (int i = 0; i < j_req["ships"].size(); i++) {
            int x = j_req["ships"][i]["x"];
            int y = j_req["ships"][i]["y"];
            int size = j_req["ships"][i]["size"];
            std::string orient = j_req["ships"][i]["orient"];
            ships.push_back(Game::Ship(x, y, size, orient));
        }
        int i = active_games[j_req["game_id"]]->set_ready(j_req["player_id"], ships);
        if (i == 0) {
            res.set_content("{}", "text/json");
            std::cout << "response sent\n";

            if (active_games[j_req["game_id"]]->all_ready()) {
                std::cout << "starting game\n";
                active_games[j_req["game_id"]]->start_game();
                std::map <std::string, json> data_map = active_games[j_req["game_id"]]->get_data_for_start();
                for (auto data : data_map) {
                    Client cli(data.first.c_str());
                    auto res = cli.Post("/start", data.second.dump(), "application/json");
                    // <- here must be check of res
                }
                return;
            }

            std::cout << "sending on webhooks... ";
            json req_on_webhooks = json::object();
            req_on_webhooks["player_id"] = j_req["player_id"];
            req_on_webhooks["ready"] = true;
            for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
                Client cli(webhook.c_str());
                auto res = cli.Post("/readychange", req_on_webhooks.dump(), "application/json");
                // <- here must be check of res
            }
            std::cout << "OK\n";
            return;
        }
        if (i == 1) {
            std::cout << "incorrect ship size\n";
            res.set_content("{\"error\":\"incorrect ship size\"}", "text/json");
            return;
        }
        if (i == 2) {
            std::cout << "wrong amount of ships\n";
            res.set_content("{\"error\":\"wrong amount of ships\"}", "text/json");
            return;
        }
        if (i == 3) {
            std::cout << "wrong placement of ships\n";
            res.set_content("{\"error\":\"wrong placement of ships\"}", "text/json");
            return;
        }
        if (i == 4) {
            std::cout << "no player in game\n";
            res.set_content("{\"error\":\"no player in game\"}", "text/json");
            return;
        }
        if (i == 5) {
            std::cout << "player already ready\n";
            res.set_content("{\"error\":\"player already ready\"}", "text/json");
            return;
        }
        if (i == 6) {
            std::cout << "player not in team\n";
            res.set_content("{\"error\":\"player not in team\"}", "text/json");
            return;
        }
        std::cout << "error\n";
        res.set_content("{\"error\":\"error\"}", "text/json");
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void deselect_ready(const Request& req, Response& res) {
    std::cout << "req for deselect ready ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game have already started\n";
            res.set_content("{\"error\":\"game have already started\"}", "text/json");
            return;
        }
        int i = active_games[j_req["game_id"]]->deselect_ready(j_req["player_id"]);
        if (i == 0) {
            res.set_content("{}", "text/json");
            std::cout << "response sent\n";

            std::cout << "sending on webhooks... ";
            json req_on_webhooks = json::object();
            req_on_webhooks["player_id"] = j_req["player_id"];
            req_on_webhooks["ready"] = false;
            for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks(j_req["player_id"])) {
                Client cli(webhook.c_str());
                auto res = cli.Post("/readychange", req_on_webhooks.dump(), "application/json");
                // <- here must be check of res
            }
            std::cout << "OK\n";
            return;
        }
        if (i == 1) {
            std::cout << "no player in game\n";
            res.set_content("{\"error\":\"no player in game\"}", "text/json");
            return;
        }
        std::cout << "error\n";
        res.set_content("{\"error\":\"error\"}", "text/json");
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}

void do_shoot(const Request& req, Response& res) {
    std::cout << "req for shoot ";
    json j_req = json::parse(req.body);
    if (!(j_req.contains("player_id") && j_req.contains("game_id") 
        && j_req.contains("target") && j_req.contains("x") && j_req.contains("y"))) {
        std::cout << "\n invalid req\n";
        res.set_content("{\"error\":\"invalid req\"}", "text/json");
        return;
    }
    std::cout << " from " << j_req["player_id"] << "\n";
    if (active_games.find(j_req["game_id"]) != active_games.end()) {
        if (!active_games[j_req["game_id"]]->have_started()) {
            std::cout << "game not started\n";
            res.set_content("{\"error\":\"game not started\"}", "text/json");
            return;
        }
        int i = active_games[j_req["game_id"]]->shoot(j_req["player_id"], j_req["target"], j_req["x"], j_req["y"]);
        if (i == 0 || i == -1) {
            std::string turn = active_games[j_req["game_id"]]->get_turn();
            json j_res = json::object();
            j_res["field"] = active_games[j_req["game_id"]]->get_enemy_field(j_req["target"]);
            j_res["turn"] = turn;
            res.set_content(j_res.dump(), "text/json");
            std::cout << "response sent\n";

            j_res["enemy_id"] = j_req["target"];
            std::string webhook = active_games[j_req["game_id"]]->get_friend_webhook(j_req["player_id"]);
            Client cli(webhook.c_str());
            auto res = cli.Post("/friendshoot", j_res.dump(), "application/json");
            // <- here must be check of res
            std::cout << "data for friend sent\n";

            std::vector<std::string> en_webhooks = active_games[j_req["game_id"]]->get_enemy_webhooks(j_req["player_id"]);
            json req_to_enemys = json::object();
            req_to_enemys["target_id"] = j_req["target"];
            req_to_enemys["x"] = j_req["x"];
            req_to_enemys["y"] = j_req["y"];
            req_to_enemys["turn"] = turn;

            std::cout << "sending enemy 1\n";
            Client en1(en_webhooks[0].c_str());
            res = en1.Post("/enemyshoot", req_to_enemys.dump(), "application/json");
            // <- here must be check of res

            std::cout << "sending enemy 2\n";
            Client en2(en_webhooks[1].c_str());
            res = en2.Post("/enemyshoot", req_to_enemys.dump(), "application/json");
            // <- here must be check of res

            if (i == -1) {
                std::cout << "game ended\n";
                for (std::string webhook : (*active_games[j_req["game_id"]]).get_webhooks()) {
                    Client cli(webhook.c_str());
                    auto res = cli.Post("/endgame", "{}", "application/json");
                    // <- here must be check of res
                }
                delete active_games[j_req["game_id"]];
                active_games.erase(j_req["game_id"]);
            }

            return;
        }
        switch (i)
        {
        case(0): break;
        case(1):
        case(2):
        case(3):
        case(4):
        case(5):
        case(6):
        default:
            std::cout << "error\n";
            res.set_content("{\"error\":\"error\"}", "text/json");
            break;
        }
    }
    else {
        std::cout << "no such game\n";
        res.set_content("{\"error\":\"no such game\"}", "text/json");
    }
}
import tkinter as Tk
from PIL import Image, ImageTk, ImageEnhance
from flask import Flask, request
import requests
import threading
import json
import time


#==[ Classes ]=======
class Ship:
    def __init__(self, x, y, size, orient='e', ltl=False):
        self.x = x
        self.y = y
        self.size = size
        self.orient = orient
        self.parts = []
        for i in range(size):
            self.parts.append(True)

        angle = 0
        if orient == 'n': angle = 90
        elif orient == 'w': angle = 180
        elif orient == 's': angle = 270

        if not(ltl):
            if size == 1: self.img = ImageTk.PhotoImage(ship_1_img.rotate(angle, expand=True))
            elif size == 2: self.img = ImageTk.PhotoImage(ship_2_img.rotate(angle, expand=True))
            elif size == 3: self.img = ImageTk.PhotoImage(ship_3_img.rotate(angle, expand=True))
            else: self.img = ImageTk.PhotoImage(ship_4_img.rotate(angle, expand=True))
        else:
            if size == 1: self.img = ImageTk.PhotoImage(ltl_ship_1_img.rotate(angle, expand=True))
            elif size == 2: self.img = ImageTk.PhotoImage(ltl_ship_2_img.rotate(angle, expand=True))
            elif size == 3: self.img = ImageTk.PhotoImage(ltl_ship_3_img.rotate(angle, expand=True))
            else: self.img = ImageTk.PhotoImage(ltl_ship_4_img.rotate(angle, expand=True))

class Cell:
    def __init__(self, x, y, contains_ship=False):
        self.x = x
        self.y = y
        self.shooted = False
        self.contains_ship = contains_ship
        self.ship_key = None

class Field:
    def __init__(self):
        self.battle_field = []
        for x in range(10):
            self.battle_field.append([])
            for y in range(10):
                self.battle_field[x].append(Cell(x, y))
        self.ships = {}   # словарь содержит инфу о всех кораблях на поле ("xy": Ship)

    def check_for_add(self, ship):
        place_is_clear = True
        if ship.orient == 'e':
            if (ship.x + ship.size - 1) < 10:
                for i in range(ship.size):
                    if not(self.check_area(ship.x + i, ship.y)):
                        place_is_clear = False
                        break
            else: place_is_clear = False
        elif ship.orient == 'n':
            if (ship.y - ship.size + 1) >= 0:
                for i in range(ship.size):
                    if not(self.check_area(ship.x, ship.y - i)):
                        place_is_clear = False
                        break
            else: place_is_clear = False
        elif ship.orient == 'w':
            if (ship.x - ship.size + 1) >= 0:
                for i in range(ship.size):
                    if not(self.check_area(ship.x - i, ship.y)):
                        place_is_clear = False
                        break
            else: place_is_clear = False
        elif ship.orient == 's':
            if (ship.y + ship.size - 1) < 10:
                for i in range(ship.size):
                    if not(self.check_area(ship.x, ship.y + i)):
                        place_is_clear = False
                        break
            else: place_is_clear = False
        else: return False
        return place_is_clear

    def add(self, ship):
        if not(self.check_for_add(ship)): return
        ship_key = str(ship.x)+str(ship.y)
        self.ships[ship_key] = ship
        if ship.orient == 'e':
            for i in range(ship.size):
                self.battle_field[ship.x + i][ship.y].contains_ship = True
                self.battle_field[ship.x + i][ship.y].ship_key = ship_key
        elif ship.orient == 'n':
            for i in range(ship.size):
                self.battle_field[ship.x][ship.y - i].contains_ship = True
                self.battle_field[ship.x][ship.y - i].ship_key = ship_key
        elif ship.orient == 'w':
            for i in range(ship.size):
                self.battle_field[ship.x - i][ship.y].contains_ship = True
                self.battle_field[ship.x - i][ship.y].ship_key = ship_key
        elif ship.orient == 's':
            for i in range(ship.size):
                self.battle_field[ship.x][ship.y + i].contains_ship = True
                self.battle_field[ship.x][ship.y + i].ship_key = ship_key

    def check_area(self, x, y):
        area_is_clear = True
        if (self.battle_field[x][y].contains_ship): area_is_clear = False
        elif (x < 9) and (self.battle_field[x+1][y].contains_ship): area_is_clear = False
        elif (x < 9) and (y < 9) and (self.battle_field[x+1][y+1].contains_ship): area_is_clear = False
        elif (y < 9) and (self.battle_field[x][y+1].contains_ship): area_is_clear = False
        elif (x > 0) and (y < 9) and (self.battle_field[x-1][y+1].contains_ship): area_is_clear = False
        elif (x > 0) and (self.battle_field[x-1][y].contains_ship): area_is_clear = False
        elif (x > 0) and (y > 0) and (self.battle_field[x-1][y-1].contains_ship): area_is_clear = False
        elif (y > 0) and (self.battle_field[x][y-1].contains_ship): area_is_clear = False
        elif (x < 9) and (y > 0) and (self.battle_field[x+1][y-1].contains_ship): area_is_clear = False
        return area_is_clear

    def delete(self, ship):
        ship_key = str(ship.x)+str(ship.y)
        if ship_key in self.ships:
            if self.ships[ship_key] == ship:
                if ship.orient == 'e':
                    for i in range(ship.size):
                        self.battle_field[ship.x + i][ship.y].contains_ship = False
                        self.battle_field[ship.x + i][ship.y].ship_key = None
                elif ship.orient == 'n':
                    for i in range(ship.size):
                        self.battle_field[ship.x][ship.y - i].contains_ship = False
                        self.battle_field[ship.x][ship.y - i].ship_key = None
                elif ship.orient == 'w':
                    for i in range(ship.size):
                        self.battle_field[ship.x - i][ship.y].contains_ship = False
                        self.battle_field[ship.x - i][ship.y].ship_key = None
                elif ship.orient == 's':
                    for i in range(ship.size):
                        self.battle_field[ship.x][ship.y + i].contains_ship = False
                        self.battle_field[ship.x][ship.y + i].ship_key = None
                del self.ships[ship_key]

    def shoot(self, x, y):
        print("shoot at: ", x, y)
        if (x < 0 or x > 9 or y < 0 or y > 9): return
        self.battle_field[x][y].shooted = True
        if not(self.battle_field[x][y].contains_ship):
            print("cell not contains ship")
            return
        ship_key = self.battle_field[x][y].ship_key
        if ship_key == None:
            print("key is None")
            return
        else: print("key:",ship_key)
        dx = abs(int(ship_key[0]) - x)
        if dx != 0:
            self.ships[ship_key].parts[dx] = False
        else:
            dy = abs(int(ship_key[1]) - y)
            self.ships[ship_key].parts[dy] = False
        dead = True
        for part in self.ships[ship_key].parts:
            if part: dead = False
        if dead:
            def shoot_around(x, y):
                if x < 9: self.battle_field[x + 1][y].shooted = True
                if x < 9 and y > 0: self.battle_field[x + 1][y - 1].shooted = True
                if y > 0: self.battle_field[x][y - 1].shooted = True
                if x > 0 and y > 0: self.battle_field[x - 1][y - 1].shooted = True
                if x > 0: self.battle_field[x - 1][y].shooted = True
                if x > 0 and y < 9: self.battle_field[x - 1][y + 1].shooted = True
                if y < 9: self.battle_field[x][y + 1].shooted = True
                if x < 9 and y < 9: self.battle_field[x + 1][y + 1].shooted = True
                
            ship = self.ships[ship_key]
            if ship.orient == 'e':
                for i in range(ship.size):
                    shoot_around(ship.x + i, ship.y)
            elif ship.orient == 'n':
                for i in range(ship.size):
                    shoot_around(ship.x, ship.y - i)
            elif ship.orient == 'w':
                for i in range(ship.size):
                    shoot_around(ship.x - i, ship.y)
            elif ship.orient == 's':
                for i in range(ship.size):
                    shoot_around(ship.x, ship.y + i)

    def print_field(self):
        for y in range(10):
            for x in range(10):
                if self.battle_field[x][y].contains_ship: print(1, end='')
                else: print(0, end='')
            print()

class ChosenShip:
    def __init__(self):
        self.exist = False
        self.on_field = False
        self.position = None   # "xy" or Canvas() object
#------------

#==[ Globals ]=======
player_ID = ""
player_name = ""
svr_URL = ""
webhook_URL = ""
need_req = False

game_ID = ""
red_team = False
ready = False

turn = ""
#------------

#==[ clic functions]=======
def clic_play(event):
    global lbl_main_page, lbl_choose_game_page, player_ID, player_name, svr_URL, webhook_URL, need_req
    player_name = ent_nickname.get()
    svr_URL = ent_svr_URL.get()
    webhook_URL = ent_webhook_URL.get()
    if (player_name != "") and (svr_URL != "") and (webhook_URL != ""):
        if svr_URL[-1] != '/': svr_URL += '/'
        if need_req:
            try: res = requests.get(svr_URL + 'register')
            except:
                print("can't make a request")
                return
            player_ID = res.text
            need_req = False
            data_json = {}
            data_json["ID"] = player_ID
        else:
            data_file = open("game_data.json", 'r')
            data_json = json.load(data_file)
            data_file.close()
        data_json["name"] = player_name
        data_json["server"] = svr_URL
        data_json["webhook"] = webhook_URL
        data_file = open("game_data.json", 'w')
        json.dump(data_json, data_file)
        data_file.close()

        upd_active_games()
        lbl_main_page.pack_forget()
        lbl_choose_game_page.pack()
    else: show_messege("Fill in all the fields!")

def clic_back_to_p1(event):
    global lbl_main_page, lbl_choose_game_page
    lbl_choose_game_page.pack_forget()
    lbl_main_page.pack()

def clic_upd_games(event):
    upd_active_games()

def clic_join_with_id(event):
    join_game(ent_game_id.get())

def clic_join_active(event):
    join_game(event.widget.game_id)

def clic_create(event):
    global lbl_choose_game_page, lbl_preparation_page, game_ID, prepare_field, chosen_ship
    try:
        res = requests.post(svr_URL+'create', json={"player_id":player_ID, "player_name":player_name, "webhook":webhook_URL})
    except:
        print("can't make a request")
        return
    if res.status_code != 200: return
    j_res = json.loads(res.text)
    if "error" in j_res:
        print(j_res['error'])
        return
    clear_preparation_page()
    game_ID = j_res['game_id']
    list_players[0]['text'] = player_name
    list_players[0].player_id = player_ID
    lbl_game_id['text'] = "ID: " + game_ID

    lbl_choose_game_page.pack_forget()
    lbl_preparation_page.pack()

def clic_back_to_p2(event):
    if ready:
        print("unavailable while ready")
        return
    requests.post(svr_URL+'exit', json={"player_id":player_ID, "game_id":game_ID})
    upd_active_games()
    lbl_preparation_page.pack_forget()
    lbl_choose_game_page.pack()

def clic_enter_to_team(event):
    global list_team_members, red_team
    if ready:
        print("unavailable while ready")
        return
    if event.widget.member_id == player_ID:
        try:
            res = requests.post(svr_URL+'quitteam', json={"player_id":player_ID, "game_id":game_ID})
        except:
            print("can't make a request")
            return   ###### <--- exit game
        if res.status_code != 200: return   ###### <--- exit game

        j_res = json.loads(res.text)
        if "error" in j_res:
            print(j_res['error'])
            return   ###### <--- exit game

        event.widget.member_id = "0"
        event.widget['text'] = ". . ."

    elif event.widget.member_id == "0":
        if event.widget.num > 1: team = "red"
        else: team = "blue"
        num = event.widget.num % 2
        try:
            res = requests.post(svr_URL+'enterteam', json={"player_id":player_ID, "game_id":game_ID, "team":team, "num":num})
        except:
            print("can't make a request")
            return   ###### <--- exit game
        if res.status_code != 200: return   ###### <--- exit game

        j_res = json.loads(res.text)
        if "error" in j_res:
            print(j_res['error'])
            return   ###### <--- exit game

        for lbl in list_team_members:
            if lbl.member_id == player_ID:
                lbl['text'] = ". . ."
                lbl.member_id = "0"
        event.widget.member_id = player_ID
        event.widget['text'] = player_name
        if event.widget.num == 2 or event.widget.num == 3: red_team = True
        else: red_team = False

def clic_prepare_cnv(event):
    global chosen_ship, prepare_field, chosen_border_img
    x = event.x - 2
    y = event.y - 2
    if x >= 0 and x <= 299 and y >= 0 and y <= 299:
        cell_x = x // 30
        cell_y = y // 30
        cell = prepare_field.battle_field[cell_x][cell_y]
        if cell.contains_ship and ready:
            print("unavailable while ready")
            return

        draw_border = False
        if chosen_ship.exist:
            if cell.contains_ship:
                if cell.ship_key == chosen_ship.position: # delete chosen
                    chosen_ship.exist = False
                    draw_field(cnv_prepare_field, prepare_field)
                else: # set new chosen
                    if chosen_ship.on_field:
                        draw_field(cnv_prepare_field, prepare_field)
                        chosen_ship.position = cell.ship_key
                        draw_border = True
                    else:
                        chosen_ship.position['highlightbackground'] = "#3F49CC"
                        chosen_ship.on_field = True
                        chosen_ship.position = cell.ship_key
                        draw_border = True
            else: # move
                if chosen_ship.on_field:
                    old_ship = prepare_field.ships[chosen_ship.position]
                    new_ship = Ship(cell_x, cell_y, old_ship.size, old_ship.orient)
                    prepare_field.delete(old_ship)
                    if prepare_field.check_for_add(new_ship):
                        chosen_ship.exist = False
                        prepare_field.add(new_ship)
                        draw_field(cnv_prepare_field, prepare_field)
                    else:
                        prepare_field.add(old_ship)
                        bad_place(new_ship)
                else:
                    new_ship = Ship(cell_x, cell_y, chosen_ship.position.ship_size)
                    if prepare_field.check_for_add(new_ship):
                        chosen_ship.position['highlightbackground'] = "#3F49CC"
                        chosen_ship.position.contains_ship = False
                        chosen_ship.position.delete("all")
                        chosen_ship.exist = False
                        prepare_field.add(new_ship)
                        draw_field(cnv_prepare_field, prepare_field)
                    else: bad_place(new_ship)
        else:
            if cell.contains_ship: # set chosen
                chosen_ship.exist = True
                chosen_ship.on_field = True
                chosen_ship.position = cell.ship_key
                draw_border = True

        if draw_border:
            img = ImageTk.getimage(prepare_field.ships[cell.ship_key].img)
            chosen_border_img = ImageTk.PhotoImage(create_border_img(img))
            img_x, img_y = coords_for_img(prepare_field.ships[cell.ship_key])
            cnv_prepare_field.create_image(img_x, img_y, image=chosen_border_img)           

def clic_right_prepare_cnv(event):
    x = event.x - 2
    y = event.y - 2
    if x >= 0 and x <= 299 and y >= 0 and y <= 299:
        cell_x = x // 30
        cell_y = y // 30
    else: return

    if prepare_field.battle_field[cell_x][cell_y].contains_ship:
        if ready:
            print("unavailable while ready")
            return

        ship_key = prepare_field.battle_field[cell_x][cell_y].ship_key
        old_ship = prepare_field.ships[ship_key]
        if old_ship.orient == 'e': new_ship = Ship(old_ship.x, old_ship.y, old_ship.size, 'n')
        elif old_ship.orient == 'n': new_ship = Ship(old_ship.x, old_ship.y, old_ship.size, 'w')
        elif old_ship.orient == 'w': new_ship = Ship(old_ship.x, old_ship.y, old_ship.size, 's')
        else: new_ship = Ship(old_ship.x, old_ship.y, old_ship.size, 'e')

        prepare_field.delete(old_ship)
        if prepare_field.check_for_add(new_ship):
            prepare_field.add(new_ship)
            if chosen_ship.exist and chosen_ship.on_field: chosen_ship.exist = False
            draw_field(cnv_prepare_field, prepare_field)
        else:
            prepare_field.add(old_ship)
            bad_place(new_ship)

def clic_cnv_ship(event):
    global chosen_ship, prepare_field, chosen_border_img
    if chosen_ship.exist:
        if chosen_ship.on_field:
            if event.widget.contains_ship:
                draw_field(cnv_prepare_field, prepare_field)
                chosen_ship.on_field = False
                chosen_ship.position = event.widget
                event.widget['highlightbackground'] = "#FF0000"
            else:
                if event.widget.ship_size == prepare_field.ships[chosen_ship.position].size:
                    chosen_ship.exist = False
                    prepare_field.delete(prepare_field.ships[chosen_ship.position])
                    draw_field(cnv_prepare_field, prepare_field)
                    event.widget.contains_ship = True
                    if event.widget.ship_size == 4: event.widget.create_image(60, 16, image=ship_4_img_tk)
                    elif event.widget.ship_size == 3: event.widget.create_image(45, 16, image=ship_3_img_tk)
                    elif event.widget.ship_size == 2: event.widget.create_image(30, 16, image=ship_2_img_tk)
                    else: event.widget.create_image(16, 16, image=ship_1_img_tk)
        else:
            if event.widget == chosen_ship.position:
                chosen_ship.exist = False
                event.widget['highlightbackground'] = "#3F49CC"
            else:
                if event.widget.contains_ship:
                    chosen_ship.position['highlightbackground'] = "#3F49CC"
                    chosen_ship.position = event.widget
                    event.widget['highlightbackground'] = "#FF0000"
    else:
        if event.widget.contains_ship:
            chosen_ship.exist = True
            chosen_ship.on_field = False
            chosen_ship.position = event.widget
            event.widget['highlightbackground'] = "#FF0000"

def clic_lbl_ready(event):
    global ready
    if ready:
        try:
            res = requests.post(svr_URL+'notready', json={"player_id":player_ID, "game_id":game_ID})
        except:
            print("can't make a request")
            return
        if res.status_code != 200: return
        j_res = json.loads(res.text)
        if "error" in j_res:
            print(j_res['error'])
            return

        event.widget['image'] = ready_img
        ready = False
        for lbl in list_players:
            if lbl.player_id == player_ID:
                lbl['bg'] = "#3A43CC"
                break
        return

    if (len(prepare_field.ships) != 10):
        print("not all ships planted")
        return
    in_team = False
    for lbl in list_team_members:
        if lbl.member_id == player_ID:
            in_team = True
            break
    if not(in_team):
        print("you are not in team")
        return

    j_ships = []
    for ship in prepare_field.ships.items():
        j_ships.append({"x":ship[1].x, "y":ship[1].y, "size":ship[1].size, "orient":ship[1].orient})
    try:
        res = requests.post(svr_URL+'ready', json={"player_id":player_ID, "game_id":game_ID, "ships":j_ships})
    except:
        print("can't make a request")
        return
    if res.status_code != 200: return
    j_res = json.loads(res.text)
    if "error" in j_res:
        print(j_res['error'])
        return
    event.widget['image'] = ready_set_img
    ready = True
    for lbl in list_players:
        if lbl.player_id == player_ID:
            lbl['bg'] = "green"
            break

def clic_enemy_field(event):
    global turn
    if turn != player_ID:
        print("it is not your turn")
        return
    if not(cnv_shoots[0].status == 1 
    	or cnv_shoots[1].status == 1 
    	or cnv_shoots[2].status == 1 
    	or cnv_shoots[3].status == 1): return

    x = event.x - 2
    y = event.y - 2
    if not(x >= 0 and x <= 249 and y >= 0 and y <= 249): return
    cell_x = x // 25
    cell_y = y // 25
    if event.widget.num == 1: field = enemy_1_field
    else: field = enemy_2_field
    if field.battle_field[cell_x][cell_y].shooted: return

    try:
        res = requests.post(svr_URL+'shoot', 
            json={"player_id":player_ID, "game_id":game_ID, "target":event.widget.player_id, "x":cell_x, "y":cell_y})
    except:
        print("can't make a request")
        return
    if res.status_code != 200: return
    j_res = json.loads(res.text)
    if "error" in j_res:
        print(j_res['error'])
        return

    upd_enemy_field(field, j_res["field"])

    if event.widget.num == 1: draw_field(cnv_enemy_1_field, field, ltl=True)
    else: draw_field(cnv_enemy_2_field, field, ltl=True)

    turn = j_res['turn']
    if turn == player_ID:
        for cnv in cnv_shoots:
            if cnv.status == 1:
                cnv.status = 0
                cnv.delete("all")
                break
    else:
        for cnv in cnv_shoots:
            if cnv.status == 0:
                cnv.status = 1
                cnv.create_image(14, 14, image=shoot_img)

        lbl_names[2]['bg'] = lbl_names[2]['fg']
        for lbl in lbl_names:
            if lbl.player_id == turn:
                lbl['bg'] = "green"
                break

#------------

#==[ Functions ]=====
def join_game(game_id):
    global lbl_choose_game_page, lbl_preparation_page, game_ID
    if len(game_id) != 8:
        print("wrong id")
        return
    try:
        res = requests.post(svr_URL+'join', json={"player_id":player_ID, "player_name":player_name, "webhook":webhook_URL, "game_id":game_id})
    except:
        print("can't make a request")
        return
    if res.status_code != 200: return
    j_res = json.loads(res.text)
    if "error" in j_res:
        print(j_res['error'])
        return

    clear_preparation_page()
    game_ID = game_id
    lbl_game_id['text'] = "ID: " + game_id
    i = 0
    for player in j_res["players"]:
        list_players[i]['text'] = player["player_name"]
        list_players[i].player_id = player["player_id"]
        i += 1
    if j_res["team_a"][0] != "0":
        list_team_members[0].player_id = j_res["team_a"][0]
        list_team_members[0]['text'] = get_name(j_res["team_a"][0])

    if j_res["team_a"][1] != "0":
        list_team_members[1].player_id = j_res["team_a"][1]
        list_team_members[1]['text'] = get_name(j_res["team_a"][1])

    if j_res["team_b"][0] != "0":
        list_team_members[2].player_id = j_res["team_b"][0]
        list_team_members[2]['text'] = get_name(j_res["team_b"][0])

    if j_res["team_b"][1] != "0":
        list_team_members[3].player_id = j_res["team_b"][1]
        list_team_members[3]['text'] = get_name(j_res["team_b"][1])

    lbl_choose_game_page.pack_forget()
    lbl_preparation_page.pack()

def get_name(player_id):
    for lbl in list_players:
        if lbl.player_id == player_id: return lbl['text']

def draw_field(cnv, field, ltl=False):
    cnv.delete("all")
    if ltl:
        cell = 25
    else:
        cell = 30
    for i in range(10):
        cnv.create_line((cell * i, 2), (cell * i, cell * 10 + 2), fill='#00A2E8')
        cnv.create_line((2, cell * i), (cell * 10 + 2, cell * i), fill='#00A2E8')
    for ship in field.ships.items():
        x, y = coords_for_img(ship[1], cell)
        cnv.create_image(x, y, image=ship[1].img)
    for x in range(10):
        for y in range(10):
            if field.battle_field[x][y].shooted:
                if field.battle_field[x][y].contains_ship:
                    cnv.create_image(x*cell + (cell // 2) + 1, y*cell + (cell // 2) + 1, image=shooted_ship_img)
                else:
                    cnv.create_image(x*cell + (cell // 2) + 1, y*cell + (cell // 2) + 1, image=shooted_cell_img)

def bad_place(ship):
    img = ImageTk.getimage(ship.img)
    img.putalpha(96)
    watery_img = ImageTk.PhotoImage(img)
    x, y = coords_for_img(ship)
    tmp = cnv_prepare_field.create_image(x, y, image=watery_img)
    root.update()
    root.after(500, cnv_prepare_field.delete(tmp))

def coords_for_img(ship, cell=30):
    if ship.orient == 'e':
        x, y = ship.x * cell + (cell//2) * ship.size - 1, ship.y * cell + (cell//2)
    elif ship.orient == 'n':
        x, y = ship.x * cell + (cell//2), ship.y * cell - (cell//2) * ship.size + cell + 1
    elif ship.orient == 'w':
        x, y = ship.x * cell - (cell//2) * ship.size + cell + 1, ship.y * cell + (cell//2) + 1
    else:
        x, y = ship.x * cell + (cell//2), ship.y * cell + (cell//2) * ship.size - 1
    return (x, y)

def create_border_img(img):
    width, height = img.size
    img = Image.new("RGBA", (width, height), '#00000000')
    matrix = img.load()
    for i in range(width):
        matrix[i, 0] = (255, 0, 0, 255)
        matrix[i, 1] = (255, 0, 0, 255)
        matrix[i, height-1] = (255, 0, 0, 255)
        matrix[i, height-2] = (255, 0, 0, 255)

    for i in range(height):
        matrix[0, i] = (255, 0, 0, 255)
        matrix[1, i] = (255, 0, 0, 255)
        matrix[width-1, i] = (255, 0, 0, 255)
        matrix[width-2, i] = (255, 0, 0, 255)
    return img

def clear_preparation_page():
    global prepare_field, chosen_ship
    lbl_game_id['text'] = ""
    for i in range(4):
        list_players[i].player_id = "0"
        list_players[i]['text'] = ". . ."
    for i in range(4):
        list_team_members[i].member_id = "0"
        list_team_members[i]['text'] = ". . ."

    cnv_4_ship.create_image(60, 16, image=ship_4_img_tk)
    cnv_4_ship.contains_ship = True
    for cnv in cnv_3_ships:
        cnv.create_image(45, 16, image=ship_3_img_tk)
        cnv.contains_ship = True
    for cnv in cnv_2_ships:
        cnv.create_image(30, 16, image=ship_2_img_tk)
        cnv.contains_ship = True
    for cnv in cnv_1_ships:
        cnv.create_image(15, 16, image=ship_1_img_tk)
        cnv.contains_ship = True

    chosen_ship = ChosenShip()
    prepare_field = Field()
    draw_field(cnv_prepare_field, prepare_field)

def upd_active_games():
    try:
        res = requests.get(svr_URL+'activegames')
    except:
        print("can't make a request")
        return
    if res.status_code != 200: return
    j_res = json.loads(res.text)

    for i in range(5):
        list_active_games[i]['text'] = ". . . . ."
        list_active_games[i].game_id = "0"

    for i in range(len(j_res)):
        if i > 4: break
        text = j_res[i]["admin_name"] + " " * (13-len(j_res[i]["admin_name"])) + " ID: " + j_res[i]["game_id"] + "   " + j_res[i]["p_count"] + "/4"
        list_active_games[i]['text'] = text
        list_active_games[i].game_id = j_res[i]["game_id"]

def upd_enemy_field(en_field, j_field):
    for x in range(10):
        for y in range(10):
            shooted = j_field["battle_field"][x][y]["shooted"]
            contains_ship = j_field["battle_field"][x][y]["contains_ship"]
            en_field.battle_field[x][y].shooted = shooted
            en_field.battle_field[x][y].contains_ship = contains_ship
    en_field.ships = {}
    for ship in j_field["ships"]:
        new_ship = Ship(ship['x'], ship['y'], ship['size'], ship['orient'], ltl=True)
        en_field.ships[str(ship['x'])+str(ship['y'])] = new_ship

def show_messege(msg, command=lambda event: lbl_messege.place_forget()):
    lbl_messege['text'] = msg
    lbl_messege.place(x=220, y=200)
    lbl_messege.bind("<Button-1>", command)

#------------

root = Tk.Tk()
root.title("Sea battle 2x2")
root.resizable(False, False)
root.geometry("800x630")

#==[ Main page ]==========
main_page_bgr = ImageTk.PhotoImage(file="main_page_bgr.png")
play_img = ImageTk.PhotoImage(file="play_img.png")
nickname_img = ImageTk.PhotoImage(file="nickname_img.png")

lbl_main_page = Tk.Label(root, image=main_page_bgr)
lbl_main_page.pack()

lbl_svr_URL = Tk.Label(root, text="Server URL:", font=1)
lbl_svr_URL.place(in_=lbl_main_page, x=10, y=10)

ent_svr_URL = Tk.Entry(root, font=1)
ent_svr_URL.place(in_=lbl_main_page, x=10, y=35)

lbl_webhook_URL = Tk.Label(root, text="Your webhook URL:", font=1)
lbl_webhook_URL.place(in_=lbl_main_page, x=10, y=60)

ent_webhook_URL = Tk.Entry(root, font=1)
ent_webhook_URL.place(in_=lbl_main_page, x=10, y=85)

lbl_nickname = Tk.Label(root, image=nickname_img, bg='#3F49CC')
lbl_nickname.place(in_=lbl_main_page, x=10, y=180)

ent_nickname = Tk.Entry(root, font="Helvetica 26", width=15, fg='#3F49CC', bg='#50B5D4')
ent_nickname.place(in_=lbl_main_page, x=10, y=230)

lbl_play = Tk.Label(root, image=play_img, bg='#3F49CC')
lbl_play.place(in_=lbl_main_page, x=20, y=300)
lbl_play.bind("<Button-1>", clic_play)
#-----------

#==[ Choose game page ]==========
choose_game_page_bgr = ImageTk.PhotoImage(file="choose_game_page_bgr.png")
active_games_img = ImageTk.PhotoImage(file="active_games_img.png")
update_arrows_img = ImageTk.PhotoImage(file="update_arrows_img.png")
back_arrow_img = ImageTk.PhotoImage(file="back_arrow_img.png")
game_id_mini_img = ImageTk.PhotoImage(file="game_id_mini_img.png")
join_img = ImageTk.PhotoImage(file="join_img.png")
create_img = ImageTk.PhotoImage(file="create_img.png")

act_game_img = ImageTk.PhotoImage(Image.new("RGB", (380, 50), '#7092BE'))

lbl_choose_game_page = Tk.Label(root, image=choose_game_page_bgr)

lbl_active_games = Tk.Label(root, image=active_games_img, bg='#3F49CC')
lbl_active_games.place(in_=lbl_choose_game_page, x=200, y=10)

lbl_update_games = Tk.Label(root, image=update_arrows_img, bg='#3F49CC')
lbl_update_games.place(in_=lbl_choose_game_page, x=630, y=15)
lbl_update_games.bind("<Button-1>", clic_upd_games)

lbl_back_to_p1 = Tk.Label(root, image=back_arrow_img, bg='#3F49CC')
lbl_back_to_p1.place(in_=lbl_choose_game_page, x=10, y=10)
lbl_back_to_p1.bind("<Button-1>", clic_back_to_p1)

lbl_game_id_mini = Tk.Label(root, image=game_id_mini_img, bg='#3A43CC')
lbl_game_id_mini.place(in_=lbl_choose_game_page, x=10, y=100)

ent_game_id = Tk.Entry(root, font="Helvetica 15", width=12, fg='#3F49CC', bg='#50B5D4')
ent_game_id.place(in_=lbl_choose_game_page, x=10, y=150)

lbl_join = Tk.Label(root, image=join_img, bg='#3F49CC')
lbl_join.place(in_=lbl_choose_game_page, x=10, y=190)
lbl_join.bind("<Button-1>", clic_join_with_id)

lbl_create = Tk.Label(root, image=create_img, bg='#3F49CC')
lbl_create.place(in_=lbl_choose_game_page, x=640, y=100)
lbl_create.bind("<Button-1>", clic_create)

list_active_games = []

for i in range(5):
    list_active_games.append(Tk.Label(root, image=act_game_img, bg='#3A43CC', fg='#3F49CC', text=". . . . .", font="Helvetica 15", compound='center'))
    list_active_games[i].place(in_=lbl_choose_game_page, x=210, y=(120 + i*60))
    list_active_games[i].bind("<Button-1>", clic_join_active)
    list_active_games[i].game_id = "0"
#-----------

#==[ Preparation page ]==========
preparation_page_bgr = ImageTk.PhotoImage(file="preparation_page_bgr.png")
blue_team_img = ImageTk.PhotoImage(file="blue_team_img.png")
red_team_img = ImageTk.PhotoImage(file="red_team_img.png")

ships_tileset = Image.open("ships.png").convert('RGBA')
ship_1_img = ships_tileset.crop((1, 1, 30, 30))
ship_2_img = ships_tileset.crop((31, 1, 90, 30))
ship_3_img = ships_tileset.crop((91, 1, 180, 30))
ship_4_img = ships_tileset.crop((181, 1, 300, 30))

ship_1_img_tk = ImageTk.PhotoImage(ship_1_img)
ship_2_img_tk = ImageTk.PhotoImage(ship_2_img)
ship_3_img_tk = ImageTk.PhotoImage(ship_3_img)
ship_4_img_tk = ImageTk.PhotoImage(ship_4_img)

team_member_img = ImageTk.PhotoImage(Image.new("RGB", (180, 50), '#7092BE'))

ready_img = ImageTk.PhotoImage(file="ready_img.png")
ready_set_img = ImageTk.PhotoImage(file="ready_set_img.png")

game_id_img = ImageTk.PhotoImage(Image.new("RGB", (180, 25), '#7092BE'))

lbl_preparation_page = Tk.Label(root, image=preparation_page_bgr)

lbl_back_to_p2 = Tk.Label(root, image=back_arrow_img, bg='#3F49CC')
lbl_back_to_p2.place(in_=lbl_preparation_page, x=10, y=10)
lbl_back_to_p2.bind("<Button-1>", clic_back_to_p2)

lbl_game_id = Tk.Label(root, image=game_id_img, bg='#3A43CC', fg='#3F49CC', text="", font="Helvetica 15", compound='center')
lbl_game_id.place(in_=lbl_preparation_page, x=600, y=585)

list_players = []

for i in range(4):
    list_players.append(Tk.Label(root, image=game_id_img, bg='#3A43CC', fg='#3F49CC', font="Helvetica 15", compound='center'))
    list_players[i].place(in_=lbl_preparation_page, x=600, y=220+i*32)
    list_players[i].player_id = "0"

lbl_team_a = Tk.Label(root, image=blue_team_img, bg='#3F49CC')
lbl_team_a.place(in_=lbl_preparation_page, x=170, y=20)

lbl_team_b = Tk.Label(root, image=red_team_img, bg='#3F49CC')
lbl_team_b.place(in_=lbl_preparation_page, x=470, y=20)

list_team_members = []

for i in range(4):
    list_team_members.append(Tk.Label(root, image=team_member_img, bg='#3F49CC', fg='#3F49CC', font=('Helvetica', 22, 'bold'), compound='center', text=". . ."))
    list_team_members[i].bind("<Button-1>", clic_enter_to_team)
    list_team_members[i].member_id = "0"
    list_team_members[i].num = i

list_team_members[2]['fg'] = '#940C1D'
list_team_members[3]['fg'] = '#940C1D'

list_team_members[0].place(in_=lbl_preparation_page, x=160, y=80)
list_team_members[1].place(in_=lbl_preparation_page, x=160, y=130)
list_team_members[2].place(in_=lbl_preparation_page, x=460, y=80)
list_team_members[3].place(in_=lbl_preparation_page, x=460, y=130)

cnv_prepare_field = Tk.Canvas(root, width=300, height=300, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_prepare_field.place(in_=lbl_preparation_page, x=60, y=250)
cnv_prepare_field.bind("<Button-1>", clic_prepare_cnv)
cnv_prepare_field.bind("<Button-3>", clic_right_prepare_cnv)

cnv_4_ship = Tk.Canvas(root, width=120, height=30, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_4_ship.place(in_=lbl_preparation_page, x=380, y=270)
cnv_4_ship.create_image(60, 16, image=ship_4_img_tk)
cnv_4_ship.bind("<Button-1>", clic_cnv_ship)
cnv_4_ship.contains_ship = True
cnv_4_ship.ship_size = 4

cnv_3_ships = []
for i in range(2):
    cnv_3_ships.append(Tk.Canvas(root, width=90, height=30, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_3_ships[i].place(in_=lbl_preparation_page, x=380 + 110*i, y=320)
    cnv_3_ships[i].create_image(45, 16, image=ship_3_img_tk)
    cnv_3_ships[i].bind("<Button-1>", clic_cnv_ship)
    cnv_3_ships[i].contains_ship = True
    cnv_3_ships[i].ship_size = 3

cnv_2_ships = []
for i in range(3):
    cnv_2_ships.append(Tk.Canvas(root, width=60, height=30, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_2_ships[i].place(in_=lbl_preparation_page, x=380 + 80*i, y=370)
    cnv_2_ships[i].create_image(30, 16, image=ship_2_img_tk)
    cnv_2_ships[i].bind("<Button-1>", clic_cnv_ship)
    cnv_2_ships[i].contains_ship = True
    cnv_2_ships[i].ship_size = 2

cnv_1_ships = []
for i in range(4):
    cnv_1_ships.append(Tk.Canvas(root, width=30, height=30, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_1_ships[i].place(in_=lbl_preparation_page, x=380 + 50*i, y=420)
    cnv_1_ships[i].create_image(16, 16, image=ship_1_img_tk)
    cnv_1_ships[i].bind("<Button-1>", clic_cnv_ship)
    cnv_1_ships[i].contains_ship = True
    cnv_1_ships[i].ship_size = 1

def clic_auto_set(event):                  ######
    prepare_field.add(Ship(0, 0, 4))
    prepare_field.add(Ship(1, 2, 3))
    prepare_field.add(Ship(5, 3, 3))
    prepare_field.add(Ship(1, 5, 2))
    prepare_field.add(Ship(5, 6, 2))
    prepare_field.add(Ship(2, 7, 2))
    prepare_field.add(Ship(0, 9, 1))
    prepare_field.add(Ship(3, 9, 1))
    prepare_field.add(Ship(6, 9, 1))
    prepare_field.add(Ship(9, 9, 1))
    draw_field(cnv_prepare_field, prepare_field)

lbl_ready = Tk.Label(root, image=ready_img, bg='#3F49CC')
lbl_ready.place(in_=lbl_preparation_page, x=380, y=470)
lbl_ready.bind("<Button-1>", clic_lbl_ready)
lbl_ready.bind("<Button-3>", clic_auto_set)    #######
lbl_ready.ready = False

#-----------

#==[ Battle page ]==========
lbl_battle_page = Tk.Label(root, image=preparation_page_bgr)

ltl_ship_1_img = ship_1_img.resize((24,24))
ltl_ship_2_img = ship_2_img.resize((48,24))
ltl_ship_3_img = ship_3_img.resize((72,24))
ltl_ship_4_img = ship_4_img.resize((96,24))

ltl_ship_1_img_tk = ImageTk.PhotoImage(ltl_ship_1_img)
ltl_ship_2_img_tk = ImageTk.PhotoImage(ltl_ship_2_img)
ltl_ship_3_img_tk = ImageTk.PhotoImage(ltl_ship_3_img)
ltl_ship_4_img_tk = ImageTk.PhotoImage(ltl_ship_4_img)

shoot_img = ImageTk.PhotoImage(file="shoot_img.png")
shooted_cell_img = ImageTk.PhotoImage(Image.open("shooted_cell.png").resize((24,24)))
shooted_ship_img = ImageTk.PhotoImage(Image.open("shooted_ship.png").resize((24,24)))

name_box_img = ImageTk.PhotoImage(Image.new("RGB", (180, 25), '#7092BE'))

cnv_my_field = Tk.Canvas(root, width=250, height=250, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_friend_field = Tk.Canvas(root, width=250, height=250, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_enemy_1_field = Tk.Canvas(root, width=250, height=250, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_enemy_2_field = Tk.Canvas(root, width=250, height=250, bg='#96D9EA', highlightbackground='#3F49CC')

cnv_enemy_1_field.place(in_=lbl_battle_page, x=80, y=40)
cnv_enemy_2_field.place(in_=lbl_battle_page, x=460, y=40)
cnv_my_field.place(in_=lbl_battle_page, x=20, y=350)
cnv_friend_field.place(in_=lbl_battle_page, x=520, y=350)

cnv_enemy_1_field.num = 1
cnv_enemy_2_field.num = 2

cnv_enemy_1_field.bind("<Button-1>", clic_enemy_field)
cnv_enemy_2_field.bind("<Button-1>", clic_enemy_field)

cnv_my_field.player_id = "0"
cnv_friend_field.player_id = "0"
cnv_enemy_1_field.player_id = "0"
cnv_enemy_2_field.player_id = "0"

lbl_names = []
for i in range(4):
    lbl_names.append(Tk.Label(root, image=name_box_img, bg='#3A43CC', fg='#3F49CC', text="aaaaa", font="Helvetica 15", compound='center'))
    lbl_names[i].player_id = "0"

lbl_names[0].place(in_=lbl_battle_page, x=80, y=10)
lbl_names[1].place(in_=lbl_battle_page, x=460, y=10)
lbl_names[2].place(in_=lbl_battle_page, x=20, y=320)
lbl_names[3].place(in_=lbl_battle_page, x=520, y=320)

cnv_ltl_4_ship = Tk.Canvas(root, width=100, height=25, bg='#96D9EA', highlightbackground='#3F49CC')
cnv_ltl_4_ship.place(in_=lbl_battle_page, x=280, y=380)
cnv_ltl_4_ship.create_image(51, 14, image=ltl_ship_4_img_tk)
cnv_ltl_4_ship.alive = True

cnv_ltl_3_ships = []
for i in range(2):
    cnv_ltl_3_ships.append(Tk.Canvas(root, width=75, height=25, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_ltl_3_ships[i].place(in_=lbl_battle_page, x=280 + 85*i, y=420)
    cnv_ltl_3_ships[i].create_image(38, 14, image=ltl_ship_3_img_tk)
    cnv_ltl_3_ships[i].alive = True

cnv_ltl_2_ships = []
for i in range(3):
    cnv_ltl_2_ships.append(Tk.Canvas(root, width=50, height=25, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_ltl_2_ships[i].place(in_=lbl_battle_page, x=280 + 60*i, y=460)
    cnv_ltl_2_ships[i].create_image(25, 14, image=ltl_ship_2_img_tk)
    cnv_ltl_2_ships[i].alive = True

cnv_ltl_1_ships = []
for i in range(4):
    cnv_ltl_1_ships.append(Tk.Canvas(root, width=25, height=25, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_ltl_1_ships[i].place(in_=lbl_battle_page, x=280 + 35*i, y=500)
    cnv_ltl_1_ships[i].create_image(14, 14, image=ltl_ship_1_img_tk)
    cnv_ltl_1_ships[i].alive = True

cnv_shoots = []
for i in range(4):
    cnv_shoots.append(Tk.Canvas(root, width=25, height=25, bg='#96D9EA', highlightbackground='#3F49CC'))
    cnv_shoots[i].create_image(14, 14, image=shoot_img)
    cnv_shoots[i].status = 1   # 1 - available,  0 - used,  -1 - unavailable

cnv_shoots[0].place(in_=lbl_battle_page, x=390, y=380)
cnv_shoots[1].place(in_=lbl_battle_page, x=450, y=420)
cnv_shoots[2].place(in_=lbl_battle_page, x=460, y=460)
cnv_shoots[3].place(in_=lbl_battle_page, x=420, y=500)

#-----------

#==[ Messege label ]==========
msg_box_img = ImageTk.PhotoImage(Image.new("RGB", (360, 120), '#7092BE'))

lbl_messege = Tk.Label(root, image=msg_box_img, bg='#3A43CC', fg='#3F49CC', font="Helvetica 15", compound='center')
#-----------

#==[ Flask ]======
app = Flask(__name__)

@app.route('/newplayer', methods=['POST'])
def new_player():
    j_req = request.json
    for i in range(4):
        if list_players[i].player_id == "0":
            list_players[i]['text'] = j_req["player_name"]
            list_players[i].player_id = j_req["player_id"]
            break;
    return "0"
    
@app.route('/removeplayer', methods=['POST'])
def remove_player():
    j_req = request.json
    for i in range(4):
        if list_players[i].player_id == j_req["player_id"]:
            list_players[i]['text'] = ". . ."
            list_players[i].player_id = "0"
        if list_team_members[i].member_id == j_req["player_id"]:
        	list_team_members[i].member_id = "0"
        	list_team_members[i]['text'] = ". . ."
    return "0"

@app.route('/teamchange', methods=['POST'])
def team_change():
    j_req = request.json

    list_team_members[0].member_id = j_req["team_a"][0]
    if j_req["team_a"][0] != "0": list_team_members[0]['text'] = get_name(j_req["team_a"][0])
    else: list_team_members[0]['text'] = ". . ."

    list_team_members[1].member_id = j_req["team_a"][1]
    if j_req["team_a"][1] != "0": list_team_members[1]['text'] = get_name(j_req["team_a"][1])
    else: list_team_members[1]['text'] = ". . ."

    list_team_members[2].member_id = j_req["team_b"][0]
    if j_req["team_b"][0] != "0": list_team_members[2]['text'] = get_name(j_req["team_b"][0])
    else: list_team_members[2]['text'] = ". . ."

    list_team_members[3].member_id = j_req["team_b"][1]
    if j_req["team_b"][1] != "0":list_team_members[3]['text'] = get_name(j_req["team_b"][1])
    else: list_team_members[3]['text'] = ". . ."
    return "0"

@app.route('/readychange', methods=['POST'])
def ready_change():
    j_req = request.json
    if j_req["ready"]: col = "green"
    else: col = "#3A43CC"
    for lbl in list_players:
        if lbl.player_id == j_req["player_id"]:
            lbl['bg'] = col
            break
    return "0"

@app.route('/start', methods=['POST'])
def start_game():
    global my_field, friend_field, enemy_1_field, enemy_2_field, turn
    j_req = request.json

    lbl_names[2]['text'] = player_name
    if red_team:
        lbl_names[2]['bg'] = '#940C1D'
        lbl_names[2]['fg'] = '#940C1D'
        lbl_names[3]['bg'] = '#940C1D'
        lbl_names[3]['fg'] = '#940C1D'
        cnv_my_field['highlightbackground'] = '#940C1D'
        cnv_friend_field['highlightbackground'] = '#940C1D'
    else:
        lbl_names[0]['bg'] = '#940C1D'
        lbl_names[0]['fg'] = '#940C1D'
        lbl_names[1]['bg'] = '#940C1D'
        lbl_names[1]['fg'] = '#940C1D'
        cnv_enemy_1_field['highlightbackground'] = '#940C1D'
        cnv_enemy_2_field['highlightbackground'] = '#940C1D'

    my_field = Field()
    friend_field = Field()
    enemy_1_field = Field()
    enemy_2_field = Field()

    for ship in prepare_field.ships.items():
        new_ship = Ship(ship[1].x, ship[1].y, ship[1].size, ship[1].orient, ltl=True)
        my_field.add(new_ship)

    for ship in j_req["friend"]["ships"]:
        new_ship = Ship(ship['x'], ship['y'], ship['size'], ship['orient'], ltl=True)
        friend_field.add(new_ship)

    cnv_my_field.player_id = player_ID
    cnv_friend_field.player_id = j_req["friend"]["id"]
    cnv_enemy_1_field.player_id = j_req["enemy_1"]
    cnv_enemy_2_field.player_id = j_req["enemy_2"]

    lbl_names[0]['text'] = get_name(j_req["enemy_1"])
    lbl_names[1]['text'] = get_name(j_req["enemy_2"])
    lbl_names[2]['text'] = player_name
    lbl_names[3]['text'] = get_name(j_req["friend"]["id"])

    lbl_names[0].player_id = j_req["enemy_1"]
    lbl_names[1].player_id = j_req["enemy_2"]
    lbl_names[2].player_id = player_ID
    lbl_names[3].player_id = j_req["friend"]["id"]

    turn = j_req["turn"]
    for lbl in lbl_names:
        if lbl.player_id == turn:
            lbl['bg'] = "green"
            break

    draw_field(cnv_my_field, my_field, ltl=True)
    draw_field(cnv_friend_field, friend_field, ltl=True)
    draw_field(cnv_enemy_1_field, enemy_1_field, ltl=True)
    draw_field(cnv_enemy_2_field, enemy_2_field, ltl=True)

    lbl_preparation_page.pack_forget()
    lbl_battle_page.pack()
    return "0"

@app.route('/friendshoot', methods=['POST'])
def friend_shoot():
    global turn, enemy_1_field, enemy_2_field
    j_req = request.json

    if cnv_enemy_1_field.player_id == j_req["enemy_id"]:
        upd_enemy_field(enemy_1_field, j_req["field"])
        draw_field(cnv_enemy_1_field, enemy_1_field, ltl=True)
    elif cnv_enemy_2_field.player_id == j_req["enemy_id"]:
        upd_enemy_field(enemy_2_field, j_req["field"])
        draw_field(cnv_enemy_2_field, enemy_2_field, ltl=True)
    else: return "1"

    if j_req["turn"] != turn:
        for lbl in lbl_names:
            if lbl.player_id == turn:
                lbl['bg'] = lbl['fg']
                break

    turn = j_req["turn"]
    for lbl in lbl_names:
        if lbl.player_id == turn:
            lbl['bg'] = "green"
            break

    return "0"

@app.route('/enemyshoot', methods=['POST'])
def enemy_shoot():
    global turn
    j_req = request.json
    x = j_req['x']
    y = j_req['y']

    if j_req["target_id"] == player_ID:
        my_field.shoot(x, y)
        draw_field(cnv_my_field, my_field, ltl=True)

        if my_field.battle_field[x][y].contains_ship:
            ship_key = my_field.battle_field[x][y].ship_key
            ship = my_field.ships[ship_key]

            dead = True
            for part in ship.parts:
                if part: dead = False

            if dead:
                if (ship.size == 4):
                    cnv_ltl_4_ship.alive = False
                    cnv_shoots[0].status = -1
                    cnv_ltl_4_ship.create_line(0, 0, 100, 25, fill="red", width=2)
                    cnv_ltl_4_ship.create_line(0, 25, 100, 0, fill="red", width=2)
                    cnv_shoots[0].create_line(0, 0, 25, 25, fill="red", width=2)
                    cnv_shoots[0].create_line(0, 25, 25, 0, fill="red", width=2)

                elif (ship.size == 3):
                    for cnv in cnv_ltl_3_ships:
                        if not(cnv.alive): continue
                        cnv.alive = False
                        cnv.create_line(0, 0, 75, 25, fill="red", width=2)
                        cnv.create_line(0, 25, 75, 0, fill="red", width=2)
                        break
                    if not(cnv_ltl_3_ships[0].alive or cnv_ltl_3_ships[1].alive):
                        cnv_shoots[1].status = -1
                        cnv_shoots[1].create_line(0, 0, 25, 25, fill="red", width=2)
                        cnv_shoots[1].create_line(0, 25, 25, 0, fill="red", width=2)

                elif (ship.size == 2):
                    for cnv in cnv_ltl_2_ships:
                        if not(cnv.alive): continue
                        cnv.alive = False
                        cnv.create_line(0, 0, 50, 25, fill="red", width=2)
                        cnv.create_line(0, 25, 50, 0, fill="red", width=2)
                        break
                    if not(cnv_ltl_2_ships[0].alive or cnv_ltl_2_ships[1].alive or cnv_ltl_2_ships[2].alive):
                        cnv_shoots[2].status = -1
                        cnv_shoots[2].create_line(0, 0, 25, 25, fill="red", width=2)
                        cnv_shoots[2].create_line(0, 25, 25, 0, fill="red", width=2)

                elif (ship.size == 1):
                    for cnv in cnv_ltl_1_ships:
                        if not(cnv.alive): continue
                        cnv.alive = False
                        cnv.create_line(0, 0, 25, 25, fill="red", width=2)
                        cnv.create_line(0, 25, 25, 0, fill="red", width=2)
                        break
                    if not(cnv_ltl_1_ships[0].alive or cnv_ltl_1_ships[1].alive or cnv_ltl_1_ships[2].alive or cnv_ltl_1_ships[3].alive):
                        cnv_shoots[3].status = -1
                        cnv_shoots[3].create_line(0, 0, 25, 25, fill="red", width=2)
                        cnv_shoots[3].create_line(0, 25, 25, 0, fill="red", width=2)
    else:
        friend_field.shoot(x, y)
        draw_field(cnv_friend_field, friend_field, ltl=True)

    if j_req["turn"] != turn:
        for lbl in lbl_names:
            if lbl.player_id == turn:
                lbl['bg'] = lbl['fg']
                break

    turn = j_req["turn"]
    for lbl in lbl_names:
        if lbl.player_id == turn:
            lbl['bg'] = "green"
            break

    return "0"

@app.route('/endgame', methods=['POST'])
def end_game():
    j_req = request.json
    if 'error' in j_req:
        print(j_req['error'])
        lbl_battle_page.pack_forget()
        lbl_choose_game_page.pack()
        upd_active_games()
        return

    def command(event):
        lbl_messege.place_forget()
        lbl_battle_page.pack_forget()
        lbl_choose_game_page.pack()
        upd_active_games()

    if turn == player_ID or turn == lbl_names[3].player_id:
        if red_team: show_messege("Winner: Team B", command)
        else: show_messege("Winner: Team A", command)
    else:
        if red_team: show_messege("Winner: Team A", command)
        else: show_messege("Winner: Team B", command)
    return "0"

#-------------

#==[ Threading ]======
class FlaskThread(threading.Thread):
   def __init__(self, name, counter):
      threading.Thread.__init__(self)
      self.name = name
      self.counter  = counter

   def run(self):
      if __name__ == "__main__":
         app.run(host='127.0.0.1', port=0)

app_run = FlaskThread("flask_run", 1)
#-------------

app_run.start()

try:
    data_file = open("game_data.json", 'r')
    data_json = json.load(data_file)
    data_file.close()
    player_ID = data_json["ID"]
    player_name = data_json["name"]
    svr_URL = data_json["server"]
    webhook_URL = data_json["webhook"]
    ent_nickname.insert(0, player_name)
    ent_svr_URL.insert(0, svr_URL)
    ent_webhook_URL.insert(0, webhook_URL)
    print("ID:", player_ID)
except FileNotFoundError:
    need_req = True

root.mainloop()

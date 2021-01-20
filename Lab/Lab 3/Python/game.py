from tkinter import *
from PIL import Image, ImageTk
from random import randint

root = Tk()
root.title("Линии 2")
root.resizable(False, False)
root.geometry("800x630")

game_stat = 0
chosen_ball = [-1, -1]

def initaliz_game():
    global game_stat, chosen_ball
    game_stat = 0
    chosen_ball = [-1, -1]
    tit['text'] = "Линии 2"
    count['text'] = "Счёт:"
    number['text'] = 0
    btn1['text'] = "Сделать ход"
    hint['text'] = "Подсказка:"
    hint_upd()
    over["text"] = ""
    for i in range(len(lbls_list)):
        lbls_list[i]["image"] = cell_pas
        lbls_list[i].ball = -1

def find_way(start, finish):
    global found, fin
    for i in range(len(lbls_list)):
        lbls_list[i].looked = False
    found = False
    fin = finish
    r = start[0]
    c = start[1]
    check(r+1, c)
    check(r, c+1)
    check(r-1, c)
    check(r, c-1)
    return found

def check(r, c):
    global found, fin
    if ((r >= 0) and (r < 9) and (c >= 0) and (c < 9)):
        if (lbls_matrix[r][c].ball == -1):
            if (not(lbls_matrix[r][c].looked)):
                lbls_matrix[r][c].looked = True
                if ((r == fin[0]) and (c == fin[1])): found = True
                else:
                    check(r+1, c)
                    check(r, c+1)
                    check(r-1, c)
                    check(r, c-1)

def clic(event):
    global game_stat, chosen_ball
    if (game_stat == 1):
        if (chosen_ball == [-1, -1]):        # Если нет выбранного шара
            if (event.widget.ball != -1):          # Если в клетке есть шар
                # то устанавливаем выбраный шар
                chosen_ball[0] = event.widget.row
                chosen_ball[1] = event.widget.col
                event.widget.config(image=balls_on_act_cells[event.widget.ball])  # Смена картинки на актив. ячейку
        else:      # Если есть выбранный шар
            r = chosen_ball[0]
            c = chosen_ball[1]
            if (event.widget.ball != -1):    # Если в клетке есть шар
                # то меняем выбранный шар
                lbls_matrix[r][c]["image"] = balls_on_cells[lbls_matrix[r][c].ball]  # Смена плитки старого шара на пассивную
                chosen_ball[0] = event.widget.row
                chosen_ball[1] = event.widget.col
                event.widget.config(image=balls_on_act_cells[event.widget.ball])  # Смена плитки нового шара на актив. ячейку
            else:   # если в клетке нет шара
                # то делаем ход
                if (find_way((r, c), (event.widget.row, event.widget.col))):   # Если есть путь к ячейке
                    i = lbls_matrix[r][c].ball
                    event.widget.config(image=balls_on_cells[i])
                    lbls_matrix[event.widget.row][event.widget.col].ball = i
                    chosen_ball = [-1, -1]
                    lbls_matrix[r][c]["image"] = cell_pas
                    lbls_matrix[r][c].ball = -1
                    if (not(check_lines(event.widget.row, event.widget.col))): turn()


def hint_upd():
    for n in range(3):
        i = randint(0, 6)
        hints[n]["image"] = ltl_balls[i]
        hints[n].ball = i

def check_lines(r, c):
    ball = lbls_matrix[r][c].ball
    found = False
    dir_len = 0   # длинна линии от точки проверки в заданном направлении
    abs_len = 0   # полная длинна линии
    direct = "w"  # w (-), nw (\), n (|), ne (/)  - Возможные направления линии от точки проверки
    row, col = r, c
    # для направления west (-)
    while (lbls_matrix[row][col].ball == ball):
        dir_len += 1
        if (col > 0): col -= 1
        else: break
    abs_len = dir_len
    # в обратном направлении
    row, col = r, c
    while (lbls_matrix[row][col].ball == ball):
        abs_len += 1
        if (col < 8): col += 1
        else: break
    abs_len -= 1  # основную ячейку посчитали 2 раза
    if (abs_len >= 5): found = True
    else:
        # для направления north-west (\)
        dir_len = 0
        abs_len = 0
        direct = "nw"
        row, col = r, c
        while (lbls_matrix[row][col].ball == ball):
            dir_len += 1
            if ((col > 0) and (row > 0)):
                col -= 1
                row -= 1
            else: break
        abs_len = dir_len
        # в обратном направлении
        row, col = r, c
        while (lbls_matrix[row][col].ball == ball):
            abs_len += 1
            if ((col < 8) and (row < 8)):
                col +=1
                row +=1
            else: break
        abs_len -= 1  # основную ячейку посчитали 2 раза
        if (abs_len >= 5): found = True
        else:
            # для направления north (|)
            dir_len = 0
            abs_len = 0
            direct = "n"
            row, col = r, c
            while (lbls_matrix[row][col].ball == ball):
                dir_len += 1
                if (row > 0): row -= 1
                else: break
            abs_len = dir_len
            # в обратном направлении
            row, col = r, c
            while (lbls_matrix[row][col].ball == ball):
                abs_len += 1
                if (row < 8): row +=1
                else: break
            abs_len -= 1  # основную ячейку посчитали 2 раза
            if (abs_len >= 5): found = True
            else:
                # для направления north-east (/)
                dir_len = 0
                abs_len = 0
                direct = "ne"
                row, col = r, c
                while (lbls_matrix[row][col].ball == ball):
                    dir_len += 1
                    if ((col < 8) and (row > 0)):
                        col += 1
                        row -= 1
                    else: break
                abs_len = dir_len
                # в обратном направлении
                row, col = r, c
                while (lbls_matrix[row][col].ball == ball):
                    abs_len += 1
                    if ((col > 0) and (row < 8)):
                        col -=1
                        row +=1
                    else: break
                abs_len -= 1  # основную ячейку посчитали 2 раза
                if (abs_len >= 5): found = True
    if (found):
        remove_balls((r, c), direct, dir_len, abs_len)
    return found

def remove_balls(start, direct, dir_len, abs_len):
    row = start[0]
    col = start[1]
    if (direct == "w"):
        col -= dir_len - 1
        for i in range(5):
            lbls_matrix[row][col]["image"] = cell_pas
            lbls_matrix[row][col].ball = -1
            col += 1
            number["text"] += 2
    elif (direct == "nw"):
        row -= dir_len - 1
        col -= dir_len - 1
        for i in range(5):
            lbls_matrix[row][col]["image"] = cell_pas
            lbls_matrix[row][col].ball = -1
            row += 1
            col += 1
            number["text"] += 2
    elif (direct == "n"):
        row -= dir_len - 1
        for i in range(5):
            lbls_matrix[row][col]["image"] = cell_pas
            lbls_matrix[row][col].ball = -1
            row += 1
            number["text"] += 2
    elif (direct == "ne"):
        row -= dir_len - 1
        col += dir_len - 1
        for i in range(5):
            lbls_matrix[row][col]["image"] = cell_pas
            lbls_matrix[row][col].ball = -1
            row += 1
            col -= 1
            number["text"] += 2

def turn():
    global game_stat
    avbl_cells = []
    for i in range(len(lbls_list)):
        if (lbls_list[i].ball == -1): avbl_cells.append(lbls_list[i])
    if (len(avbl_cells) < 3):
        game_stat = 0
        over["text"] = "Игра окончена."
    else:
        for n in range(3):
            i = hints[n].ball
            j = randint(0, len(avbl_cells)-1)
            avbl_cells[j]["image"] = balls_on_cells[i]
            avbl_cells[j].ball = i
            check_lines(avbl_cells[j].row, avbl_cells[j].col)
            avbl_cells.pop(j)
        fail = True
        for i in range(len(lbls_list)):
            if (lbls_list[i].ball == -1):
                fail = False
                break
        if (fail):
            game_stat = 0
            over["text"] = "Игра окончена."
        else: hint_upd()



def btn1_clic(event):
    if (game_stat == 1): turn()

def btn2_clic(event):
    global game_stat
    initaliz_game()
    game_stat = 1
    turn()

def add_to_lists(name):
    bgr0 = Image.open("cell-0.png").convert('RGBA')
    bgr1 = Image.open("cell-1.png").convert('RGBA')
    tileset = Image.open(name).convert('RGBA')
    ball = tileset.crop((0, 0, 55, 55))
    ltl_ball = tileset.crop((7, 129, 47, 167))
    bgr0.paste(ball, (6,6), ball)
    bgr1.paste(ball, (6,6), ball)
    balls.append(ImageTk.PhotoImage(ball))
    ltl_balls.append(ImageTk.PhotoImage(ltl_ball))
    balls_on_cells.append(ImageTk.PhotoImage(bgr0))
    balls_on_act_cells.append(ImageTk.PhotoImage(bgr1))

def stat(event):
    print("row: ", event.widget.row)
    print("col: ", event.widget.col)
    print("ball: ", event.widget.ball)
    print("looked: ", event.widget.looked)

page_bgr = PhotoImage(file="page-bgr.png")
cell_pas = ImageTk.PhotoImage(file="cell-0.png")
cell_act = ImageTk.PhotoImage(file="cell-1.png")

balls = []              # 0 - aqua; 1 - blue; 2 - green; 3 - pink; 4 - red; 5 - violet; 6 - yellow
ltl_balls = []
balls_on_cells = []
balls_on_act_cells = []
add_to_lists("ball-aqua.png")
add_to_lists("ball-blue.png")
add_to_lists("ball-green.png")
add_to_lists("ball-pink.png")
add_to_lists("ball-red.png")
add_to_lists("ball-violet.png")
add_to_lists("ball-yellow.png")

bgimg = page_bgr.zoom(7,7)
root_lbl = Label(root, image=bgimg)
root_lbl.pack()

lbls_list = []
lbls_matrix = []
for row in range(9):
    lbls_matrix.append([])
    for col in range(9):
        lbl = Label(root_lbl, bg="#454545", image=cell_pas)
        # добавление своих полей
        lbl.row = row
        lbl.col = col
        lbl.ball = -1
        lbl.looked = False
        # -----
        lbl.bind("<Button-1>", clic)
        lbl.bind("<Button-3>", stat)
        lbl.place(y=row*69, x=col*69)
        lbls_list.append(lbl)
        lbls_matrix[row].append(lbl)

# Создание элементов для панели справа
tit = Label(root_lbl, text="Линии 2", bg="#454545", fg="#ffffff", font="Times 30")
tit.place(x=640, y=10)

count = Label(root_lbl, text="Счёт:", bg="#454545", fg="#ffffff", font="Times 22")
count.place(x=640, y=80)

number = Label(root_lbl, text=0, bg="#454545", fg="#ffffff", font="Times 22")
number.place(x=710, y=80)

btn1 = Button(root_lbl, text="Сделать ход", font="Times 12", bg="#555", fg="#ccc", padx=10, pady=5)
btn1.bind("<Button-1>", btn1_clic)
btn1.place(x=640, y=150)

hint = Label(root_lbl, text="Подсказка:", bg="#454545", fg="#ccc", font="Times 16")
hint.place(x=640, y=210)

hints = []
hb1 = Label(root_lbl, bg="#454545", image=None)
hb1.ball = -1
hb1.place(x=640, y=240)
hints.append(hb1)
hb2 = Label(root_lbl, bg="#454545", image=None)
hb2.ball = -1
hb2.place(x=690, y=240)
hints.append(hb2)
hb3 = Label(root_lbl, bg="#454545", image=None)
hb3.ball = -1
hb3.place(x=740, y=240)
hints.append(hb3)

btn2 = Button(root_lbl, text="Новая игра", bg="#555", fg="#ccc", font="Times 15", padx=20, pady=15)
btn2.bind("<Button-1>", btn2_clic)
btn2.place(x=640, y=330)

over = Label(root_lbl, text="Игра окончена.", bg="#454545", fg="#ffffff", font="Times 15")
over.place(x=640, y=400)
# --------

initaliz_game()

root.mainloop()

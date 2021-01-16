from tkinter import *
from PIL import Image, ImageTk
from random import randint

root = Tk()
root.title("Линии 2")
root.resizable(False, False)
root.geometry("800x630")

def clic(event):
    if (event.widget.ball == -1):
        i = randint(0, 6)
        event.widget.config(image=balls_on_cells[i])
        event.widget.ball = i
    else:
        print(event.widget.ball)
        event.widget.config(image=balls_on_act_cells[event.widget.ball])

def btn1_clic(event):
    print(1)

def btn2_clic(event):
    print(2)

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

for row in range(9):
    for col in range(9):
        lbl = Label(root_lbl, bg="#454545", image=cell_pas)
        # добавление своих полей
        lbl.row = row
        lbl.col = col
        lbl.ball = -1
        # -----
        lbl.bind("<Button-1>", clic)
        lbl.place(y=row*69, x=col*69)

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

hb1 = Label(root_lbl, bg="#454545", image=ltl_balls[0])
hb1.place(x=640, y=240)
hb2 = Label(root_lbl, bg="#454545", image=ltl_balls[3])
hb2.place(x=690, y=240)
hb3 = Label(root_lbl, bg="#454545", image=ltl_balls[5])
hb3.place(x=740, y=240)

btn2 = Button(root_lbl, text="Новая игра", bg="#555", fg="#ccc", font="Times 15", padx=20, pady=15)
btn2.bind("<Button-1>", btn2_clic)
btn2.place(x=640, y=330)

over = Label(root_lbl, bg="#454545", fg="#ffffff", text="Игра окончена.", font="Times 15")
over.place(x=640, y=400)
# --------

root.mainloop()

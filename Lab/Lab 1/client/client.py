from tkinter import *
import requests
import json

# формирование макета виджета -------
root = Tk()
root.title("Погода")
simf = Label(root, text="   Симферополь   ", font="Times 20", bg="#ff8c00")
simf.pack(fill=X)
status = Label(root, font="Times 16", fg="#353535", bg="#ff8c00")
status.pack(fill=X)
temp = Label(root, font="Times 50")
temp.pack()
bott = Label(root, bg="#ff8c00")
bott.pack(side=BOTTOM, fill=X)
#------------

# глобальные переменные --------
pal = ["#ff2400","#ff8c00","#42aaff","#005de0"]
col = "#ff8c00"
#--------

# функция вызывается при нажатии ПКМ---------
def upd(event):
    req()
#------------

# функция делает и обрабатывает запрос -------
def req():
    global t, stat, col, col_t, pal
    res = requests.get('http://localhost:3000/raw')
    if (res.status_code == 200):
        if (res.text != ""):
            col_t = "black"
            j = json.loads(res.text)
            stat = j['weather_des'].encode('l1').decode()
            t = round(j['temp'])
            if (t > 25):
                col = pal[0]
            elif (t > 15):
                col = pal[1]
            elif (t > 0):
                col = pal[2]
            else:
                col = pal[3]
        else:
            stat = "Ответ сервера пуст"
            t = "(x_x)"
            col_t = "#5c5c5c"
    else:
        stat = "Ошибка получения данных"
        t = "(x_x)"
        col_t = "#5c5c5c"
    put_to_wid()
#------------

# функция заполняет виджет ------
def put_to_wid():
    global t, stat, col, col_er, simf, status, temp, bott
    simf['bg'] = col
    status['bg'] = col
    status['text'] = stat
    if (t != "(x_x)"):
        t = str(t) + " °C"
    temp['text'] = t
    temp['fg'] = col_t
    bott['bg'] = col
#-------------

simf.bind("<Button-3>", upd)
status.bind("<Button-3>", upd)
temp.bind("<Button-3>", upd)
bott.bind("<Button-3>", upd)

req()

root.mainloop()
from tkinter import *
import requests
import json

# формирование макета виджета -------
root = Tk()
root.title("Погода")
simf = Label(root, text="   Симферополь   ", font="Times 20", bg="#ff8c00")
simf.pack(fill=X)
status = Label(root, font="Times 16", fg="#5c5c5c", bg="#ff8c00")
status.pack(fill=X)
temp = Label(root, font="Times 50")
temp.pack()
bott = Label(root, bg="#ff8c00")
bott.pack(side=BOTTOM, fill=X)
#------------

# функция вызывается при нажатии ПКМ---------
def upd(event):
    req()
#------------

# функция делает и обрабатывает запрос -------
def req():
    global t, stat
    res = requests.get('http://localhost:3000/raw')
    if (res.status_code == 200):
        if (res.text != ""):
            j = json.loads(res.text)
            stat = j['weather_des'].encode('l1').decode()
            t = round(j['temp'])
        else:
            stat = "Ответ сервера пуст"
            t = "(x_x)"
    else:
        stat = "Ошибка получения данных"
        t = "(x_x)"
    put_to_wid()
#------------

# функция заполняет виджет ------
def put_to_wid():
    global t, stat, status, temp
    status['text'] = stat
    if (t != "(x_x)"):
        t = str(t) + " °C"
    temp['text'] = t
#-------------

simf.bind("<Button-3>", upd)
status.bind("<Button-3>", upd)
temp.bind("<Button-3>", upd)
bott.bind("<Button-3>", upd)

req()

root.mainloop()
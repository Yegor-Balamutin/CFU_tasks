print("Калькулятор")
print("Введите выражение или нажмите Enter чтобы выйти")
string = input()
while (string != ''):
    f1 = True
    while f1 :   # цикл проверки правильности ввода
        i = 0
        f2 = True
        f3 = True
        a = ""
        b = ""
        while (string[i] != " "):
            if not(((string[i] >= '0') and (string[i] <= '9')) or (string[i] == '.')):   # если в первом числе встретился посторонний символ
                f2 = False
                break
            a = a + string[i]   # сразу запоминаем первое введённое число
            if ((i + 1) < (len(string))):   # проверка чтобы индекс не вышел за пределы строки
                i = i + 1
            else:
                f2 = False
                break
        if (f2):
            if ((i + 1) < (len(string))):   # проверка чтобы индекс не вышел за пределы строки
                i = i + 1   # переход на символ действия
                if ((string[i] == '+') or (string[i] == '-') or (string[i] == '*') or (string[i] == '/')):
                    d = string[i]   # сразу запоминаем введённое действие
                    if ((i + 1) < (len(string))):   # проверка чтобы индекс не вышел за пределы строки
                        i = i + 1   # переход на пробел после символа действия
                        if (string[i] == ' ') and (len(string) > (i + 1)):
                            while (i < (len(string) - 1)):
                                i = i + 1
                                b = b + string[i]
                                if not(((string[i] >= '0') and (string[i] <= '9')) or (string[i] == '.')):   # если во втором числе встретился посторонний символ
                                    f3 = False
                                    break
                            if (f3):
                                f1 = False
        if (f1):
            print("Неверный ввод! Попробуйте ещё раз")
            string = input()
    a = float(a)
    b = float(b)
    if (d == '+'):
        rez = a + b
        print(rez)
    elif (d == '-'):
        rez = a - b
        print(rez)
    elif (d == '*'):
        rez = a * b
        print(rez)
    else:
        rez = a / b
        print(rez)
    print("Введите следуущее выражение или нажмите Enter чтобы выйти")
    string = input()

print("Расчёт площади треугольника")
print("Выберете способ вычисления площади:")
print("1 - через длины сторон")
print("2 - через координаты вершин")
f = True
while (f):
	k = int(input())
	if (k == 1) or (k == 2):
		f = False
	else:
		print("Неверный ввод! Попробуйте ещё раз:")
if (k == 1):
	a = int(input("a = "))
	b = int(input("b = "))
	c = int(input("c = "))
	p = (a + b + c)/2
	s = (p * (p - a) * (p - b) * (p - c))**0.5
else:
	x1,y1 = input("Введите x1 y1: ").split(' ')
	x2,y2 = input("Введите x2 y2: ").split(' ')
	x3,y3 = input("Введите x3 y3: ").split(' ')
	x1 = int(x1)
	x2 = int(x2)
	x3 = int(x3)
	y1 = int(y1)
	y2 = int(y2)
	y3 = int(y3)
	s = abs((x1 - x3) * (y2 - y3) - (y1 - y3) * (x2 - x3))/2
print("Площадь треугольника равна:" + str(s))
input("Нажмите Enter")

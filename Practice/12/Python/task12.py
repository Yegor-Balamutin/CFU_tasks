a = int(input("Рассчитать факториал числа: "))
rez = 1
for i in range(2,a+1):
	rez = rez * i
print("Результат: " + str(rez))
input("Нажмите Enter")
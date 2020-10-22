a = int(input("Возвести число: "))
b = int(input("в степень: "))
rez = 1
for i in range(b):
	rez = rez * a
print("Результат: " + str(rez))
input("Нажмите Enter")
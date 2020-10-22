a = int(input("Определить, является ли число простым: "))
rez = True
for i in range(2,a):
	if (a % i == 0):
		rez = False
		break
if rez:
	print("число простое")
else:
	print("число составное")
input("Нажмите Enter")
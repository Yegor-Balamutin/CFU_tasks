import random
s = '1'
while (s == '1'):
	a = random.randint(1,100)
	print("Загадано число от 1 до 100. Попробуйте угадать!")
	f = True
	for i in range(1,6):
		x = int(input(str(i) + " попытка: "))
		if (x == a):
			print("Поздравляю, вы угадали!")
			f = False
			break
		elif (x < a):
			print("Загаданное число больше")
		else:
			print("Загаданное число меньше")
	if (f):
		print("Вы проиграли! Было загадано: " + str(a))
	print("Хотите попробовать снова? (1 - Да)")
	s = input()
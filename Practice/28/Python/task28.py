def print_factorization(n: int) -> None:
	# составление списка простых чисел (от 2 до 999)
	prime = []
	for i in range(2, 1000):
		p = True
		for j in range(2, (i//2)+1):
			if (i % j == 0):
				p = False
				break
		if (p):
			prime.append(i)
	# обнуление значений словаря
	fact = {}             # fact - словарь, в котором ключ - простое число, а значение - сколько раз это число присутствует в n
	for i in prime:
		fact[i] = 0
	# заполнение словаря
	n1 = n
	for i in prime:
		while (n % i == 0):
			fact[i] += 1
			n = n / i
	# удаление нулевых элементов из fact
	for i in prime:
		if (fact[i] == 0):
			fact.pop(i)
	# вывод результата
	x = 0
	print(str(n1) + " = ", end="")
	for i in fact.items():
		if (i[1] > 0):
			if (i[1] == 1):
				print(i[0], end="")
			else:
				print(str(i[0]) + "^" + str(i[1]), end="")
		x += 1
		if (len(fact) > x):
			print(" * ", end="")
	print()

n = int(input("n = "))
print_factorization(n)
input("Нажмите Enter")
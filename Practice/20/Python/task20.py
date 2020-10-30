money = int(input("деньги: "))
money0 = money
k = int(input("количество видов напитков в магазине: "))
purchase = {}   # словарь показывающий количество купленных напитков
order = []   # список ключей, показывающий в каком порядке надо будет покупать напитки
drinks = {}   # словарь напитков: 0: "назв", цена, объём
d = {}   # цена за литр для каждого напитка
for i in range(k):
	print(str(i + 1) + ": ", end = "")
	name, price, volume = input().split(" ")
	price = int(price)
	volume = int(volume)
	drinks[i] = (name, price, volume)
	d[i] = price / volume
	purchase[i] = 0
while (len(d) > 0):   # цикл составления списка order
	m = 1000000
	for i in d.keys():
		if (m > d[i]):
			m = d[i]
			j = i
	order.append(j)
	del d[j]
a = 0
f = True
while f:
	if (money >= drinks[order[a]][1]):
		money -= drinks[order[a]][1]
		purchase[order[a]] += 1
	elif (a < k-1):
		a += 1
	else:
		f = False
if (money < money0):
	print("надо купить:")
	for i in range(k):
		if (purchase[i] != 0):
			print(str(drinks[i][0]) + ": " + str(purchase[i]) + " шт.")
	v = 0
	for i in range(k):
		v = v + purchase[i] * drinks[i][2]
	print("суммарный объём: " + str(v))
	print("осталось денег: " + str(money))
else:
	print(-1)
input("Нажмите Enter")

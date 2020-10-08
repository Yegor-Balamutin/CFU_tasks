a = float(input("a = "))
b = float(input("b = "))
c = float(input("c = "))
if (a == 0):
	if (b == 0):
		if(c == 0):
			print("x - любое число")
		else:
			print("нет корней")
	else:
		x = -c/b
		print("x = " + str(x))
else:
	d = b*b -4*a*c
	if (d > 0):
		x1 = (-b -(d**0.5))/(2*a)
		x2 = (-b +(d**0.5))/(2*a)
		print("x1 = " + str(x1))
		print("x2 = " + str(x2))
	elif (d == 0):
		x = (-b)/(2*a)
		print("x = " + str(x))
	else:
		print("нет действительных корней")
input()

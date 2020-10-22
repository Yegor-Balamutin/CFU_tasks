s = int(input("s = "))
l1 = int(input("l1 = "))
r1 = int(input("r1 = "))
l2 = int(input("l2 = "))
r2 = int(input("l2 = "))
x1 = l1
x2 = s - x1
if (x2 >= l2) and (x2 <= r2):
	print(x1,x2)
elif (x2 < l2):
	print("-1")
else:
	x2 = r2
	x1 = s - x2
	if (x1 >= l1) and (x1 <= r1):
		print(x1,x2)
	else:
		print("-1")
input("нажмите Enter")
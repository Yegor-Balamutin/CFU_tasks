print("Встреча")
h1,m1 = input("Время 1: ").split(":")
h2,m2 = input("Время 2: ").split(":")
h1 = int(h1)
h2 = int(h2)
m1 = int(m1)
m2 = int(m2)
t1 = h1 * 60 + m1
t2 = h2 * 60 + m2
if (abs(t1 - t2) <= 15):
	print("Встреча состоится")
else:
	print("Встреча не состоится")
input("Нажмите Enter")

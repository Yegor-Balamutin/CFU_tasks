def bmi(weight, height):
	return (weight / (height ** 2))

def print_bmi(bmi):
	if (bmi < 18.5):
		print("недостаточная масса")
	elif (bmi < 25.0):
		print("норма")
	elif (bmi < 30):
		print("избыточная масса")
	else:
		print("ожирение")

w,h = input().split()
w = float(w)
h = float(h) / 100
print_bmi(bmi(w, h))
input("Нажмите Enter")

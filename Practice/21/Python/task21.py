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

h = (float(input("Рост: "))) / 100
w = float(input("вес: "))
print_bmi(bmi(w, h))
input("Нажмите Enter")

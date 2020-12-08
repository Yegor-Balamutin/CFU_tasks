import random
def BozoSort1 (arr, asc = True):
	ready = False
	while not(ready):
		ready = True
		if asc:
			for i in range(len(arr) - 1):
				if arr[i] > arr[i+1]:
					ready = False
					break
		else:
			for i in range(len(arr) - 1):
				if arr[i] < arr[i+1]:
					ready = False
					break
		if not(ready):
			a = random.randint(0, len(arr)-1)
			b = random.randint(0, len(arr)-1)
			arr[a], arr[b] = arr[b], arr[a]
	return arr

def BozoSort2(mat, asc = True):
	arr = []
	for i in range(len(mat)):
		for j in range(len(mat[i])):
			arr.append(mat[i][j])
	arr = BozoSort1(arr, asc)
	return arr

def BozoSort3(a, b, c, asc = True):
	arr = [a, b, c]
	arr = BozoSort1(arr, asc)
	return arr

n = int(input())
arr = input().split()
for i in range(n):
        arr[i] = int(arr[i])
mat = []
lin = []
a = 0
for i in range(int(n ** 0.5)):
	for j in range(int(n ** 0.5)):
		lin.append(arr[j+a])
	mat.append(list(lin))
	lin.clear()
	a += int(n ** 0.5)
result = BozoSort1(arr)
for i in range(n):
	print(str(result[i]) + " ", end="")
print()
result = BozoSort1(arr, False)
for i in range(n):
	print(str(result[i]) + " ", end="")
print()
result = BozoSort2(mat)
for i in range(n):
	print(str(result[i]) + " ", end="")
print()
result = BozoSort2(mat, False)
for i in range(n):
	print(str(result[i]) + " ", end="")
print()
result = BozoSort3(arr[0], arr[1], arr[2])
for i in range(3):
	print(str(result[i]) + " ", end="")
print()
result = BozoSort3(arr[0], arr[1], arr[2], False)
for i in range(3):
	print(str(result[i]) + " ", end="")
print()
input("Нажмите Enter")

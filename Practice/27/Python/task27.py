n = int(input())
s = input()
s += " "
num = ""
arr = []
for i in range(len(s)):
	if s[i] == " ":
		arr.append(int(num))
		num = ""
	else:
		num += s[i]
min5 = []
for i in range(n):
	if len(min5) < 5:
		min5.append(arr[i])
		min5.sort(reverse=True)
	else:
		if min5[0] > arr[i]:
			min5[0] = arr[i]
	for j in range(len(min5)):
		print(min5[j], end=" ")
	print()
input("Нажмите Enter")
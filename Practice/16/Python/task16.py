n = int(input("Количество билетов: "))
s = []
k = 0
for i in range(1,n+1):
	s1 = input(str(i) + " билет: ")
	if (s1[0] == 'a') and (s1[4] == s1[5] == '5') and (s1[6] == s1[7] == '6') and (s1[8] == '1'):
		s.append(s1)
		k = k + 1
if (k == 0):
	print("Билетов Суслика нет.")
else:
	print("Билетов Суслика:" + str(k))
	for i in range(k):
		print(str(i+1) + ": " + s[i])
input("Нажмите Enter")
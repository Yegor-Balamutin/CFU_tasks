stop = input("Стоп-слово: ")
words = ["hallo","klempner","das","ist","fantastisch","fluggegecheimen"]
p = {}   # словарь вероятностей букв
a = ord('a')
while (a <= ord('z')):
	p[chr(a)] = 0
	a = a + 1
s = ""
for i in range(len(words)):
	s = s + words[i]
for i in range(len(s)):   # посчитали сколько раз встречается каждая буква
	p[s[i]] += 1
a = ord('a')
while (a <= ord('z')):   # разделили количество каждой буквы на количество всех букв и получили вероятность
	p[chr(a)] /= len(s)
	a = a + 1
prob = 1
for i in range(len(stop)):
	prob *= p[stop[i]]
print ("вероятность равна: " + str(prob))
input("нажмите Enter")
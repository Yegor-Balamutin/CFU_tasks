print("Введите x0, v0, t")
x0,v0,t = input().split(" ")
x0 = float(x0)
v0 = float(v0)
t = float(t)
a = 9.8
xt = x0 + v0*t - a*t*t/2
rez = abs(xt - x0)
print("Объект преодалел расстояние: " + str(rez))
input()
import math
import copy
  
class Point:
    def __init__(self, a1=0, a2=0, coord_system="Cartesian"):
        if type(a1) == type(''):
            i = 1
            x = ""
            while a1[i] != ',':
                x += a1[i]
                i += 1
            i += 2
            y = ""
            while a1[i] != ')':
                y += a1[i]
                i += 1
            self.x = float(x)
            self.y = float(y)
            self.r = 0
            self.phi = 0
            self.set_new_r_phi(self.x, self.y)
        elif coord_system == "Cartesian":
            self.x = a1
            self.y = a2
            self.r = 0
            self.phi = 0
            self.set_new_r_phi(a1, a2)
        elif coord_system == "Polar":
            self.x = 0
            self.y = 0
            self.r = a1
            self.phi = self.correct_phi(a2)
            self.set_new_x_y(self.r, self.phi)

    def __eq__(self, other):
            if (abs(self.x - other.x) <= 0.0000000001) and (abs(self.y - other.y) <= 0.0000000001): return True
            else: return False

    def __ne__(self, other):
        if (abs(self.x - other.x) > 0.0000000001) and (abs(self.y - other.y) > 0.0000000001): return True
        else: return False

    def __str__(self):
        return f"({self.x},{self.y})"

    def __repr__(self):
        return f"({self.x},{self.y})"

    def set_new_r_phi(self, x, y):
        self.r = math.sqrt(x * x + y * y)
        if (self.r == 0): self.phi = 0
        elif ((x > 0) and (y >= 0)): self.phi = math.atan(y / x)
        elif ((x > 0) and (y < 0)): self.phi = math.atan(y / x) + 2 * math.pi
        elif (x < 0): self.phi = math.atan(y / x) + math.pi
        elif ((x == 0) and (y > 0)): self.phi = math.pi / 2
        elif ((x == 0) and (y < 0)): self.phi = 3 * math.pi / 2

    def set_new_x_y(self, r, phi):
        self.x = r * math.cos(phi)
        self.y = r * math.sin(phi)

    def correct_phi(self, phi):
        if phi > 0:
            while (phi - 2 * math.pi) >= 0:
                phi -= 2 * math.pi
        else:
            while (phi + 2 * math.pi) <= 0:
                phi += 2 * math.pi
        return phi

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def get_r(self):
        return self.r
    
    def get_phi(self):
        return self.phi

    def set_x(self, x):
        self.x = x
        self.set_new_r_phi(self.x, self.y)

    def set_y(self, y):
        self.y = y
        self.set_new_r_phi(self.x, self.y)

    def set_r(self, r):
        self.r = r
        self.set_new_x_y(self.r, self.phi)

    def set_phi(self, phi):
        self.phi = self.correct_phi(phi)
        self.set_new_x_y(self.r, self.phi)
  
  
with open('data.txt') as fin:
    original = [Point(p) for p in fin.readline().split(', ')]
  
simulacrum = copy.deepcopy(original)
for p in simulacrum:
    print(p, end='')
    p.set_x(p.get_x() + 10)
    p.set_phi(p.get_phi() + 180*math.pi/180)
    p.set_y(-p.get_y())
    p.set_x(-p.get_x() - 10)
    print(p)
  
print('\nIt works!\n' if simulacrum == original else '\nIt not works!\n')
input()

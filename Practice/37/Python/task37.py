import math
  
def equal(a, b, e=1E-10):
    if -e < a - b < e: return True
    else: return False
  
  
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
 
  
class Vector

     length()
        return sqrt(x*x + y*y);
    

    # operators -------------------
    bool operator==(Vector b) {
        if ((fabs(this->x - b.x) <= 0.0000000001) && (fabs(this->y - b.y) <= 0.0000000001)) return true;
        else return false;
    }

    Vector operator-(){
        Vector v;
        v.x = -x;
        v.y = -y;
        return v;
    }

    Vector operator-(Vector b){
        Vector v;
        v.x = x - b.x;
        v.y = y - b.y;
        return v;
    }

    Vector operator+(Vector b){
        Vector v;
        v.x = x + b.x;
        v.y = y + b.y;
        return v;
    }

    Vector operator*(double b){
        Vector v;
        v.x = x * b;
        v.y = y * b;
        return v;
    }

    double operator*(Vector b){
        return x * b.x + y * b.y;
    }

  
  
a = Vector(Point(1, 2))
b = Vector(Point(-2, 0), Point(-1, 2))
if a == b and b == a: print('Equality test passed')
else: print('Equality test failed')
  
na  = Vector(Point(-1, -2))
ox  = Vector(Point( 1,  0))
nox = Vector(Point(-1,  0))
oy  = Vector(Point( 0,  1))
noy = Vector(Point( 0, -1))
if a == -na and na == -a and -ox == nox and -oy == noy: print('Invert test passed')
else: print('Invert test failed')
  
if ox + oy + oy == a and -ox == -a + oy + oy: print('Summation test passed')
else: print('Summation test failed')
  
if -ox + oy == oy - ox and -oy + ox == ox - oy: print('Subtraction test passed')
else: print('Subtraction test failed')
  
if (ox * 3 == ox + ox + ox and
    oy * 3 == oy + oy + oy and
    ox * (-3) == -ox - ox - ox and
    oy * (-3) == -oy - oy - oy): print('Multiplication by number test passed')
else: print('Multiplication by number test failed')
  
if (equal(ox.length(), 1) and
    equal(oy.length(), 1) and
    equal((ox * 3 + oy * 4).length(), 5)): print('Length test passed')
else: print('Length test failed')
  
if (equal(ox*ox, ox.length()**2) and
    equal(oy*oy, oy.length()**2) and
    equal((ox*3 + oy*4)*(ox*3 + oy*4), (ox*3 + oy*4).length()**2)): print('Multiplication by Vector test passed')
else: print('Multiplication by Vector test failed')
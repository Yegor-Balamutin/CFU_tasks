import math
  
class Rational:
    def __init__(self, numerator=0, denumerator=1):
        self.num = numerator
        self.denum = denumerator
    def gcd(a, b):
        while a != b:
            if a > b: a -= b
            else: b -= a
        return a
    def __eq__(self, other):
        if self.num == other.num and self.denum == other.denum: return True
        else return False
  
def equal(a, b, e=1E-10):
    if -e < a - b < e: return True
    else: return False
  
if (Rational.gcd(91, 65) == 13 and
    Rational.gcd(10, 3) == 1 and
    Rational.gcd(-10, 3) == 1 and
    Rational.gcd(10, -3) == 1 and
    Rational.gcd(-10, -3) == 1 and
    Rational.gcd(30, 10) == 10 and
    Rational.gcd(10, 30) == 10 and
    Rational.gcd(0, 10) == 10 and
    Rational.gcd(10, 0) == 10
    ): print('gcd test passed')
else: print('gcd test failed')
  
if (not Rational(22, 0).isNaN() and
    not Rational(22, 9).isNaN() and
    not Rational(0, 9).isNaN() and
    not Rational(-22, 9).isNaN() and
    not Rational(-22, 0).isNaN() and
    Rational(0, 0).isNaN()
    ): print('isNaN test passed')
else: print('isNaN test failed')
  
if (Rational(22, 0) == Rational(22, 0) and
    Rational(22, 0) == Rational(9, 0) and
    not(Rational(22, 0) == Rational(22, 9)) and
    not(Rational(22, 0) == Rational(-22, 9)) and
    not(Rational(22, 0) == Rational(-22, 0)) and
    not(Rational(22, 0) == Rational(0, 9)) and
    not(Rational(22, 0) == Rational(0, 0)) and
  
    Rational(22, 9) == Rational(22, 9) and
    Rational(22, 9) == Rational(-22, -9) and
    Rational(22, 9) == Rational(110, 45) and
    Rational(22, 9) == Rational(-110, -45) and
    not(Rational(22, 9) == Rational(-22, 9)) and
    not(Rational(22, 9) == Rational(22, -9)) and
    not(Rational(22, 9) == Rational(9, 22)) and
    not(Rational(22, 9) == Rational(22, 0)) and
    not(Rational(22, 9) == Rational(-22, 0)) and
    not(Rational(22, 9) == Rational(0, 9)) and
    not(Rational(22, 9) == Rational(0, 0)) and
  
    Rational(0, 1) == Rational(0, 1) and
    Rational(0, 1) == Rational(0, 9)  and
    Rational(0, 1) == Rational(0, -9)  and
    not(Rational(0, 1) == Rational(22, 9))  and
    not(Rational(0, 1) == Rational(-22, 9))  and
    not(Rational(0, 1) == Rational(22, 0)) and
    not(Rational(0, 1) == Rational(-22, 0)) and
    not(Rational(0, 1) == Rational(0, 0)) and
  
    Rational(-22, 9) == Rational(-22, 9) and
    Rational(-22, 9) == Rational(22, -9) and
    Rational(-22, 9) == Rational(-110, 45) and
    Rational(-22, 9) == Rational(110, -45) and
    not(Rational(-22, 9) == Rational(-22, -9)) and
    not(Rational(-22, 9) == Rational(22, 9)) and
    not(Rational(-22, 9) == Rational(9, -22)) and
    not(Rational(-22, 9) == Rational(22, 0)) and
    not(Rational(-22, 9) == Rational(-22, 0)) and
    not(Rational(-22, 9) == Rational(0, 9)) and
    not(Rational(-22, 9) == Rational(0, 0)) and
  
    Rational(-22, 0) == Rational(-22, 0) and
    Rational(-22, 0) == Rational(-9, 0) and
    not(Rational(-22, 0) == Rational(22, 9)) and
    not(Rational(-22, 0) == Rational(-22, 9)) and
    not(Rational(-22, 0) == Rational(22, 0)) and
    not(Rational(-22, 0) == Rational(0, 9)) and
    not(Rational(-22, 0) == Rational(0, 0)) and
  
    not(Rational(0, 0) == Rational(0, 0))
    ): print('Equality test passed')
else: print('Equality test failed')
  
if (Rational(22, 0) + Rational(22, 0) == Rational(22, 0) and
    Rational(22, 9) + Rational(22, 0) == Rational(22, 0) and
    Rational(0, 9) + Rational(22, 0) == Rational(22, 0) and
    Rational(-22, 9) + Rational(22, 0) == Rational(22, 0) and
    (Rational(-22, 0) + Rational(22, 0)).isNaN() and
  
    Rational(22, 0) + Rational(22, 9) == Rational(22, 0) and
    Rational(22, 9) + Rational(22, 9) == Rational(44, 9) and
    Rational(0, 9) + Rational(22, 9) == Rational(22, 9) and
    Rational(-22, 9) + Rational(22, 9) == Rational(0, 9) and
    Rational(-22, 0) + Rational(22, 9) == Rational(-22, 0) and
  
    Rational(22, 0) + Rational(0, 1) == Rational(22, 0) and
    Rational(22, 9) + Rational(0, 1) == Rational(22, 9) and
    Rational(0, 9) + Rational(0, 1) == Rational(0, 9) and
    Rational(-22, 9) + Rational(0, 1) == Rational(-22, 9) and
    Rational(-22, 0) + Rational(0, 1) == Rational(-22, 0) and
  
    Rational(22, 0) + Rational(-22, 9) == Rational(22, 0) and
    Rational(22, 9) + Rational(-22, 9) == Rational(0, 9) and
    Rational(0, 9) + Rational(-22, 9) == Rational(-22, 9) and
    Rational(-22, 9) + Rational(-22, 9) == Rational(-44, 9) and
    Rational(-22, 0) + Rational(-22, 9) == Rational(-22, 0) and
  
    (Rational(22, 0) + Rational(-22, 0)).isNaN() and
    Rational(22, 9) + Rational(-22, 0) == Rational(-22, 0) and
    Rational(0, 9) + Rational(-22, 0) == Rational(-22, 0) and
    Rational(-22, 9) + Rational(-22, 0) == Rational(-22, 0) and
    Rational(-22, 0) + Rational(-22, 0) == Rational(-22, 0) and
  
    (Rational(22, 0) + Rational(0, 0)).isNaN() and
    (Rational(22, 9) + Rational(0, 0)).isNaN() and
    (Rational(0, 9) + Rational(0, 0)).isNaN() and
    (Rational(-22, 9) + Rational(0, 0)).isNaN() and
    (Rational(-22, 0) + Rational(0, 0)).isNaN()
    ): print('Summation test passed')
else: print('Summation test failed')
  
if ((Rational(22, 0) - Rational(22, 0)).isNaN() and
    Rational(22, 9) - Rational(22, 0) == Rational(-22, 0) and
    Rational(0, 9) - Rational(22, 0) == Rational(-22, 0) and
    Rational(-22, 9) - Rational(22, 0) == Rational(-22, 0) and
    Rational(-22, 0) - Rational(22, 0) == Rational(-22, 0) and
  
    Rational(22, 0) - Rational(22, 9) == Rational(22, 0) and
    Rational(22, 9) - Rational(22, 9) == Rational(0, 9) and
    Rational(0, 9) - Rational(22, 9) == Rational(-22, 9) and
    Rational(-22, 9) - Rational(22, 9) == Rational(-44, 9) and
    Rational(-22, 0) - Rational(22, 9) == Rational(-22, 0) and
  
    Rational(22, 0) - Rational(0, 1) == Rational(22, 0) and
    Rational(22, 9) - Rational(0, 1) == Rational(22, 9) and
    Rational(0, 9) - Rational(0, 1) == Rational(0, 9) and
    Rational(-22, 9) - Rational(0, 1) == Rational(-22, 9) and
    Rational(-22, 0) - Rational(0, 1) == Rational(-22, 0) and
  
    Rational(22, 0) - Rational(-22, 9) == Rational(22, 0) and
    Rational(22, 9) - Rational(-22, 9) == Rational(44, 9) and
    Rational(0, 9) - Rational(-22, 9) == Rational(22, 9) and
    Rational(-22, 9) - Rational(-22, 9) == Rational(0, 9) and
    Rational(-22, 0) - Rational(-22, 9) == Rational(-22, 0) and
  
    Rational(22, 0) - Rational(-22, 0) == Rational(22, 0) and
    Rational(22, 9) - Rational(-22, 0) == Rational(22, 0) and
    Rational(0, 9) - Rational(-22, 0) == Rational(22, 0) and
    Rational(-22, 9) - Rational(-22, 0) == Rational(22, 0) and
    (Rational(-22, 0) - Rational(-22, 0)).isNaN() and
  
    (Rational(22, 0) - Rational(0, 0)).isNaN() and
    (Rational(22, 9) - Rational(0, 0)).isNaN() and
    (Rational(0, 9) - Rational(0, 0)).isNaN() and
    (Rational(-22, 9) - Rational(0, 0)).isNaN() and
    (Rational(-22, 0) - Rational(0, 0)).isNaN()
    ): print('Subtraction test passed')
else: print('Subtraction test failed')
  
if (Rational(22, 0) * Rational(22, 0) == Rational(22, 0) and
    Rational(22, 9) * Rational(22, 0) == Rational(22, 0) and
    (Rational(0, 9) * Rational(22, 0)).isNaN() and
    Rational(-22, 9) * Rational(22, 0) == Rational(-22, 0) and
    Rational(-22, 0) * Rational(22, 0) == Rational(-22, 0) and
  
    Rational(22, 0) * Rational(22, 9) == Rational(22, 0) and
    Rational(22, 9) * Rational(22, 9) == Rational(22*22, 9*9) and
    Rational(0, 9) * Rational(22, 9) == Rational(0, 9) and
    Rational(-22, 9) * Rational(22, 9) == Rational(-22*22, 9*9) and
    Rational(-22, 0) * Rational(22, 9) == Rational(-22, 0) and
  
    (Rational(22, 0) * Rational(0, 1)).isNaN() and
    Rational(22, 9) * Rational(0, 1) == Rational(0, 9) and
    Rational(0, 9) * Rational(0, 1) == Rational(0, 9) and
    Rational(-22, 9) * Rational(0, 1) == Rational(0, 9) and
    (Rational(-22, 0) * Rational(0, 1)).isNaN() and
  
    Rational(22, 0) * Rational(-22, 9) == Rational(-22, 0) and
    Rational(22, 9) * Rational(-22, 9) == Rational(-22*22, 9*9) and
    Rational(0, 9) * Rational(-22, 9) == Rational(0, 9) and
    Rational(-22, 9) * Rational(-22, 9) == Rational(22*22, 9*9) and
    Rational(-22, 0) * Rational(-22, 9) == Rational(22, 0) and
  
    Rational(22, 0) * Rational(-22, 0) == Rational(-22, 0) and
    Rational(22, 9) * Rational(-22, 0) == Rational(-22, 0) and
    (Rational(0, 9) * Rational(-22, 0)).isNaN() and
    Rational(-22, 9) * Rational(-22, 0) == Rational(22, 0) and
    Rational(-22, 0) * Rational(-22, 0) == Rational(22, 0) and
  
    (Rational(22, 0) * Rational(0, 0)).isNaN() and
    (Rational(22, 9) * Rational(0, 0)).isNaN() and
    (Rational(0, 9) * Rational(0, 0)).isNaN() and
    (Rational(-22, 9) * Rational(0, 0)).isNaN() and
    (Rational(-22, 0) * Rational(0, 0)).isNaN()
    ): print('Multiplication test passed')
else: print('Multiplication test failed')
  
if ((Rational(22, 0) / Rational(22, 0)).isNaN() and
    Rational(22, 9) / Rational(22, 0) == Rational(0, 9) and
    Rational(0, 9) / Rational(22, 0) == Rational(0, 9) and
    Rational(-22, 9) / Rational(22, 0) == Rational(0, 9) and
    (Rational(-22, 0) / Rational(22, 0)).isNaN() and
  
    Rational(22, 0) / Rational(22, 9) == Rational(22, 0) and
    Rational(22, 9) / Rational(22, 9) == Rational(9, 9) and
    Rational(0, 9) / Rational(22, 9) == Rational(0, 9) and
    Rational(-22, 9) / Rational(22, 9) == Rational(-9, 9) and
    Rational(-22, 0) / Rational(22, 9) == Rational(-22, 0) and
  
    Rational(22, 0) / Rational(0, 1) == Rational(22, 0) and
    Rational(22, 9) / Rational(0, 1) == Rational(22, 0) and
    (Rational(0, 9) / Rational(0, 1)).isNaN() and
    Rational(-22, 9) / Rational(0, 1) == Rational(-22, 0) and
    Rational(-22, 0) / Rational(0, 1) == Rational(-22, 0) and
  
    Rational(22, 0) / Rational(-22, 9) == Rational(-22, 0) and
    Rational(22, 9) / Rational(-22, 9) == Rational(-9, 9) and
    Rational(0, 9) / Rational(-22, 9) == Rational(0, 9) and
    Rational(-22, 9) / Rational(-22, 9) == Rational(9, 9) and
    Rational(-22, 0) / Rational(-22, 9) == Rational(22, 0) and
  
    (Rational(22, 0) / Rational(-22, 0)).isNaN() and
    Rational(22, 9) / Rational(-22, 0) == Rational(0, 9) and
    Rational(0, 9) / Rational(-22, 0) == Rational(0, 9) and
    Rational(-22, 9) / Rational(-22, 0) == Rational(0, 9) and
    (Rational(-22, 0) / Rational(-22, 0)).isNaN() and
  
    (Rational(22, 0) / Rational(0, 0)).isNaN() and
    (Rational(22, 9) / Rational(0, 0)).isNaN() and
    (Rational(0, 9) / Rational(0, 0)).isNaN() and
    (Rational(-22, 9) / Rational(0, 0)).isNaN() and
    (Rational(-22, 0) / Rational(0, 0)).isNaN()
    ): print('Division test passed')
else: print('Division test failed')
  
if (equal(float(Rational(-22, -9)), 22/9.0) and
    equal(float(Rational(-9, -9)), 1) and
    equal(float(Rational(-6, -9)), 6/9.0) and
    equal(float(Rational(0, -9)), 0) and
    equal(float(Rational(6, -9)), -6/9.0) and
    equal(float(Rational(9, -9)), -1) and
    equal(float(Rational(22, -9)), -22/9.0) and
    math.isinf(float(Rational(-9, 0))) and
    math.isnan(float(Rational(0, 0))) and
    math.isinf(float(Rational(9, 0))) and
    equal(float(Rational(-22, 9)), -22/9.0) and
    equal(float(Rational(-9, 9)), -1) and
    equal(float(Rational(-6, 9)), -6/9.0) and
    equal(float(Rational(0, 9)), 0) and
    equal(float(Rational(6, 9)), 6/9.0) and
    equal(float(Rational(9, 9)), 1) and
    equal(float(Rational(22, 9)), 22/9.0)
    ): print('Conversion to double test passed')
else: print('Conversion to double test failed')
  
if (bool(Rational(-22, -9)) and
    bool(Rational(-9, -9)) and
    bool(Rational(-6, -9)) and
    not bool(Rational(0, -9)) and
    bool(Rational(6, -9)) and
    bool(Rational(9, -9)) and
    bool(Rational(22, -9)) and
    bool(Rational(-9, 0)) and
    bool(Rational(0, 0)) and
    bool(Rational(9, 0)) and
    bool(Rational(-22, 9)) and
    bool(Rational(-9, 9)) and
    bool(Rational(-6, 9)) and
    not bool(Rational(0, 9)) and
    bool(Rational(6, 9)) and
    bool(Rational(9, 9)) and
    bool(Rational(22, 9))
    ): print('Conversion to bool test passed')
else: print('Conversion to bool test failed')
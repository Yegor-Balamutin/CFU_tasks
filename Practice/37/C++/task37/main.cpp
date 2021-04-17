#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double sqr(double a);
bool equal(double a, double b, double e = 1E-10);

const auto PI = 3.141592653589793;

class Point {
    double x;
    double y;
    double r;
    double phi;

    void set_new_r_phi(double x, double y) {
        this->r = sqrt(x * x + y * y);
        if (this->r == 0) this->phi = 0;
        else if ((x > 0) && (y >= 0)) this->phi = atan(y / x);
        else if ((x > 0) && (y < 0)) this->phi = atan(y / x) + 2 * PI;
        else if (x < 0) this->phi = atan(y / x) + PI;
        else if ((x == 0) && (y > 0)) this->phi = PI / 2;
        else if ((x == 0) && (y < 0)) this->phi = 3 * PI / 2;
    }

    void set_new_x_y(double r, double phi) {
        phi = correct_phi(phi);
        this->x = r * cos(phi);
        this->y = r * sin(phi);
    }

    double correct_phi(double phi) {
        if (phi > 0) {
            while ((phi - 2 * PI) >= 0) {
                phi -= 2 * PI;
            }
        }
        else {
            while ((phi + 2 * PI) <= 0) {
                phi += 2 * PI;
            }
        }
        return phi;
    }

public:
    enum class Coord {
        Cartesian,
        Polar
    };
    Point(double a1 = 0, double a2 = 0, Coord coord_system = Coord::Cartesian) {
        if (coord_system == Coord::Cartesian) {
            this->x = a1;
            this->y = a2;
            set_new_r_phi(a1, a2);
        }
        else {
            a2 = correct_phi(a2);
            this->r = a1;
            this->phi = a2;
            set_new_x_y(a1, a2);
        }
    }

    double get_x() {
        return x;
    }
    double get_y() {
        return y;
    }
    double get_r() {
        return r;
    }
    double get_phi() {
        return phi;
    }

    void set_x(double x) {
        this->x = x;
        set_new_r_phi(this->x, this->y);
    }
    void set_y(double y) {
        this->y = y;
        set_new_r_phi(this->x, this->y);
    }
    void set_r(double r) {
        this->r = r;
        set_new_x_y(this->r, this->phi);
    }
    void set_phi(double phi) {
        phi = correct_phi(phi);
        this->phi = phi;
        set_new_x_y(this->r, this->phi);
    }
};

class Vector{
public:
    double x;
    double y;

    Vector(){
        x = 1;
        y = 0;
    }
    Vector(Point end){
        x = end.get_x();
        y = end.get_y();
    }
    Vector(Point begin, Point end){
        x = end.get_x() - begin.get_x();
        y = end.get_y() - begin.get_y();
    }

    double length(){
        return sqrt(x*x + y*y);
    }

    // operators -------------------
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
};


int main()
{
    Vector a(Point(1, 2)), b(Point(-2, 0), Point(-1, 2));
    if (a == b && b == a) cout << "Equality test passed\n";
    else cout << "Equality test failed\n";

    Vector na(Point(-1, -2)), ox(Point(1, 0)), nox(Point(-1, 0)), oy(Point(0, 1)), noy(Point(0, -1));
    if (a == -na && na == -a && -ox == nox && -oy == noy) cout << "Invert test passed\n";
    else cout << "Invert test failed\n";

    if (ox + oy + oy == a && -ox == -a + oy + oy) cout << "Summation test passed\n";
    else cout << "Summation test failed\n";

    if (-ox + oy == oy - ox && -oy + ox == ox - oy) cout << "Subtraction test passed\n";
    else cout << "Subtraction test failed\n";

    if (ox * 3 == ox + ox + ox &&
        oy * 3 == oy + oy + oy &&
        ox * (-3) == -ox - ox - ox &&
        oy * (-3) == -oy - oy - oy) cout << "Multiplication by number test passed\n";
    else cout << "Multiplication by number test failed\n";

    if (equal(ox.length(), 1) &&
        equal(oy.length(), 1) &&
        equal((ox * 3 + oy * 4).length(), 5)) cout << "Length test passed\n";
    else cout << "Length test failed\n";

    if (equal(ox*ox, sqr(ox.length())) &&
        equal(oy*oy, sqr(oy.length())) &&
        equal((ox*3 + oy*4)*(ox*3 + oy*4), sqr((ox*3 + oy*4).length()))) cout << "Multiplication by Vector test passed\n";
    else cout << "Multiplication by Vector test failed\n";
}

bool equal(double a, double b, double e) {
    if (-e < a - b && a - b < e) return true;
    else return false;
}

double sqr(double a) {
    return a * a;
}

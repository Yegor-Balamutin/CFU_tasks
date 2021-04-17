#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

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

    bool operator==(Point& b) {
        if (fabs(x - b.get_x()) <= 0.0000000001) return true;
        else return false;
    }

    bool operator!=(Point& b) {
        if (fabs(x - b.get_x()) > 0.0000000001) return true;
        else return false;
    }
};


std::ostream& operator<<(std::ostream& out, Point& p) {
    out << "(" << p.get_x() << "," << p.get_y() << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Point& p) {
    double x, y;
    std::string str, numb;
    in >> str;
    int i = 1;
    numb = "";
    while (str[i] != ',') {
        numb += str[i];
        i++;
    }
    x = std::stod(numb);
    p.set_x(x);
    i++;
    numb = "";
    while (str[i] != ')') {
        numb += str[i];
        i++;
    }
    y = std::stod(numb);
    p.set_y(y);
    return in;
}

int main() {
    std::vector<Point> original;
    std::ifstream fin("data.txt");
    if (!fin.is_open()) {
        std::cout << "Can't open file" << std::endl;
        return 1;
    }
    else {
        while (!fin.eof()) {
            Point p;
            fin >> p;
            fin.ignore(2); // Точки разделены двумя символами ", "
            original.push_back(p);
        }
        fin.close();
    }

    std::vector<Point> simulacrum(original);
    for (auto& p : simulacrum) {
        std::cout << p;
        p.set_x(p.get_x() + 10);
        p.set_phi(p.get_phi() + 180 * PI / 180);
        p.set_y(-p.get_y());
        p.set_x(-p.get_x() - 10);
        std::cout << p << std::endl;
    }

    if (std::equal(original.begin(), original.end(), simulacrum.begin()))
        std::cout << "\nIt works!\n";
    else
        std::cout << "\nIt not works!\n";
}

#include <iostream>
#include <cmath>

using namespace std;

class Triangle {
protected:
    double a, b, c; 

public:
    Triangle(double sideA = 3, double sideB = 4, double sideC = 5) {
        // Перевірка чи може трикутник існувати
        if (sideA + sideB > sideC && sideA + sideC > sideB && sideB + sideC > sideA) {
            a = sideA;
            b = sideB;
            c = sideC;
        } else {
            a = 3;
            b = 4;
            c = 5;
        }
    }

    // Методи зміни сторін
    void setSides(double sideA, double sideB, double sideC) {
        if (sideA + sideB > sideC && sideA + sideC > sideB && sideB + sideC > sideA) {
            a = sideA;
            b = sideB;
            c = sideC;
        }
    }

    // Обчислення кутів (у градусах)
    double getAngleA() { return acos((b*b + c*c - a*a) / (2*b*c)) * 180 / M_PI; }
    double getAngleB() { return acos((a*a + c*c - b*b) / (2*a*c)) * 180 / M_PI; }
    double getAngleC() { return acos((a*a + b*b - c*c) / (2*a*b)) * 180 / M_PI; }

    // Обчислення периметра
    double getPerimeter() { return a + b + c; }

    void print() {
        cout << "Трикутник зі сторонами: " << a << ", " << b << ", " << c << endl;
        cout << "Кути: " << getAngleA() << "°, " << getAngleB() << "°, " << getAngleC() << "°" << endl;
        cout << "Периметр: " << getPerimeter() << endl;
    }
};

class RightAngled : public Triangle {
public:
    RightAngled(double sideA = 3, double sideB = 4, double sideC = 5) : Triangle(sideA, sideB, sideC) {}

    // Перевірка чи трикутник прямокутний
    bool isRight() {
        return (a*a + b*b == c*c) || (a*a + c*c == b*b) || (b*b + c*c == a*a);
    }

    // Обчислення площі
    double getArea() {
        if (a > b && a > c) return 0.5 * b * c;
        if (b > a && b > c) return 0.5 * a * c;
        return 0.5 * a * b;
    }

    void print() {
        Triangle::print();
        if (isRight()) {
            cout << "Площа: " << getArea() << endl;
            cout << "Це прямокутний трикутник" << endl;
        } else {
            cout << "Це не прямокутний трикутник" << endl;
        }
    }
};

int main() {
    // Тестування класу Triangle
    Triangle t1(5, 5, 5);
    t1.print();

    // Тестування класу RightAngled
    RightAngled r1(3, 4, 5);
    r1.print();

    RightAngled r2(6, 8, 10);
    r2.print();

    // Спроба створити не прямокутний трикутник
    RightAngled r3(2, 3, 4);
    r3.print();

    return 0;
}

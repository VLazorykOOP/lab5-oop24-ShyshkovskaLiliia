#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Vector {
protected:
    int* data;
    size_t size;

public:
    Vector() : data(nullptr), size(0) {}
    Vector(size_t s) : size(s) { data = new int[size](); }
    Vector(const Vector& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    ~Vector() { delete[] data; }

    size_t getSize() const { return size; }
    virtual int& operator[](size_t index) { return data[index]; }

    friend ostream& operator<<(ostream& os, const Vector& vec);
    friend istream& operator>>(istream& is, Vector& vec);
};

ostream& operator<<(ostream& os, const Vector& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size; i++) {
        os << vec.data[i];
        if (i != vec.size - 1) os << ", ";
    }
    os << "]";
    return os;
}

istream& operator>>(istream& is, Vector& vec) {
    for (size_t i = 0; i < vec.size; i++) {
        is >> vec.data[i];
    }
    return is;
}

class SafeVector : public Vector {
private:
    size_t lowerBound;
    size_t upperBound;

public:
    SafeVector(size_t s, size_t lower = 0, size_t upper = 0) 
        : Vector(s), lowerBound(lower), upperBound(upper ? upper : s - 1) {
        if (upperBound >= size) upperBound = size - 1;
    }
    SafeVector(const SafeVector& other) 
        : Vector(other), lowerBound(other.lowerBound), upperBound(other.upperBound) {}
    SafeVector& operator=(const SafeVector& other) {
        if (this != &other) {
            Vector::operator=(other); // Виклик базового оператора
            lowerBound = other.lowerBound;
            upperBound = other.upperBound;
        }
        return *this;
    }

    int& operator[](size_t index) override {
        if (index < lowerBound || index > upperBound) {
            throw out_of_range("Index " + to_string(index) + 
                             " is out of bounds [" + to_string(lowerBound) + 
                             ", " + to_string(upperBound) + "]");
        }
        return data[index];
    }
};

int main() {
    Vector v(5);
    for (size_t i = 0; i < v.getSize(); i++) {
        v[i] = static_cast<int>(i) * 10;
    }
    cout << "Base vector: " << v << endl;

    SafeVector sv(10, 2, 7);
    for (size_t i = 2; i <= 7; i++) {
        sv[i] = static_cast<int>(i) * 100;
    }
    cout << "Safe vector: " << sv << " (bounds: 2-7)" << endl;

    try {
        cout << "Trying to access sv[1]... ";
        sv[1] = 999;
    } catch (const out_of_range& e) {
        cout << "Error: " << e.what() << endl;
    }

    try {
        cout << "Trying to access sv[5]... ";
        sv[5] = 555;
        cout << "Success! Value: " << sv[5] << endl;
    } catch (const out_of_range& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}

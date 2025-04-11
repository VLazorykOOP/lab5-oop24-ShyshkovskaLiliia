#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Vector {
protected:
    int* data;      // Динамічний масив для зберігання елементів
    size_t size;    // Поточний розмір вектора

public:
    // Конструктор за замовчуванням
    Vector() : data(nullptr), size(0) {
        cout << "Vector default constructor\n";
    }

    // Конструктор з параметром розміру
    Vector(size_t s) : size(s) {
        data = new int[size];
        cout << "Vector size constructor (" << size << ")\n";
    }

    // Конструктор копіювання
    Vector(const Vector& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Vector copy constructor\n";
    }

    // Конструктор перенесення
    Vector(Vector&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Vector move constructor\n";
    }

    // Оператор присвоювання копіюванням
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        cout << "Vector copy assignment operator\n";
        return *this;
    }

    // Оператор присвоювання перенесенням
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        cout << "Vector move assignment operator\n";
        return *this;
    }

    // Деструктор
    virtual ~Vector() {
        delete[] data;
        cout << "Vector destructor\n";
    }

    // Функція для отримання розміру вектора
    size_t getSize() const { return size; }

    // Оператор доступу до елемента (без перевірки меж)
    virtual int& operator[](size_t index) {
        return data[index];
    }
};

  // Дружні функції для введення/виведення
    friend ostream& operator<<(ostream& os, const Vector& vec);
    friend istream& operator>>(istream& is, Vector& vec);

// Оператор виведення вектора у потік
ostream& operator<<(ostream& os, const Vector& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size; i++) {
        os << vec.data[i];
        if (i != vec.size - 1) os << ", ";
    }
    os << "]";
    return os;
}

// Оператор введення вектора з потоку
istream& operator>>(istream& is, Vector& vec) {
    cout << "Enter vector size: ";
    is >> vec.size;
    
    delete[] vec.data;
    vec.data = new int[vec.size];
    
    cout << "Enter " << vec.size << " elements: ";
    for (size_t i = 0; i < vec.size; i++) {
        is >> vec.data[i];
    }
    
    return is;
}

class SafeVector : public Vector {
private:
    size_t lowerBound;  // Нижня межа індексу
    size_t upperBound;  // Верхня межа індексу

public:
    // Конструктор за замовчуванням
    SafeVector() : Vector(), lowerBound(0), upperBound(0) {
        cout << "SafeVector default constructor\n";
    }

    // Конструктор з параметрами розміру та меж
    SafeVector(size_t s, size_t lower = 0, size_t upper = 0) 
        : Vector(s), lowerBound(lower), upperBound(upper ? upper : s - 1) {
        if (upperBound >= size) {
            upperBound = size - 1;
        }
        cout << "SafeVector parameterized constructor (lower=" 
             << lowerBound << ", upper=" << upperBound << ")\n";
    }

    // Конструктор копіювання
    SafeVector(const SafeVector& other) 
        : Vector(other), lowerBound(other.lowerBound), upperBound(other.upperBound) {
        cout << "SafeVector copy constructor\n";
    }

    // Конструктор перенесення
    SafeVector(SafeVector&& other) noexcept 
        : Vector(move(other)), lowerBound(other.lowerBound), upperBound(other.upperBound) {
        other.lowerBound = 0;
        other.upperBound = 0;
        cout << "SafeVector move constructor\n";
    }

    // Оператор присвоювання копіюванням
    SafeVector& operator=(const SafeVector& other) {
        if (this != &other) {
            Vector::operator=(other);
            lowerBound = other.lowerBound;
            upperBound = other.upperBound;
        }
        cout << "SafeVector copy assignment operator\n";
        return *this;
    }

    // Оператор присвоювання перенесенням
    SafeVector& operator=(SafeVector&& other) noexcept {
        if (this != &other) {
            Vector::operator=(move(other));
            lowerBound = other.lowerBound;
            upperBound = other.upperBound;
            other.lowerBound = 0;
            other.upperBound = 0;
        }
        cout << "SafeVector move assignment operator\n";
        return *this;
    }

    // Деструктор
    ~SafeVector() override {
        cout << "SafeVector destructor\n";
    }

    // Перевизначений оператор доступу з перевіркою меж
    int& operator[](size_t index) override {
        if (index < lowerBound || index > upperBound) {
            throw out_of_range("Index " + to_string(index) + 
                             " is out of bounds [" + to_string(lowerBound) + 
                             ", " + to_string(upperBound) + "]");
        }
        return data[index];
    }

    // Встановлення нових меж
    void setBounds(size_t lower, size_t upper) {
        if (lower > upper || upper >= size) {
            throw invalid_argument("Invalid bounds");
        }
        lowerBound = lower;
        upperBound = upper;
    }

    // Отримання нижньої межі
    size_t getLowerBound() const { return lowerBound; }

    // Отримання верхньої межі
    size_t getUpperBound() const { return upperBound; }
};

int main() {
    cout << "=== Testing Vector class ===\n";
    Vector v1(5);
    for (size_t i = 0; i < v1.getSize(); i++) {
        v1[i] = static_cast<int>(i) * 10;
    }
    cout << "v1: " << v1 << endl;

    Vector v2 = v1; // Копіювання
    cout << "v2 (copy of v1): " << v2 << endl;

    Vector v3 = move(v1); // Перенесення
    cout << "v3 (moved from v1): " << v3 << endl;
    cout << "v1 after move: size=" << v1.getSize() << endl;

    cout << "\n=== Testing SafeVector class ===\n";
    SafeVector sv1(10, 2, 7);
    for (size_t i = sv1.getLowerBound(); i <= sv1.getUpperBound(); i++) {
        sv1[i] = static_cast<int>(i) * 100;
    }
    cout << "sv1: " << sv1 << " (bounds: " << sv1.getLowerBound() 
         << "-" << sv1.getUpperBound() << ")\n";

    // Тестування копіювання
    SafeVector sv2 = sv1;
    cout << "sv2 (copy of sv1): " << sv2 << endl;

    // Тестування перенесення
    SafeVector sv3 = move(sv1);
    cout << "sv3 (moved from sv1): " << sv3 << endl;
    cout << "sv1 after move: size=" << sv1.getSize() 
         << ", bounds: " << sv1.getLowerBound() << "-" << sv1.getUpperBound() << endl;

    // Тестування оператора присвоювання
    SafeVector sv4;
    sv4 = sv3;
    cout << "sv4 (assigned from sv3): " << sv4 << endl;

    // Тестування виходу за межі
    try {
        cout << "Trying to access sv4[1]... ";
        sv4[1] = 999; // Це має викликати виняток
    } catch (const out_of_range& e) {
        cout << "Exception: " << e.what() << endl;
    }

    try {
        cout << "Trying to access sv4[5]... ";
        sv4[5] = 555; // Це має працювати
        cout << "Success! Value: " << sv4[5] << endl;
    } catch (const out_of_range& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}

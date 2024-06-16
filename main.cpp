#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template <typename T>
class Matrix {
private:
    vector<vector<T>> data;
    size_t rows;
    size_t cols;

public:
    // Конструкторы
    Matrix() : rows(0), cols(0) {}

    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        data.resize(rows, vector<T>(cols, T()));
    }

    // Перегрузка операторов для ввода и вывода
    friend istream& operator>>(istream& is, Matrix& matrix) {
        cout << "Введите количество строк и столбцов матрицы: ";
        is >> matrix.rows >> matrix.cols;
        matrix.data.resize(matrix.rows, vector<T>(matrix.cols));
        cout << "Введите элементы матрицы:" << endl;
        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                is >> matrix.data[i][j];
            }
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                os << matrix.data[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    // Перегрузка мат. операторов
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw runtime_error("Размеры матриц должны совпадать");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw runtime_error("Размеры матриц должны совпадать");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw runtime_error("Неправильные размеры матриц для их умножения");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                result.data[i][j] = T();
                for (size_t k = 0; k < cols; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) {
            return *this;
        }
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        return *this;
    }

    // Метод для вычисления обратной матрицы
    Matrix operator!() const {
        if (rows != cols) {
            throw runtime_error("Матрица должна быть квадратной");
        }
        size_t n = rows;
        Matrix<T> result(n, n);
        Matrix<T> augmented(n, 2 * n);

        // Создаем расширенную матрицу
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                augmented.data[i][j] = data[i][j];
                augmented.data[i][j + n] = (i == j) ? 1 : 0;
            }
        }

        // Приводим к ступенчатому виду
        for (size_t i = 0; i < n; ++i) {
            if (augmented.data[i][i] == 0) {
                size_t swapRow = i + 1;
                while (swapRow < n && augmented.data[swapRow][i] == 0) {
                    ++swapRow;
                }
                if (swapRow == n) {
                    throw runtime_error("Матрица вырождена");
                }
                swap(augmented.data[i], augmented.data[swapRow]);
            }

            T diagValue = augmented.data[i][i];
            for (size_t j = 0; j < 2 * n; ++j) {
                augmented.data[i][j] /= diagValue;
            }

            for (size_t row = 0; row < n; ++row) {
                if (row != i) {
                    T factor = augmented.data[row][i];
                    for (size_t col = 0; col < 2 * n; ++col) {
                        augmented.data[row][col] -= factor * augmented.data[i][col];
                    }
                }
            }
        }

        // Извлекаем обратную матрицу
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                result.data[i][j] = augmented.data[i][j + n];
            }
        }

        return result;
    }

    // Методы доступа
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Статические методы для создания нулевой и единичной матриц
    static Matrix zero(size_t rows, size_t cols) {
        return Matrix(rows, cols);
    }

    static Matrix identity(size_t size) {
        Matrix result(size, size);
        for (size_t i = 0; i < size; ++i) {
            result.data[i][i] = 1;
        }
        return result;
    }
};

// Пример использования класса Matrix
int main() {
    setlocale(LC_ALL, "Russian");

    try {
        // Создание первой матрицы произвольного размера и чтение данных из консоли
        Matrix<int> mat1;
        cout << "Введите первую матрицу" << endl;
        cin >> mat1;

        cout << "Размеры матрицы 1: " << mat1.getRows() << "x" << mat1.getCols() << endl;
        cout << "Матрица 1:\n" << mat1;

        // Создание второй матрицы произвольного размера и чтение данных из консоли
        Matrix<int> mat2;
        cout << "Введите вторую матрицу:" << endl;
        cin >> mat2;

        cout << "Размеры матрицы 2" << mat2.getRows() << "x" << mat2.getCols() << endl;
        cout << "Матрица 2:\n" << mat2;

        // Сложение матриц
        if (mat1.getRows() == mat2.getRows() && mat1.getCols() == mat2.getCols()) {
            Matrix<int> mat3 = mat1 + mat2;
            cout << "Сумма:\n" << mat3;
        }
        else {
            cout << "Матрицы имеют разные размеры, сложение невозможно." << endl;
        }

        // Вычитание матриц
        if (mat1.getRows() == mat2.getRows() && mat1.getCols() == mat2.getCols()) {
            Matrix<int> mat4 = mat1 - mat2;
            cout << "Разница:\n" << mat4;
        }
        else {
            cout << "Матрицы имеют разные размеры, вычитание невозможно." << endl;
        }

        // Умножение матриц
        if (mat1.getCols() == mat2.getRows()) {
            Matrix<int> mat5 = mat1 * mat2;
            cout << "Произведение:\n" << mat5;
        }
        else {
            cout << "Неправильные размеры матриц для их умножения." << endl;
        }

        // Умножение матрицы на скаляр
        int scalar;
        cout << "Введите скаляр для умножения первой матрицы: ";
        cin >> scalar;
        Matrix<int> mat6 = mat1 * scalar;
        cout << "Умножение на скаляр\n" << mat6;

        // Обратная матрица1
        if (mat1.getRows() == mat1.getCols()) {
            Matrix<int> mat7 = !mat1;
            cout << "Обратная матрица:\n" << mat7;
        }
        else {
            cout << "Матрица должна быть квадратной для вычисления обратной." << endl;
        }

        // Обратная матрица2
        if (mat2.getRows() == mat2.getCols()) {
            Matrix<int> mat8 = !mat2;
            cout << "Обратная матрица:\n" << mat8;
        }
        else {
            cout << "Матрица должна быть квадратной для вычисления обратной." << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}

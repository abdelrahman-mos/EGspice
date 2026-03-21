#ifndef Matrix_hpp
#define Matrix_hpp
#include <vector>
#include <iostream>

template <typename T>
class Matrix {
public:
    Matrix(size_t rows, size_t cols) : data(rows, std::vector<T>(cols)) {}
    
    std::vector<T>& operator[](size_t index) {
        return data[index];
    }

    const std::vector<T>& operator[](size_t index) const {
        return data[index];
    }

    size_t numRows() const {
        return data.size();
    }

    size_t numCols() const {
        return data.empty() ? 0 : data[0].size();
    }

    Matrix<T> operator*(const Matrix<T>& mul) {
        if (this->numCols() != mul.numRows()) {
            std::cout << "Cannot multiply, dimenstions don't match" << std::endl;
        }

        Matrix<T> result(this->numRows(), mul.numCols());
        for (size_t i = 0; i < this->numRows(); ++i) {
            for (size_t j = 0; j < mul.numCols(); ++j) {
                T sum = 0;
                for (size_t k = 0; k < this->numCols(); ++k) {
                    sum += (*this)[i][k] * mul[k][j];
                }
                result[i][j] = sum;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat) {
        for (size_t i = 0; i < mat.numRows(); i++) {
            for (size_t j = 0; j < mat.numCols(); j++) {
                os << mat[i][j] << "\t";
            }
            os << "\n";
        }
        return os;
    }
    
    friend std::ostream& operator<<(std::ostream& os, std::shared_ptr<Matrix<T>> mat) {
        for (size_t i = 0; i < (*mat).numRows(); i++) {
            for (size_t j = 0; j < (*mat).numCols(); j++) {
                os << (*mat)[i][j] << "\t";
            }
            os << "\n";
        }
        return os;
    }
private:
    std::vector<std::vector<T>> data;
};

#endif /* Matrix_hpp */
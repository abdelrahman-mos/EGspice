#ifndef Matrix_hpp
#define Matrix_hpp
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#define MX_ATOL 1e-18

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

    void fillZeros() {
        for (size_t i = 0; i < this->numRows(); i++) {
            std::fill(data[i].begin(), data[i].end(), 0);
        }
    }

    Matrix<T> operator*(const Matrix<T>& mul) {
        if (this->numCols() != mul.numRows()) {
            std::cout << "Cannot multiply, dimensions don't match" << std::endl;
            return nullptr;
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

    Matrix<T> operator*(const T num) {
        Matrix<T> result(this->numRows(), this->numCols());
        for (size_t i = 0; i < this->numRows(); i++) {
            for (size_t j = 0; j < this->numCols(); j++) {
                result[i][j] = *(this)[i][j] * num;
            }
        }
        return result;
    }

    static std::shared_ptr<Matrix<T>> back_substitution(std::shared_ptr<Matrix<T>> coeff, std::shared_ptr<Matrix<T>> free_term) {
        auto output = std::make_shared<Matrix<T>>(free_term->numRows(), 1);
        for (int i = free_term->numRows()-1; i >= 0; i--) {
            T sum = 0.0;
            for (size_t j = i+1; j < free_term->numRows(); j++) {
                sum += (*coeff)[i][j] * (*output)[j][0];
            }
            (*output)[i][0] = ((*free_term)[i][0] - sum) / (*coeff)[i][i];
        }
        return output;
    }

    static void gaussian_elimination(std::shared_ptr<Matrix<T>> coeff, std::shared_ptr<Matrix<T>> free_term, T* det) {
        for (size_t i = 0; i < coeff->numCols(); i++) {
            size_t pivot = i;
            for (size_t row = i+1; row < coeff->numRows(); row++) {
                if (std::abs((*coeff)[row][i]) > std::abs((*coeff)[pivot][i])) pivot = row;
            }

            if (std::abs((*coeff)[pivot][i]) < MX_ATOL) {
                if (det) *det = 0.0;
                std::cout << "matrix is singular, cannot solve" << std::endl;
                break;
            }

            if (pivot != i) {
                std::vector<T> tmp_arr = (*coeff)[i];
                (*coeff)[i] = (*coeff)[pivot];
                (*coeff)[pivot] = tmp_arr;
                if (det) *det = -*det; // swapping row flips the sign
                if (free_term) {
                    tmp_arr = (*free_term)[i];
                    (*free_term)[i] = (*free_term)[pivot];
                    (*free_term)[pivot] = tmp_arr;
                }
            }

            if (det) *det *= (*coeff)[i][i];

            // eliminate row below
            for (size_t row = i+1; row < coeff->numRows(); row++) {
                T factor = (*coeff)[row][i] / (*coeff)[i][i];
                if (free_term) {
                    (*free_term)[row][0] -= factor * (*free_term)[i][0];
                }
                for (size_t col = i; col < coeff->numCols(); col++) {
                    (*coeff)[row][col] -= factor * (*coeff)[i][col];
                }
            }
        }
    }

    static std::shared_ptr<Matrix<T>> solve_matrix(std::shared_ptr<Matrix<T>> A, std::shared_ptr<Matrix<T>> B) {
        if (A->numRows() != A->numCols()) {
            std::cout << "Matrix A is not square." << std::endl;
        }
        auto coeff_matrix = std::make_shared<Matrix<T>>(*A);
        auto free_terms = std::make_shared<Matrix<T>>(*B);

        gaussian_elimination(coeff_matrix, free_terms, nullptr);
        auto output = back_substitution(coeff_matrix, free_terms);

        return output;
    }

    void ressize_matrix(int new_rows, int new_cols) {
        data.resize(new_rows);
        for (size_t i = 0; i < data.size(); i++) {
            data[i].resize(new_cols);
        }
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
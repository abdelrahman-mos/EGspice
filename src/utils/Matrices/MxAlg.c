#include "../../../include/utils/MxAlg.h"
#include <stdarg.h>

Matrix* mat_add(Matrix* mat_A, Matrix* mat_B) {
    if ((mat_A->nRows != mat_B->nRows) || (mat_A->nCols != mat_B->nCols)) {
        fprintf(stderr, "matrices dimensions don't match. mat A of size %dx%d, mat B of size %dx%d\n", 
            mat_A->nRows, mat_A->nCols, mat_B->nRows, mat_B->nCols);
        return NULL;
    }
    
    Matrix* output = create_matrix(mat_A->nRows, mat_A->nCols, MFT_NONE);

    for (int r = 0; r < mat_A->nRows; r++) {
        for (int c = 0; c < mat_A->nCols; c++) {
            output->pValues[r][c] = mat_A->pValues[r][c] + mat_B->pValues[r][c];
        }
    }

    return output;
}

Matrix* mat_sub(Matrix* mat_A, Matrix* mat_B) {
    if ((mat_A->nRows != mat_B->nRows) || (mat_A->nCols != mat_B->nCols)) {
        fprintf(stderr, "matrices dimensions don't match. mat A of size %dx%d, mat B of size %dx%d\n", 
            mat_A->nRows, mat_A->nCols, mat_B->nRows, mat_B->nCols);
        return NULL;
    }
    
    Matrix* output = create_matrix(mat_A->nRows, mat_A->nCols, MFT_NONE);

    for (int r = 0; r < mat_A->nRows; r++) {
        for (int c = 0; c < mat_A->nCols; c++) {
            output->pValues[r][c] = mat_A->pValues[r][c] - mat_B->pValues[r][c];
        }
    }

    return output;
}

Matrix* mat_sum(int num, ...) {
    va_list input_list;
    va_start(input_list, num);
    Matrix* mat = va_arg(input_list, Matrix*);
    Matrix* output = create_matrix(mat->nRows, mat->nCols, MFT_ZEROS);
    output = mat_add(output, mat);
    for (int i = 1; i < num; i++) {
        mat = va_arg(input_list, Matrix*);
        output = mat_add(output, mat);
    }
    va_end(input_list);
    return output;
}

// output matrix must be zero filled
void normal_mul(Matrix* mat_A, Matrix* mat_B, Matrix* output) {
    /*
    Cij = sum_k Aik*Bkj
    */
    for (int i = 0; i < output->nRows; i++) {
        for (int j = 0; j < output->nCols; j++) {
            for (int k = 0; k < mat_A->nCols; k++) {
                output->pValues[i][j] += mat_A->pValues[i][k] * mat_B->pValues[k][j];
            }
        }
    }
}

// still not implemented
// not valid in this use case lol
void strassen_mul(Matrix* mat_A, Matrix* mat_B, Matrix* output) {
    return normal_mul(mat_A, mat_B, output);
}

Matrix* mat_mul(Matrix* mat_A, Matrix* mat_B) {
    if (mat_A->nCols != mat_B->nRows) {
        fprintf(stderr, "matrices inner dimensions don't match. mat A of size %dx%d, mat B of size %dx%d\n", 
            mat_A->nRows, mat_A->nCols, mat_B->nRows, mat_B->nCols);
        return NULL;
    }

    Matrix* output = create_matrix(mat_A->nRows, mat_B->nCols, MFT_ZEROS);
    normal_mul(mat_A, mat_B, output);

    return output;
}

Matrix* mat_transpose(Matrix* mat) {
    Matrix* output = create_matrix(mat->nCols, mat->nRows, MFT_NONE);
    for (int r = 0; r < mat->nRows; r++) {
        for (int c = 0; c < mat->nCols; c++) {
            output->pValues[c][r] = mat->pValues[r][c]; 
        }
    }
    return output;
}

Matrix* back_substitution(Matrix* matA, Matrix* matB) {
    Matrix* output = create_matrix(matB->nRows, 1, MFT_ZEROS);
    for (int i = matB->nRows-1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i+1; j < matB->nRows; j++) {
            sum += matA->pValues[i][j] * output->pValues[j][0];
        }
        output->pValues[i][0] = (matB->pValues[i][0] - sum) / matA->pValues[i][i];
    }
    return output;
}

// takes a mutable matrix that will change
// do not use matrices that you don't want to change
void gaussian_elimination(Matrix* matA, Matrix* matB, double complex* det) {
    for (int i = 0; i < matA->nCols; i++) {
        int pivot = i;
        for (int row = i+1; row < matA->nRows; row++) {
            if (cabs(matA->pValues[row][i]) > cabs(matA->pValues[pivot][i])) pivot = row;
        }

        if (cabs(matA->pValues[pivot][i]) < MX_ATOL) {
            // matrix is singular
            if (det) *det = 0.0;
            fprintf(stderr, "matrix A is singular, cannot solve\n");
            break;
        }

        if (pivot != i) {
            // we need to swap rows here
            double complex* tmp_arr = matA->pValues[i];
            matA->pValues[i] = matA->pValues[pivot];
            matA->pValues[pivot] = tmp_arr;
            if (det) *det = -*det; // swapping a row flips the sign
            if (matB) {
                tmp_arr = matB->pValues[i];
                matB->pValues[i] = matB->pValues[pivot];
                matB->pValues[pivot] = tmp_arr;
            }
        }

        if (det) *det *= matA->pValues[i][i]; // multiply diagonal elements

        // eliminate row below
        for (int row = i+1; row < matA->nRows; row++) {
            double factor = matA->pValues[row][i] / matA->pValues[i][i];
            if (matB) {
                matB->pValues[row][0] -= factor * matB->pValues[i][0];
            }
            for (int col = i; col < matA->nCols; col++) {
                matA->pValues[row][col] -= factor * matA->pValues[i][col];
            }
        }
    }
}

Matrix* solve_matrix(Matrix* matA, Matrix* matB) {
    if ((matA != NULL) && (matA->nRows != matA->nCols)) {
        fprintf(stderr, "Matrix A is not square, thus number of equations is not equal to the number of variables. Matrix\
            A of size %dx%d", matA->nRows, matA->nCols);
            return NULL;
    }
    if ((matB != NULL) && (matB->nCols != 1)) {
        fprintf(stderr, "Free term matrix cannot have more than one column, matrix B of size %dx%d\n",
        matB->nRows, matB->nCols);
        return NULL;
    }
    if ((matB != NULL) && (matA->nCols != matB->nRows)) {
        fprintf(stderr, "Inconsistent number of equations vs number of free terms.\n");
        return NULL;
    }
    Matrix* coefficients = copy_matrix(matA);
    Matrix* free_terms = copy_matrix(matB);
    gaussian_elimination(coefficients, free_terms, NULL);
    // make gaussian elimination then return the back substitution output
    Matrix* output = back_substitution(coefficients, free_terms);
    destroy_matrix(coefficients);
    destroy_matrix(free_terms);
    return output;
}

// implemented using gaussian elimination (might use same algorithm for solve)
double complex mat_determinant(Matrix* mat) {
    if (mat->nCols != mat->nRows) {
        fprintf(stderr, "matrix must be a square to calculate determinant, matrix size %dx%d", mat->nCols, mat->nRows);
        return -INFINITY;
    }

    double complex det = 1.0;
    Matrix* tmp = copy_matrix(mat);
    gaussian_elimination(mat, NULL, &det);

    destroy_matrix(tmp);
    return det;
}

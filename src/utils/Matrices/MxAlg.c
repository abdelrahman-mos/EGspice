#include "../../../include/utils/MxAlg.h"
#include <stdarg.h>
#include <math.h>

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
    if ((mat_A->nRows == mat_A->nCols) && (log2(mat_A->nRows)-(int)log2(mat_A->nRows) == 0)) {
        // square matrix with a power of 2 size
        strassen_mul(mat_A, mat_B, output);
    } else {
        normal_mul(mat_A, mat_B, output);
    }

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

// takes a mutable matrix that will change
// do not use matrices that you don't want to change
void gaussian_elimination(Matrix* mat, double* det) {
    for (int i = 0; i < mat->nCols; i++) {
        printf("matrix now:\n");
        print_matrix(mat);
        int pivot = i;
        for (int row = i+1; row < mat->nRows; row++) {
            if (fabs(mat->pValues[row][i]) > fabs(mat->pValues[pivot][i])) pivot = row;
        }

        if (fabs(mat->pValues[pivot][i]) < MX_ATOL) {
            // matrix is singular
            if (det) *det = 0.0;
            break;
        }

        if (pivot != i) {
            // we need to swap rows here
            double* tmp_arr = mat->pValues[i];
            mat->pValues[i] = mat->pValues[pivot];
            mat->pValues[pivot] = tmp_arr;
            if (det) *det = -*det; // swapping a row flips the sign
        }

        if (det) *det *= mat->pValues[i][i]; // multiply diagonal elements

        // eliminate row below
        for (int row = i+1; row < mat->nRows; row++) {
            double factor = mat->pValues[row][i] / mat->pValues[i][i];
            for (int col = i; col < mat->nCols; col++) {
                mat->pValues[row][col] -= factor * mat->pValues[i][col];
            }
        }
    }
}

// implemented using gaussian elimination (might use same algorithm for solve)
double mat_determinant(Matrix* mat) {
    if (mat->nCols != mat->nRows) {
        fprintf(stderr, "matrix must be a square to calculate determinant, matrix size %dx%d", mat->nCols, mat->nRows);
        return -INFINITY;
    }

    double det = 1.0;
    Matrix* tmp = copy_matrix(mat);
    gaussian_elimination(mat, &det);

    destroy_matrix(tmp);
    return det;
}

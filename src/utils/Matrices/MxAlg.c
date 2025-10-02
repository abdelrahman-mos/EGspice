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
void strassen_mul(Matrix* mat_A, Matrix* mat_B, Matrix* output) {
    return normal_mul(mat_A, mat_B, output);
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

double mat_determinant(Matrix* mat);
#include "../../../include/utils/MxAlg.h"

Matrix* create_matrix(int nRows, int nCols, MatFillType fill_type) {
    Matrix* output = (Matrix*)malloc(sizeof(Matrix));
    output->nRows = nRows;
    output->nCols = nCols;
    output->pValues = (double complex**)calloc(nRows, sizeof(double complex*)); // allocate memory for number of rows (single column of arrays)
    for (int r = 0; r < nRows; r++) {
        output->pValues[r] = (double complex*)calloc(nCols, sizeof(double complex)); // allocate memory for each row with size of numCols
    }

    if (fill_type != MFT_NONE) {
        for (int r = 0; r < nRows; r++) {
            for (int c = 0; c < nCols; c++) {
                switch (fill_type)
                {
                case MFT_ZEROS:
                    output->pValues[r][c] = 0.0;
                    break;
                case MFT_ONES:
                    output->pValues[r][c] = 1.0;
                    break;
                case MFT_RAND:
                    output->pValues[r][c] = rand()/(double)RAND_MAX;
                    break;
                case MFT_IDENTITY:
                    output->pValues[r][c] = (r == c) ? 1.0 : 0.0;
                    break;
                case MFT_CMPLX_ONES:
                    output->pValues[r][c] = 1*I;
                    break;
                default:
                    return output;
                }
            }
        }
    }
    return output;
}

Matrix* copy_matrix(Matrix* matrix) {
    Matrix* output = create_matrix(matrix->nRows, matrix->nCols, MFT_NONE);
    for (int i = 0; i < matrix->nRows; i++) {
        for (int j = 0; j < matrix->nCols; j++) {
            output->pValues[i][j] = matrix->pValues[i][j];
        }
    }
    return output;
}

Matrix* reshape_matrix(Matrix* mat, int new_rows, int new_cols, MatFillType fill_type) {
    if ((new_rows == mat->nRows) && (new_cols == mat->nCols)) {
        // no reshaping happens here
        return mat;
    }
    Matrix* output = create_matrix(new_rows, new_cols, fill_type);
    int total_rows, total_cols;
    total_rows = (new_rows < mat->nRows) ? new_rows : mat->nRows;
    total_cols = (new_cols < mat->nCols) ? new_cols : mat->nCols;
    // copy existing values and keep the rest as fill_type
    for (int i = 0; i < total_rows; i++) {
        for (int j = 0; j < total_cols; j++) {
            output->pValues[i][j] = mat->pValues[i][j];
        }
    }
    return output;
}

void destroy_matrix(Matrix* matrix) {
    if (!matrix) return;
    for (int r = 0; r < matrix->nRows; r++) {
        free(matrix->pValues[r]);
    }
    free(matrix->pValues);
    free(matrix);
}

void print_matrix(Matrix* matrix) {
    if (!matrix) return;
    for (int r = 0; r < matrix->nRows; r++) {
        for (int c = 0; c < matrix->nCols; c++) {
            double imag = cimag(matrix->pValues[r][c]);
            printf("%.15lf%s%.15lfi\t",
                creal(matrix->pValues[r][c]),
                (imag >= 0.0) ? "+" : "",
                imag
            );
        }
        printf("\n");
    }
}
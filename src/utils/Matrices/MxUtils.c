#include "../../../include/utils/MxAlg.h"

Matrix* create_matrix(int nRows, int nCols, MatFillType fill_type) {
    Matrix* output = (Matrix*)malloc(sizeof(Matrix));
    output->nRows = nRows;
    output->nCols = nCols;
    output->pValues = (double**)calloc(nRows, sizeof(double*)); // allocate memory for number of rows (single column of arrays)
    for (int r = 0; r < nRows; r++) {
        output->pValues[r] = (double*)calloc(nCols, sizeof(double)); // allocate memory for each row with size of numCols
    }

    if (fill_type != MFT_NONE) {
        for (int r = 0; r < nRows; r++) {
            for (int c = 0; c < nCols; c++) {
                switch (fill_type)
                {
                case MFT_ZEROS:
                    output->pValues[r][c] = 0;
                    break;
                case MFT_ONES:
                    output->pValues[r][c] = 1;
                    break;
                case MFT_RAND:
                    output->pValues[r][c] = rand()/(double)RAND_MAX;
                    break;
                case MFT_IDENTITY:
                    output->pValues[r][c] = (r == c) ? 1 : 0;
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
            printf("%.15lf\t", matrix->pValues[r][c]);
        }
        printf("\n");
    }
}
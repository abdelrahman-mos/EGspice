#ifndef MXALG_H
#define MXALG_H

#include <stdio.h>
#include <stdlib.h>

#define MX_ATOL 1e-18

typedef struct
{
    int nRows;
    int nCols;
    double** pValues;
} Matrix;

typedef enum {MFT_NONE, MFT_ZEROS, MFT_ONES, MFT_RAND, MFT_IDENTITY} MatFillType;

Matrix* create_matrix(int nRows, int nCols, MatFillType fill_type);
Matrix* copy_matrix(Matrix* matrix);
void destroy_matrix(Matrix* matrix);
void print_matrix(Matrix* matrix);
Matrix* mat_add(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_sub(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_sum(int num, ...);
Matrix* mat_mul(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_transpose(Matrix* mat);
double mat_determinant(Matrix* mat);

#endif
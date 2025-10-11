#ifndef MXALG_H
#define MXALG_H

#include <stdio.h>
#include <stdlib.h>
#include "mathutils.h"

typedef struct
{
    int nRows;
    int nCols;
    double complex** pValues;
} Matrix;

typedef enum {MFT_NONE, MFT_ZEROS, MFT_ONES, MFT_RAND, MFT_IDENTITY, MFT_CMPLX_ONES} MatFillType;

Matrix* create_matrix(int nRows, int nCols, MatFillType fill_type);
Matrix* copy_matrix(Matrix* matrix);
void destroy_matrix(Matrix* matrix);
void print_matrix(Matrix* matrix);
Matrix* mat_add(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_sub(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_sum(int num, ...);
Matrix* mat_mul(Matrix* mat_A, Matrix* mat_B);
Matrix* mat_transpose(Matrix* mat);
Matrix* solve_matrix(Matrix* matA, Matrix* matB);
Matrix* reshape_matrix(Matrix* mat, int new_rows, int new_cols, MatFillType fill_type);
double complex mat_determinant(Matrix* mat);

#endif
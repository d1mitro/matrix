#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "s21_matrix.h"

int matrix_check(s21_matrix *A, s21_matrix *B) {
    if (A == NULL || B == NULL) return 1;
    if (A->matrix == NULL || B->matrix == NULL) return 1;
    if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) return 1;
    if (A->rows != B->rows || A->columns != B->columns) return 2;
    return 0;
}
int check_for_mult(s21_matrix *A, s21_matrix *B) {
    if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL || A->rows == NULL || A->columns == NULL || B->rows == NULL || B->columns == NULL) {
        return 1;
    }
    if (A->rows != B->columns || A->columns != B->rows) {
        return 2;
    }
    return 0;
}
int s21_create_matrix(int rows, int columns, s21_matrix *result) {
    if (rows <= 0 || columns <= 0) { return 1; }
    result->matrix = (double**)malloc(rows * sizeof(double *));
    
    for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)malloc(sizeof(double) * columns); 
        for (int j = 0; j < columns; j++) {
            result->matrix[i][j] = 0.0;
        }
    }
    result->rows = rows;
    result->columns = columns;
    return 0;
}
void s21_remove_matrix(s21_matrix *A) {
    if (A == NULL || A->matrix == NULL) {
        return ;
    }
    for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
    }
    free(A->matrix);
    A->columns = 0;
    A->rows = 0;
    A->matrix = NULL;
}
int s21_eq_matrix(s21_matrix *A, s21_matrix *B) {
    int status = matrix_check(A, B);
    if (status) {
        return status;
    }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6 ) {
                return 0;
            }
        }
    }
    return 1;
}

int s21_sum_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
    int status = matrix_check(A, B);
    if (status) {
        return status;
    }
    if (s21_create_matrix(A->rows, A->columns, result)) {
        return 1;
    }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
    }
    return 0;
}
int s21_sub_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
    int status = matrix_check(A, B);
    if (status) {
        return status;
    }
    if (s21_create_matrix(A->rows, A->columns, result)) {
        return 1;
    }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
    }
    return 0;
}
int s21_mult_number(s21_matrix *A, double number, s21_matrix *result) {
    int status = matrix_check(A, A);
    if (status) {
        return status;
    }
    if (s21_create_matrix(A->rows, A->columns, result)) {
        return 1;
    }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] * number;
        }
    }
    return 0;
}
int s21_mult_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
    int status = check_for_mult(A, B);
    if (status) {
        return status;
    }
    if (s21_create_matrix(A->rows, B->columns, result)) {
        return 1;
    }
    for (int i = 0; i < A->rows; i++) {
        double temp = 0;
        for (int j = 0; j < B->columns; j++) {
            result->matrix = 0;
            for (int k = 0; k < A->columns; k++) {
                result->matrix[i][j] += A->matrix[j][k] * B->matrix[k][j];
            } 
        }
    }
    return 0;
}

int s21_transpose(s21_matrix *A, s21_matrix *result) {
    int status = matrix_check(A, A);
    if (status) {
        return status;
    }
    if (s21_create_matrix(A->columns, A->rows, result)) {
        return 1;
    }
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            result->matrix[j][i] = A->matrix[i][j];
        }
    }
    return 0;
}
int s21_determinant(s21_matrix *A, double *result) {
    if (matrix_check(A, A)) return 1;
    if (A->rows != A->columns || A->rows == 0) return 2;
    
    if (A->rows == 1) {
        *result = A->matrix[0][0];
        return 0;
    }
    if (A->rows == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
        return 0;
    }
    
    s21_matrix copy;
    s21_create_matrix(A->rows, A->columns, &copy);
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            copy.matrix[i][j] = A->matrix[i][j];
        }
    }
    
    double det = 1.0;
    
    for (int k = 0; k < copy.rows - 1; k++) {
        
        int pivot_row = k;
        double max_val = fabs(copy.matrix[k][k]);
        
        for (int i = k + 1; i < copy.rows; i++) {
            if (fabs(copy.matrix[i][k]) > max_val) {
                max_val = fabs(copy.matrix[i][k]);
                pivot_row = i;
            }
        }
        
        if (max_val < 1e-10) {
            s21_remove_matrix(&copy);
            *result = 0.0;
            return 0;
        }
        // replace
        if (pivot_row != k) {
            for (int j = 0; j < copy.columns; j++) {
                double temp = copy.matrix[k][j];
                copy.matrix[k][j] = copy.matrix[pivot_row][j];
                copy.matrix[pivot_row][j] = temp;
            }
            det = -det;  
        }
        
        for (int i = k + 1; i < copy.rows; i++) {
            double factor = copy.matrix[i][k] / copy.matrix[k][k];
            
            for (int j = k; j < copy.columns; j++) {
                copy.matrix[i][j] -= factor * copy.matrix[k][j];
            }
        }
    }
    
    for (int i = 0; i < copy.rows; i++) {
        det *= copy.matrix[i][i];
    }
    
    *result = det;
    s21_remove_matrix(&copy);
    return 0;
}

int s21_calc_complements(s21_matrix *A, s21_matrix *result) {
    if (A == NULL || A->matrix == NULL) return 1;
    if (A->rows <= 0 || A->columns <= 0) return 1;
    if (A->rows != A->columns) return 2;  
    
    if (A->rows == 1) {
        s21_create_matrix(1, 1, result);
        result->matrix[0][0] = 1.0;
        return 0;
    }
    
    int status = s21_create_matrix(A->rows, A->columns, result);
    if (status != 0) return status;
    
    for (int x = 0; x < A->rows; x++) {
        for (int y = 0; y < A->columns; y++) {
            s21_matrix minor;
            status = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
            if (status != 0) {
                s21_remove_matrix(result);
                return status;
            }
            
            int i1 = 0;
            for (int i = 0; i < A->rows; i++) {
                if (i == x) continue;
                int j1 = 0;
                for (int j = 0; j < A->columns; j++) {
                    if (j == y) continue;
                    minor.matrix[i1][j1] = A->matrix[i][j];
                    j1++;
                }
                i1++;
            }
            
            double det_minor;
            status = s21_determinant(&minor, &det_minor);
            s21_remove_matrix(&minor);  
            
            if (status != 0) {
                s21_remove_matrix(result);
                return status;
            }
            
            double sign = ((x + y) % 2 == 0) ? 1.0 : -1.0;
            result->matrix[x][y] = sign * det_minor;
        }
    }
    
    return 0;
}
#include "s21_matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int matrix_check(s21_matrix *A, s21_matrix *B) {
  if (A == NULL || B == NULL) return 1;
  if (A->matrix == NULL || B->matrix == NULL) return 1;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)
    return 1;
  if (A->rows != B->rows || A->columns != B->columns) return 2;
  return 0;
}

int check_for_mult(s21_matrix *A, s21_matrix *B) {
  if (A == NULL || B == NULL) return 1;
  if (A->matrix == NULL || B->matrix == NULL) return 1;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)
    return 1;
  if (A->columns != B->rows) return 2;
  return 0;
}

int s21_create_matrix(int rows, int columns, s21_matrix *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return 1;
  }

  result->matrix = (double **)malloc(rows * sizeof(double *));
  if (result->matrix == NULL) {
    return 1;
  }

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double *)malloc(sizeof(double) * columns);
    if (result->matrix[i] == NULL) {
      // Очистка при ошибке
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      result->matrix = NULL;
      return 1;
    }
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
    return;
  }
  for (int i = 0; i < A->rows; i++) {
    if (A->matrix[i] != NULL) {
      free(A->matrix[i]);
    }
  }
  free(A->matrix);
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(s21_matrix *A, s21_matrix *B) {
  if (A == NULL || B == NULL) return 0;
  if (A->matrix == NULL || B->matrix == NULL) return 0;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)
    return 0;
  if (A->rows != B->rows || A->columns != B->columns) return 0;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6) {
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
  if (result == NULL) {
    return 1;
  }
  if (s21_create_matrix(A->rows, A->columns, result)) {
    return 1;
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
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
  if (result == NULL) {
    return 1;
  }
  if (s21_create_matrix(A->rows, A->columns, result)) {
    return 1;
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return 0;
}

int s21_mult_number(s21_matrix *A, double number, s21_matrix *result) {
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    return 1;
  }
  if (result == NULL) {
    return 1;
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
  if (result == NULL) {
    return 1;
  }
  if (s21_create_matrix(A->rows, B->columns, result) != 0) {
    return 1;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = 0.0;
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return 0;
}

int s21_transpose(s21_matrix *A, s21_matrix *result) {
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    return 1;
  }
  if (result == NULL) {
    return 1;
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
  if (A == NULL || A->matrix == NULL) return 1;
  if (A->rows <= 0 || A->columns <= 0) return 1;
  if (A->rows != A->columns) return 2;

  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return 0;
  }
  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
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

int s21_inverse_matrix(s21_matrix *A, s21_matrix *result) {
  if (A == NULL || A->matrix == NULL) return 1;
  if (A->rows <= 0 || A->columns <= 0) return 1;
  if (A->rows != A->columns) return 2;

  double det;
  int status = s21_determinant(A, &det);
  if (status != 0) return status;
  if (fabs(det) < 1e-7) return 2;

  s21_matrix complements;
  status = s21_calc_complements(A, &complements);
  if (status != 0) return status;

  s21_matrix transposed;
  status = s21_transpose(&complements, &transposed);
  s21_remove_matrix(&complements);
  if (status != 0) {
    s21_remove_matrix(&transposed);
    return status;
  }

  status = s21_mult_number(&transposed, 1.0 / det, result);
  s21_remove_matrix(&transposed);

  return status;
}
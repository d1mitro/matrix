#include <check.h>
#include <math.h>
#include <stdlib.h>
#include "s21_matrix.h"

void fill_matrix(s21_matrix *A, double values[]) {
  int k = 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = values[k++];
    }
  }
}

int compare_matrices(s21_matrix *A, s21_matrix *B) {
  if (A->rows != B->rows || A->columns != B->columns) return 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6) return 0;
    }
  }
  return 1;
}

START_TEST(test_create_matrix_1) {
  s21_matrix A;
  int status = s21_create_matrix(3, 4, &A);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 4);
  ck_assert_ptr_nonnull(A.matrix);
  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      ck_assert_double_eq_tol(A.matrix[i][j], 0.0, 1e-6);
    }
  }
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_create_matrix_2) {
  s21_matrix A;
  int status = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(A.rows, 1);
  ck_assert_int_eq(A.columns, 1);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_create_matrix_error) {
  s21_matrix A;
  int status = s21_create_matrix(0, 5, &A);
  ck_assert_int_eq(status, 1);
  status = s21_create_matrix(-1, 5, &A);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_remove_matrix_1) {
  s21_matrix A;
  s21_create_matrix(3, 3, &A);
  s21_remove_matrix(&A);
  ck_assert_ptr_null(A.matrix);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_null) {
  s21_remove_matrix(NULL);
  s21_matrix A = {NULL, 0, 0};
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_eq_matrix_1) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  double values2[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  fill_matrix(&B, values2);
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_2) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  double values2[] = {1.0, 2.0, 3.0, 5.0};
  fill_matrix(&A, values1);
  fill_matrix(&B, values2);
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_error) {
  s21_matrix A, B;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 2, &B);
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_1) {
  s21_matrix A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  double values2[] = {5.0, 6.0, 7.0, 8.0};
  fill_matrix(&A, values1);
  fill_matrix(&B, values2);
  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {6.0, 8.0, 10.0, 12.0};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_sum_matrix_error) {
  s21_matrix A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 2, &B);
  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_1) {
  s21_matrix A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  double values1[] = {5.0, 6.0, 7.0, 8.0};
  double values2[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  fill_matrix(&B, values2);
  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {4.0, 4.0, 4.0, 4.0};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_mult_number_1) {
  s21_matrix A, result;
  s21_create_matrix(2, 2, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  int status = s21_mult_number(&A, 2.5, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {2.5, 5.0, 7.5, 10.0};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_mult_number_zero) {
  s21_matrix A, result;
  s21_create_matrix(2, 2, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  int status = s21_mult_number(&A, 0.0, &result);
  ck_assert_int_eq(status, 0);
  for (int i = 0; i < result.rows; i++) {
    for (int j = 0; j < result.columns; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], 0.0, 1e-6);
    }
  }
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_1) {
  s21_matrix A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  double values1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  double values2[] = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
  fill_matrix(&A, values1);
  fill_matrix(&B, values2);
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  double expected[] = {58.0, 64.0, 139.0, 154.0};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_mult_matrix_error) {
  s21_matrix A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &B);
  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_transpose_1) {
  s21_matrix A, result;
  s21_create_matrix(2, 3, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  fill_matrix(&A, values1);
  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  double expected[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
  s21_matrix expected_matrix;
  s21_create_matrix(3, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_determinant_1) {
  s21_matrix A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;
  double det;
  int status = s21_determinant(&A, &det);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det, 5.0, 1e-6);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2) {
  s21_matrix A;
  s21_create_matrix(2, 2, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  double det;
  int status = s21_determinant(&A, &det);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det, -2.0, 1e-6);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3) {
  s21_matrix A;
  s21_create_matrix(3, 3, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  fill_matrix(&A, values1);
  double det;
  int status = s21_determinant(&A, &det);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det, 0.0, 1e-6);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_4) {
  s21_matrix A;
  s21_create_matrix(3, 3, &A);
  double values1[] = {2.0, 5.0, 7.0, 6.0, 3.0, 4.0, 5.0, -2.0, -3.0};
  fill_matrix(&A, values1);
  double det;
  int status = s21_determinant(&A, &det);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det, -1.0, 1e-6);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_error) {
  s21_matrix A;
  s21_create_matrix(2, 3, &A);
  double det;
  int status = s21_determinant(&A, &det);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_1) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);
  double values1[] = {1.0, 2.0, 3.0, 0.0, 4.0, 2.0, 5.0, 2.0, 1.0};
  fill_matrix(&A, values1);
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {0.0, 10.0, -20.0, 4.0, -14.0, 8.0, -8.0, -2.0, 4.0};
  s21_matrix expected_matrix;
  s21_create_matrix(3, 3, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_calc_complements_2) {
  s21_matrix A, result;
  s21_create_matrix(2, 2, &A);
  double values1[] = {1.0, 2.0, 3.0, 4.0};
  fill_matrix(&A, values1);
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {4.0, -3.0, -2.0, 1.0};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
}
END_TEST

START_TEST(test_calc_complements_error) {
  s21_matrix A, result;
  s21_create_matrix(2, 3, &A);
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_1) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);
  double values1[] = {2.0, 5.0, 7.0, 6.0, 3.0, 4.0, 5.0, -2.0, -3.0};
  fill_matrix(&A, values1);
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {1.0, -1.0, 1.0, -38.0, 41.0, -34.0, 27.0, -29.0, 24.0};
  s21_matrix expected_matrix;
  s21_create_matrix(3, 3, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_matrix check;
  s21_mult_matrix(&A, &result, &check);
  s21_matrix identity;
  s21_create_matrix(3, 3, &identity);
  double id[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
  fill_matrix(&identity, id);
  ck_assert_int_eq(compare_matrices(&check, &identity), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
  s21_remove_matrix(&check);
  s21_remove_matrix(&identity);
}
END_TEST

START_TEST(test_inverse_matrix_2) {
  s21_matrix A, result;
  s21_create_matrix(2, 2, &A);
  double values1[] = {4.0, 7.0, 2.0, 6.0};
  fill_matrix(&A, values1);
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  double expected[] = {0.6, -0.7, -0.2, 0.4};
  s21_matrix expected_matrix;
  s21_create_matrix(2, 2, &expected_matrix);
  fill_matrix(&expected_matrix, expected);
  ck_assert_int_eq(compare_matrices(&result, &expected_matrix), 1);
  s21_matrix check;
  s21_mult_matrix(&A, &result, &check);
  s21_matrix identity;
  s21_create_matrix(2, 2, &identity);
  double id[] = {1.0, 0.0, 0.0, 1.0};
  fill_matrix(&identity, id);
  ck_assert_int_eq(compare_matrices(&check, &identity), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected_matrix);
  s21_remove_matrix(&check);
  s21_remove_matrix(&identity);
}
END_TEST

START_TEST(test_inverse_matrix_3) {
  s21_matrix A, result;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.2, 1e-6);
  s21_matrix check;
  s21_mult_matrix(&A, &result, &check);
  s21_matrix identity;
  s21_create_matrix(1, 1, &identity);
  identity.matrix[0][0] = 1.0;
  ck_assert_int_eq(compare_matrices(&check, &identity), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&check);
  s21_remove_matrix(&identity);
}
END_TEST

START_TEST(test_inverse_matrix_error) {
  s21_matrix A, result;
  s21_create_matrix(2, 2, &A);
  double values1[] = {1.0, 2.0, 2.0, 4.0};
  fill_matrix(&A, values1);
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_error_non_square) {
  s21_matrix A, result;
  s21_create_matrix(2, 3, &A);
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_error_null) {
  s21_matrix result;
  int status = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

Suite *s21_matrix_suite(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tc_create = tcase_create("Create");
  tcase_add_test(tc_create, test_create_matrix_1);
  tcase_add_test(tc_create, test_create_matrix_2);
  tcase_add_test(tc_create, test_create_matrix_error);
  tcase_add_test(tc_create, test_remove_matrix_1);
  tcase_add_test(tc_create, test_remove_matrix_null);
  suite_add_tcase(suite, tc_create);
  TCase *tc_eq = tcase_create("Eq");
  tcase_add_test(tc_eq, test_eq_matrix_1);
  tcase_add_test(tc_eq, test_eq_matrix_2);
  tcase_add_test(tc_eq, test_eq_matrix_error);
  suite_add_tcase(suite, tc_eq);
  TCase *tc_sum = tcase_create("Sum");
  tcase_add_test(tc_sum, test_sum_matrix_1);
  tcase_add_test(tc_sum, test_sum_matrix_error);
  tcase_add_test(tc_sum, test_sub_matrix_1);
  suite_add_tcase(suite, tc_sum);
  TCase *tc_mult = tcase_create("Mult");
  tcase_add_test(tc_mult, test_mult_number_1);
  tcase_add_test(tc_mult, test_mult_number_zero);
  tcase_add_test(tc_mult, test_mult_matrix_1);
  tcase_add_test(tc_mult, test_mult_matrix_error);
  suite_add_tcase(suite, tc_mult);
  TCase *tc_transpose = tcase_create("Transpose");
  tcase_add_test(tc_transpose, test_transpose_1);
  suite_add_tcase(suite, tc_transpose);
  TCase *tc_det = tcase_create("Determinant");
  tcase_add_test(tc_det, test_determinant_1);
  tcase_add_test(tc_det, test_determinant_2);
  tcase_add_test(tc_det, test_determinant_3);
  tcase_add_test(tc_det, test_determinant_4);
  tcase_add_test(tc_det, test_determinant_error);
  suite_add_tcase(suite, tc_det);
  TCase *tc_calc = tcase_create("Calc");
  tcase_add_test(tc_calc, test_calc_complements_1);
  tcase_add_test(tc_calc, test_calc_complements_2);
  tcase_add_test(tc_calc, test_calc_complements_error);
  suite_add_tcase(suite, tc_calc);
  TCase *tc_inverse = tcase_create("Inverse");
  tcase_add_test(tc_inverse, test_inverse_matrix_1);
  tcase_add_test(tc_inverse, test_inverse_matrix_2);
  tcase_add_test(tc_inverse, test_inverse_matrix_3);
  tcase_add_test(tc_inverse, test_inverse_matrix_error);
  tcase_add_test(tc_inverse, test_inverse_matrix_error_non_square);
  tcase_add_test(tc_inverse, test_inverse_matrix_error_null);
  suite_add_tcase(suite, tc_inverse);
  return suite;
}

int main(void) {
  int number_failed;
  Suite *suite = s21_matrix_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (number_failed == 0) ? 0 : 1;
}
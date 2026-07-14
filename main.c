#include <stdio.h>
#include <math.h>
#include "s21_matrix.h"

int main() {
    s21_matrix A, B;
    
    // Проверяем, что создание прошло успешно
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    
    // Заполняем матрицы
    A.matrix[0][0] = 1.0;
    A.matrix[0][1] = 2.0;
    A.matrix[1][0] = 3.0;
    A.matrix[1][1] = 4.0;
    
    B.matrix[0][0] = 1.0000000005;
    B.matrix[0][1] = 2.0;
    B.matrix[1][0] = 3.0;
    B.matrix[1][1] = 4.0;
    
    int result = s21_eq_matrix(&A, &B);
    printf("%d\n", result);  // Должно быть 1

    
    B.matrix[0][0] = 1.003002;
    result = s21_eq_matrix(&A, &B);
    printf("%d\n", result);  // Должно быть 0


    // Обязательно освобождаем память!
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    
    return 0;
}
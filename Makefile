CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lm
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

TARGET = s21_matrix.a
TEST_TARGET = tests
TEST_SRC = main.c
SRC_FILE = s21_matrix.c

all: $(TARGET)

$(TARGET): $(SRC_FILE)
	$(CC) $(CFLAGS) -c $(SRC_FILE)
	ar rcs $(TARGET) s21_matrix.o
	rm -f s21_matrix.o

tests: $(TARGET)
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -l:$(TARGET) $(LDFLAGS) -o $(TEST_TARGET)
	./$(TEST_TARGET)

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: $(TARGET)
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -l:$(TARGET) $(LDFLAGS) -lgcov -o $(TEST_TARGET)_gcov
	./$(TEST_TARGET)_gcov
	lcov -t "s21_matrix" -o s21_matrix.info -c -d .
	genhtml -o gcov_report s21_matrix.info
	@echo "Report generated in gcov_report/index.html"
	rm -f s21_matrix.o $(TEST_TARGET)_gcov *.gcda *.gcno *.info

clean:
	rm -f $(TARGET) s21_matrix.o $(TEST_TARGET) $(TEST_TARGET)_gcov
	rm -f *.gcda *.gcno *.gcov *.info
	rm -rf gcov_report

.PHONY: all clean tests gcov_report
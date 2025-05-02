#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

int test_sudoku_grid1[9][9] = { // this example is from the assignment itself and is supposed to be valid.
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

int test_sudoku_grid2[9][9] = { // valid
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

int test_sudoku_grid3[9][9] = { // invalid
    {5, 3, 4, 6, 7, 8, 9, 1, 5},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

int test_sudoku_grid4[9][9] = { // valid
    {8, 2, 7, 1, 5, 4, 3, 9, 6},
    {9, 6, 5, 3, 2, 7, 1, 4, 8},
    {3, 4, 1, 6, 8, 9, 7, 5, 2},
    {5, 9, 3, 4, 6, 8, 2, 7, 1},
    {4, 7, 2, 5, 1, 3, 6, 8, 9},
    {6, 1, 8, 9, 7, 2, 4, 3, 5},
    {7, 8, 6, 2, 3, 5, 9, 1, 4},
    {1, 5, 4, 7, 9, 6, 8, 2, 3},
    {2, 3, 9, 8, 4, 1, 5, 6, 7}
};

int test_sudoku_grid5[9][9] = { // invalid
    {8, 2, 7, 1, 5, 4, 3, 9, 6},
    {9, 6, 5, 3, 2, 7, 1, 4, 8},
    {3, 4, 1, 6, 8, 9, 7, 5, 2},
    {5, 9, 3, 4, 6, 8, 2, 7, 1},
    {4, 7, 2, 5, 1, 3, 6, 8, 9},
    {6, 1, 8, 9, 7, 2, 4, 3, 5},
    {7, 8, 6, 2, 3, 0, 9, 1, 4},
    {1, 5, 4, 7, 9, 6, 8, 2, 3},
    {2, 3, 9, 8, 4, 1, 5, 6, 7}
};

int test_sudoku_grid6[9][9] = { // invalid
    {8, 2, 7, 1, 5, 4, 3, 9, 6},
    {9, 6, 5, 3, 2, 7, 1, 4, 8},
    {3, 4, 1, 6, 8, 9, 7, 5, 2},
    {5, 9, 3, 4, 6, 8, 2, 7, 1},
    {4, 7, 2, 5, 1, 3, 6, 8, 9},
    {6, 1, 8, 9, 7, 2, 4, 3, 5},
    {7, 8, 6, 2, 3, 5, 9, 1, 4},
    {1, 5, 4, 7, 9, 6, 8, 2, 3},
    {2, 3, 9, 8, 4, 1, 5, 6, 9}
};

#define TEST_SUDOKU_GRID test_sudoku_grid1

bool all_rows_valid = true;
bool all_columns_valid = true;
bool box_validity[9] = {true, true, true, true, true, true, true, true, true}; // for each of the 3x3 subgrids

bool is_valid_unit(int unit[9]) {
    bool used[10] = {false}; // 1-9 are valid numbers (index 0 is unused)

    for (int i = 0; i < 9; i++) {

        if (unit[i] < 1 || unit[i] > 9) {
            return false; // invalid number
        }

        if (used[unit[i]]) {
            return false; // duplicate number
        }
        used[unit[i]] = true;
    }
    return true; // all numbers are unique (valid unit)
}

void *check_rows(void *arg) {
    for (int i = 0; i < 9; i++) {
        if (!is_valid_unit(TEST_SUDOKU_GRID[i])) {
            all_rows_valid = false;
            break;
        }
    }
    pthread_exit(NULL);
}

void *check_columns(void *arg) {
    for (int i = 0; i < 9; i++) {
        int column[9];
        for (int j = 0; j < 9; j++) {
            column[j] = TEST_SUDOKU_GRID[j][i];
        }
        if (!is_valid_unit(column)) {
            all_columns_valid = false;
            break;
        }
    }
    pthread_exit(NULL);
}
void *check_one_box(void *arg) {
    int box_index = *(int *)arg;
    int start_row = (box_index / 3) * 3;
    int start_col = (box_index % 3) * 3;
    int box[9];
    int index = 0;

    for (int i = start_row; i < start_row + 3; i++) {
        for (int j = start_col; j < start_col + 3; j++) {
            box[index++] = TEST_SUDOKU_GRID[i][j];
        }
    }

    if (!is_valid_unit(box)) {
        box_validity[box_index] = false;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t row_thread, column_thread;
    pthread_t box_threads[9];

    int box_indices[9];

    pthread_create(&row_thread, NULL, check_rows, NULL);
    pthread_create(&column_thread, NULL, check_columns, NULL);

    for (int i = 0; i < 9; i++) {
        box_indices[i] = i;
        pthread_create(&box_threads[i], NULL, check_one_box, (void *)&box_indices[i]);
    }

    // wait for all threads to finish
    pthread_join(row_thread, NULL);
    pthread_join(column_thread, NULL);

    for (int i = 0; i < 9; i++) {
        pthread_join(box_threads[i], NULL);
    }

    bool total_box_validity = box_validity[0] && box_validity[1] && box_validity[2] &&
                              box_validity[3] && box_validity[4] && box_validity[5] &&
                              box_validity[6] && box_validity[7] && box_validity[8];

    if (all_rows_valid && all_columns_valid && total_box_validity) {
        printf("The Sudoku grid is valid.\n");

    } else {
        printf("The Sudoku grid is invalid because:\n");

        if (!all_rows_valid) {
            printf(" - Invalid row.\n");
        }

        if (!all_columns_valid) {
            printf(" - Invalid column.\n");
        }

        if (!total_box_validity) {
            printf(" - Invalid box.\n");
        }
    }
    return 0;
}
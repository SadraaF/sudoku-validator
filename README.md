# Multithreaded Sudoku Solution Validator

This C program uses multithreading to efficiently validate a 9x9 Sudoku puzzle solution. The program checks all rows, columns, and 3x3 subgrids concurrently to determine if the puzzle is solved according to the rules of Sudoku. This project was created for an Operating Systems assignment at AUT.

## Key Concepts Demonstrated

-   **Multithreading:** Using the `pthread` library to perform multiple validation tasks in parallel.
-   **Task Parallelism:** The problem is broken down into 11 independent validation tasks, each handled by a separate thread.
-   **Synchronization:** Using `pthread_join` to ensure all validation threads have completed before making a final decision on the solution's validity.

## Technologies Used

-   **Language:** C
-   **Compiler:** GCC
-   **Core Libraries:** `pthread.h`

## How to Compile and Run

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/SadraaF/sudoku-validator.git
    cd sudoku-validator
    ```
2.  **Compile the code:**
    The `-lpthread` flag is required to link the POSIX threads library.
    ```bash
    gcc main.c -o validator -lpthread
    ```
3.  **Run the program:**
    ```bash
    ./validator
    ```
    You can change the Sudoku grid being tested by modifying the `#define TEST_SUDOKU_GRID` macro in `main.c`.

## Validation Logic

The program ensures a Sudoku solution is valid by verifying three conditions simultaneously:
1.  Each row contains the digits 1 through 9 exactly once.
2.  Each column contains the digits 1 through 9 exactly once.
3.  Each of the nine 3x3 subgrids contains the digits 1 through 9 exactly once.

To achieve this, the program launches **11 threads**:
-   **1 thread** is responsible for validating all 9 rows.
-   **1 thread** is responsible for validating all 9 columns.
-   **9 threads** are created, with each thread responsible for validating one of the nine 3x3 subgrids.

The main thread waits for all 11 threads to finish their execution and then aggregates their results to determine the overall validity of the Sudoku board.

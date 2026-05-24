#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define MAX_THREADS 64
#define Block_size 64
#define array_size1 100
#define array_size2 500
#define array_size3 1000

typedef struct {
    int n;
    int thread_id;
    int threads_num;

    /* For Task 2 */
    int start_row;
    int end_row;

    /* For Task 3 */
    int start_column;
    int end_column;

    /* For Task 4*/
    int block_size;
    int block_row;
    int block_column;

}Thread_data;

void fill_matrices_random();
int min_int(int, int);
void base_line();
void row_wise(int, int);
void column_wise(int, int);
void block_tiled(int, int);
void save_baseline();
int verify_result();

double A[array_size3][array_size3];
double B[array_size3][array_size3];
double C[array_size3][array_size3];
double C_ref[array_size3][array_size3];

int main(){
    fill_matrices_random();
    LARGE_INTEGER freq, start, end;
    int threads_num = 16;

    printf("==================================================\n");
    printf(" Matrix Size : %d x %d\n", array_size3, array_size3);
    printf(" Threads     : %d\n", threads_num);
    printf("--------------------------------------------------\n");

    QueryPerformanceFrequency(&freq);

    /* BASELINE */
    QueryPerformanceCounter(&start);
    base_line();
    QueryPerformanceCounter(&end);

    double t1 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("Baseline: %8.5f sec | [Reference Matrix] \n", t1);

	save_baseline();

    /* ROW WISE */
    QueryPerformanceCounter(&start);
    row_wise(array_size3, threads_num);
    QueryPerformanceCounter(&end);

	
    double t2 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    
    if (verify_result()) {
        printf("Row Wise: %8.5f sec | [Output Matches Baseline] \n", t2);
    } else {
        printf("Row Wise: %8.5f sec | [Output Does Not Match Baseline] \n", t2);
    }


    /* COLUMN WISE */
    QueryPerformanceCounter(&start);
    column_wise(array_size3, threads_num);
    QueryPerformanceCounter(&end);

    double t3 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    if (verify_result()) {
        printf("Column Wise: %8.5f sec | [Output Matches Baseline] \n", t3);
    }
    else {
        printf("Column Wise: %8.5f sec | [Output Does Not Match Baseline] \n", t3);
    }

    /* BLOCK TILED */
    QueryPerformanceCounter(&start);
    block_tiled(array_size3, threads_num);
    QueryPerformanceCounter(&end);

    
    double t4 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    if (verify_result()) {
        printf("Block Tiled: %8.5f sec | [Output Matches Baseline] \n", t4);
    }
    else {
        printf("Block Tiled: %8.5f sec | [Output Does Not Match Baseline] \n", t4);
    }

    printf("==================================================\n");

    return 0;
}

void fill_matrices_random()
{
    srand(42);

    for (int i = 0; i < array_size3; i++) {
        for (int j = 0; j < array_size3; j++) {
            A[i][j] = (double)(rand() % 100) / 10.0;
            B[i][j] = (double)(rand() % 100) / 10.0;
        }
    }
}

int min_int(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

void base_line() {

    for (int i = 0;i < array_size3;i++) {
        for (int j = 0;j < array_size3;j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < array_size3;k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//Save the results of C in C_ref 
void save_baseline() {
    for (int i = 0; i < array_size3; i++) {
        for (int j = 0; j < array_size3; j++) {
            C_ref[i][j] = C[i][j];
        }
    }
}

// Returns 1 if valid, 0 if invalid
int verify_result() {
    double tolerance = 1e-6;
    for (int i = 0; i < array_size3; i++) {
        for (int j = 0; j < array_size3; j++) {
            if (fabs(C_ref[i][j] - C[i][j]) > tolerance) {
                return 0; // Failed
        }
        }
    }
    return 1; // Passed
}


DWORD WINAPI row_worker(LPVOID general) {
    Thread_data* data = (Thread_data*)general;
    for (int i = data->start_row;i < data->end_row;i++) {
        for (int j = 0;j < data->n;j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < data->n;k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return 0;
}

DWORD WINAPI column_worker(LPVOID general) {
    Thread_data* data = (Thread_data*)general;
    for (int i = 0; i < data->n; i++) {
        for (int j = data->start_column; j < data->end_column; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < data->n;k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return 0;
}

void row_wise(int n, int threads_num) {
    HANDLE threads_ref[MAX_THREADS];
    DWORD threads_id[MAX_THREADS];
    Thread_data thread[MAX_THREADS];
    if (threads_num > MAX_THREADS) {
        printf("The number of threads is greater than the maximum limit !\n");
        return;
    }
    for (int i = 0; i < threads_num; i++) {
        thread[i].n = n;
        thread[i].start_row = (i * n) / threads_num;
        thread[i].end_row = ((i + 1) * n) / threads_num;

        threads_ref[i] = CreateThread(NULL, 0, row_worker, &thread[i], 0, &threads_id[i]);
    }

    WaitForMultipleObjects(threads_num, threads_ref, TRUE, INFINITE);

    for (int i = 0; i < threads_num; i++) {
        CloseHandle(threads_ref[i]);
    }
}

void column_wise(int n, int threads_num) {
    HANDLE threads_ref[MAX_THREADS];
    DWORD threads_id[MAX_THREADS];
    Thread_data thread[MAX_THREADS];

    if (threads_num > MAX_THREADS) {
        printf("The number of threads is greater than the maximum limit !\n");
        return;
    }

    for (int i = 0; i < threads_num; i++) {
        thread[i].n = n;
        thread[i].start_column = (i * n) / threads_num;
        thread[i].end_column = ((i + 1) * n) / threads_num;

        threads_ref[i] = CreateThread(NULL, 0, column_worker, &thread[i], 0, &threads_id[i]);
    }

    WaitForMultipleObjects(threads_num, threads_ref, 1, INFINITE);
    for (int i = 0; i < threads_num; i++) {
        CloseHandle(threads_ref[i]);
    }
}

DWORD WINAPI block_worker(LPVOID general)
{
    Thread_data* data = (Thread_data*)general;

    int n = data->n;
    int block_size = data->block_size;
    int thread_id = data->thread_id;
    int threads_num = data->threads_num;

    int blocks_per_row = (n + block_size - 1) / block_size;
    int total_blocks = blocks_per_row * blocks_per_row;

    for (int block_index = 0; block_index < total_blocks; block_index++) {

        if (block_index % threads_num != thread_id) {
            continue;
        }

        int block_row = block_index / blocks_per_row;
        int block_column = block_index % blocks_per_row;

        int start_row = block_row * block_size;
        int end_row = min_int(start_row + block_size, n);

        int start_column = block_column * block_size;
        int end_column = min_int(start_column + block_size, n);

        for (int i = start_row; i < end_row; i++) {
            for (int j = start_column; j < end_column; j++) {
                C[i][j] = 0.0;

                for (int k = 0; k < n; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    return 0;
}

void block_tiled(int n, int threads_num)
{
    HANDLE threads_ref[MAX_THREADS];
    DWORD threads_id[MAX_THREADS];
    Thread_data thread[MAX_THREADS];

    if (threads_num > MAX_THREADS) {
        printf("The number of threads is greater than the maximum limit!\n");
        return;
    }

    for (int i = 0; i < threads_num; i++) {
        thread[i].n = n;
        thread[i].block_size = Block_size;
        thread[i].thread_id = i;
        thread[i].threads_num = threads_num;

        threads_ref[i] = CreateThread(NULL, 0, block_worker, &thread[i], 0, &threads_id[i]);
    }

    WaitForMultipleObjects(threads_num, threads_ref, 1, INFINITE);

    for (int i = 0; i < threads_num; i++) {
        CloseHandle(threads_ref[i]);
    }
}
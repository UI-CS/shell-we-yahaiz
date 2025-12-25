#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int row;
    int column;
} parameters;

int valid[11] = {0};

int sudoku[9][9] = {
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

void *validate_rows(void *param) {
    for (int i = 0; i < 9; i++) {
        int visited[10] = {0};
        for (int j = 0; j < 9; j++) {
            int val = sudoku[i][j];
            if (val < 1 || val > 9 || visited[val]) {
                pthread_exit(NULL);
            }
            visited[val] = 1;
        }
    }
    valid[0] = 1;
    pthread_exit(NULL);
}

void *validate_cols(void *param) {
    for (int j = 0; j < 9; j++) {
        int visited[10] = {0};
        for (int i = 0; i < 9; i++) {
            int val = sudoku[i][j];
            if (val < 1 || val > 9 || visited[val]) {
                pthread_exit(NULL);
            }
            visited[val] = 1;
        }
    }
    valid[1] = 1;
    pthread_exit(NULL);
}

void *validate_subgrid(void *param) {
    parameters *data = (parameters *) param;
    int startRow = data->row;
    int startCol = data->column;
    
    int grid_index = 2 + (startRow / 3) * 3 + (startCol / 3);

    int visited[10] = {0};
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int val = sudoku[i][j];
            if (val < 1 || val > 9 || visited[val]) {
                pthread_exit(NULL);
            }
            visited[val] = 1;
        }
    }
    valid[grid_index] = 1;
    free(param);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[11];
    
    pthread_create(&threads[0], NULL, validate_rows, NULL);
    
    pthread_create(&threads[1], NULL, validate_cols, NULL);
    
    int thread_idx = 2;
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            parameters *data = (parameters *) malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            pthread_create(&threads[thread_idx++], NULL, validate_subgrid, data);
        }
    }

    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    int is_valid = 1;
    for (int i = 0; i < 11; i++) {
        if (valid[i] == 0) {
            is_valid = 0;
            switch(i) {
                case 0: printf("Error in Rows check.\n"); break;
                case 1: printf("Error in Columns check.\n"); break;
                default: printf("Error in Subgrid %d check.\n", i-1);
            }
        }
    }

    if (is_valid) {
        printf("Sudoku solution is VALID.\n");
    } else {
        printf("Sudoku solution is INVALID.\n");
    }

    return 0;
}
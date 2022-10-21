#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <strings.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Введите все параметры при запуске программы [размер] [имя файла]");
        exit(0);
    }

    FILE *f;
    f = fopen(argv[2],"r");
    if(!f) {
        printf("ERROR: Error open %s in mode %s \n", argv[2], "r");
        exit(0);
    }
    printf("INFO: File %s in mode %s successfully opened\n", argv[2], "r");

    int resInRows = (atoi(argv[1]) * 1026) / sqrt(3*atoi(argv[1]));

    int** matrix;
    int* arr;
    int** res_matrix;
    int flag = 0;

    arr = (int*) malloc(resInRows * sizeof(int));

    matrix = (int**) malloc(resInRows * sizeof(int*));
    for (int j = 0; j < resInRows; j++) {
        matrix[j] = (int*) malloc(resInRows * sizeof(int));
    }

    res_matrix = (int**) malloc(resInRows * sizeof(int*));
    for (int j = 0; j < resInRows; j++) {
        res_matrix[j] = (int*) malloc(resInRows * sizeof(int));
    }

    int index_row = 0;
    int index_col = 0;

    char data[2];

    char *val = malloc(80);

    while(fgets(data, 2, f) != NULL) {

        if (flag == 0) {

            if (strcmp(data, " ") == 0) {
                matrix[index_col][index_row] = atoi(val);
                memset(val, 0, 80);
                index_row++;
                continue;
            }

            if (strcmp(data, "\n") == 0) {

                matrix[index_col][index_row] = atoi(val);
                if (index_row == 0) {
                    flag = 1;
                } else {
                    index_row = 0;
                    index_col++;
                }
                memset(val, 0, 80);
                continue;
            }

            if (strcmp(data, "\t") == 0) {
                continue;
            }
            sprintf(val, "%s%s", val, data);
            continue;
        }

        if (strcmp(data, " ") == 0) {
            arr[index_row] = atoi(val);
            memset(val, 0, 80);
            index_row++;
            continue;
        }

        if (strcmp(data, "\n") == 0) {
            arr[index_row] = atoi(val);
            continue;
        }

        if (strcmp(data, "\t") == 0) {
            continue;
        }

        sprintf(val, "%s%s", val, data);

        bzero(data, 1);
    }

    fclose(f);
    f = fopen("res.txt","w+b");

    float res_time = 0;
    time_t start = clock();
    int data_counter = 0;

    int i;
    int j;

#pragma omp parallel for num_threads(2) shared(matrix, arr, res_matrix) private (i, j) reduction(+: data_counter) schedule(static)
    for (i = 0; i < resInRows; i++) {
        for (j = 0; j < resInRows; j++) {
            res_matrix[i][j] = matrix[i][j] * arr[j];
            data_counter+=2;
        }
    }

    time_t end = clock();
    res_time = ((float)(end - start) / 1000000.0F ) * 1000;


    for (int i = 0; i < resInRows;i++) {
        for (int j = 0; j < resInRows - 1; j++) {
            fprintf(f, "%d ", res_matrix[i][j]);
        }
        // array
        fprintf(f, "%d", res_matrix[i][resInRows - 1]);
        fprintf(f, "\n");
    }
      printf("Count time: %f ms\n", res_time);
      printf("Count of digits: %d ms\n", data_counter);  
    fprintf(f, "Count time: %f ms\n", res_time);
    fprintf(f, "Count of digits: %d ms\n", data_counter);
    fclose(f);
}

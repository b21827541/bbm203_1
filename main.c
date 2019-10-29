#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

struct my_file{
    char *filename;
    char *fileformat;
    char *filecontent;
    int index;
    struct my_file *next;
}; typedef struct my_file NODE;

struct Matrix{
    int rows;
    int cols;
    double* data;
};typedef struct Matrix Matrix;

Matrix* new_matrix(int n_rows, int n_cols){
    struct Matrix* m = malloc(sizeof(Matrix));
    m -> rows = n_rows;
    m -> cols = n_cols;
    m -> data = malloc(sizeof(double) * n_rows * n_cols); // stackoverflow da baska bisey daha var olmazsa bak

    return m;
}

Matrix* duplicate_matrix(double* data, int n_rows, int n_cols){
    struct Matrix *m = new_matrix(n_rows, n_cols);
    for (int i = 0; i < n_rows*n_cols; i++) {
        m -> data[i] = data[i];
    }
    return m;
}

void print_matrix(Matrix* m, char **argv){
    FILE *foutp = fopen(argv[3], "a");
    for(int x = 0; x < m -> rows; x++){
        fprintf(foutp, "\n");
        for(int y = 0; y < m -> cols; y++){
            int out = (int) m -> data[x * ( m -> cols) + y ];
            putw(out, foutp);
        }
    }
    fclose(foutp);
}

int main(int argc, char **argv) {

    NODE *head = malloc(sizeof(NODE));
    head = NULL;

    FILE *arrayp = fopen(argv[1], "r");

    FILE *finp = fopen(argv[2], "r");
    size_t size_in = 0;
    size_t len = 0;
    ssize_t read;
    char *line = NULL;
    while((read = getline(&line, &len, finp)) != -1){
        size_in += read;
    }
    size_in *= sizeof(char *);
    fclose(finp);

    finp = fopen(argv[2], "r");
    if(finp == NULL){
        printf("error"); //NOT SURE IF WE ARE GOING TO USE THIS OUTPUT
        exit(0);
    }
    FILE *foutp =fopen(argv[3], "w");
    fclose(foutp);
    char *str = malloc(sizeof(char)*size_in);

    while(fgets(str, size_in, finp) != NULL){
        if(strstr(str, "matread")){

        }else if(strstr(str,"matzeros")){

        }else if(strstr(str,"matslice")){

        }else if(strstr(str,"matslicerow")){

        }else if(strstr(str,"matslicecol")){

        }else if(strstr(str,"matstack")){

        }else if(strstr(str, "pad")){

        }else if(strstr(str,"padval")){

        }else if(strstr(str,"vecread")){

        }else if(strstr(str,"vecslice")){

        }else if(strstr(str,"vecstack")){

        }else if(strstr(str,"veczeros")){

        }else if(strstr(str,"add")){

        }else if(strstr(str,"multiply")){

        }else if(strstr(str,"substract")){

        }else if(strstr(str,"dot")){

        }else if(strstr(str,"mvstack")){

        }else{
            foutp = fopen(argv[3], "a");
            fprintf(foutp, "error\n");
            fclose(foutp);
        }
    }

    fclose(finp);
    free(str);
    free(head);
    return 0;
}
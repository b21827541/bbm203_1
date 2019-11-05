#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

struct my_file{
    char *filename;
    char *fileformat;
    char *filecontent;
    int index;
    struct my_file *next;
}; typedef struct my_file NODE;

struct Matrix{
    char * name;
    int rows;
    int cols;
    double** data;
};typedef struct Matrix Matrix;

struct Vector{
    int rows;
    int cols;
    double** data;
};typedef struct Vector vector;

Matrix* new_matrix(char* n_name,int n_rows, int n_cols){
    struct Matrix* m = malloc(sizeof(Matrix));
    m -> name = n_name;
    m -> rows = n_rows;
    m -> cols = n_cols;
    //m -> data = malloc(sizeof(double) * n_rows * n_cols); // stackoverflow da baska bisey daha var olmazsa bak
    double** data = malloc(sizeof(double*) * n_rows);
    for(int x = 0; x < n_rows; x++){
        data[x] = calloc(n_cols, sizeof(double));
    }
    m -> data = data;
    return m;
}

void print_matrix(Matrix* m, char* fileName) { // there was FILE** readFıle
    FILE *foutp = fopen(fileName, "a");
    bool flag = false;

    for (int x = 0; x < m->rows; x++) {
        if(flag)
            fprintf(foutp,"\n");
        for (int y = 0; y < m->cols; y++) {
            int out = (int) m->data[x][y];
            //int out = (int) m->data[x * (m->cols) + y];
            fprintf(foutp,"%d ", out);
            flag = true;
            //putw(out, foutp);
        }
    }
    fprintf(foutp,"\n");
    fclose(foutp);
}
Matrix* duplicate_matrix(char * name,double* data, int n_rows, int n_cols){ //i might need to change this part (stackoverflow)
    struct Matrix *m = new_matrix(name, n_rows, n_cols);
    for(int y = 0; y < n_rows; y++) {
        for(int x = 0; x < n_cols; x++) {
            m->data[y][x] = data[n_cols*y+x];
        }
    }
    /*for (int i = 0; i < n_rows*n_cols; i++) {
        m -> data[i] = data[i];
    }*/
    return m;
}
/*void print_matrixe(Matrix* m) {
    for(int x = 0; x < m->rows; x++) {
        printf("%s", "\n");
        for(int y = 0; y < m->cols; y++) {                  //  THIS WORKS
            printf("%f ", m->data[x][y]);
        }
    }
}*/

int main(int argc, char **argv) {
    NODE *head = malloc(sizeof(NODE));
    head = NULL;

    //FILE *arrayp = fopen(argv[1], "r");
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

    // matrix size
    // first part is just to make an array to hold them
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(argv[1]); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    size_t size_of_name;
    //int no_of_vector = 0;
    //int no_of_matrix = 0;
    dirp = opendir(argv[1]);

    while ((entry = readdir(dirp)) != NULL) {
        if((strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0 || (*entry->d_name) == '.' ))
        {
        }
        else
        {
            printf ("[%s]\n", entry->d_name);
            size_of_name = strlen(entry -> d_name);


            if(strstr(entry -> d_name, ".vec")){
                continue;
            }
            if(strstr(entry->d_name, ".mat")) {
                char *name = malloc(size_of_name * sizeof(char));
                int find = '.';
                int value;
                int c = 0;
                char *ptr;
                name = entry->d_name;
                ptr = strchr(name, find);
                char *m_name = malloc(strlen(name) - strlen(ptr) + sizeof(char));
                while (c <
                       strlen(name) - strlen(ptr)) {                              //just the vector or matrix name part
                    *(m_name + c) = *(name + c);                                        // not sure if we need it tho
                    c++;
                }
                printf("%s\n", m_name);

                char *directory = (char *) malloc(strlen(argv[1]) + sizeof(char));

                strcpy(directory, argv[1]);
                directory = strcat(directory, "/");
                size_t new_mem_size = strlen(argv[1]) + sizeof(char) + strlen(entry->d_name);
                directory = (char *) realloc(directory, new_mem_size);
                directory = strcat(directory, entry->d_name);
                FILE *arrayp = fopen(directory, "r");
                int memory = 0;
                int column = 0;
                int row = 0;
                char ch;
                arrayp = fopen(directory,"r");
                for(ch = getc(arrayp); ch != EOF; ch = getc(arrayp)){
                    memory +=1;
                }
                fclose(arrayp);
                arrayp = fopen(directory,"r");

                char line[memory * sizeof(double)];
                while(fgets(line, memory * sizeof(double), arrayp) != NULL) {
                    row++;
                    for (char *p = strtok(line, " "); p != NULL; p = strtok(NULL, " ")) {
                        column++;
                    }
                }
                fclose(arrayp);
                arrayp = fopen(directory,"r");
                double numbers[column*row];
                int i = 0;
                char string[memory * sizeof(double)];
                while(fgets(string, memory * sizeof(double), arrayp) != NULL) {
                    for (char *pt = strtok(string, " "); pt != NULL; pt = strtok(NULL, " ")) {
                        numbers[i] = atoi(pt);
                        i ++;
                    }
                }
                column /=row ;
                fclose(arrayp);

                printf("%d   %d\n", row, column);

                Matrix* m1 = duplicate_matrix(m_name, numbers, row, column);
                print_matrix(m1,argv[3]);
                /*free(name);
                free(directory);
                free(head);*/
            }

        }
    }
    closedir(dirp);

    //struct Matrix *matrices;
    //matrices = malloc( file_count * sizeof(struct Matrix));

    //this part is to create the matrix
    /*int memory = 0;
    int column = 0;
    int row = 1;
    char c;*/

    /*while((c = fgetc(arrayp)) != EOF){
        if(c == ' '){
            column++;
        }
        if(c == '\n'){
            row++;
        }
    }*/

    /*fclose(arrayp);
    arrayp = fopen(argv[1], "r");
    for(c = getc(arrayp); c != EOF; c = getc(arrayp)){
        memory +=1;
    }
    column /= 2;
    fclose(arrayp);
    char string[memory * sizeof(int)];
    arrayp = fopen(argv[1], "r");
    double numbers[column*row];
    int i = 0;
    while(fgets(string, memory * sizeof(int), arrayp) != NULL) {
        for (char *p = strtok(string, " "); p != NULL; p = strtok(NULL, " ")) {
                numbers[i] = atoi(p);
                i ++;
        }
    }
    Matrix* m1 = duplicate_matrix(numbers, row, column);
    print_matrix(m1,argv[3]);*/














    printf("asd");
    //free(matrices);
    fclose(finp);
    free(str);
    free(head);
}
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

void print_matrix(Matrix* m, FILE **readFile, char* fileName) {
    FILE *foutp = fopen(fileName, "a");
    for (int x = 0; x < m->rows; x++) {
        fprintf(foutp, "\n");
        for (int y = 0; y < m->cols; y++) {
            int out = (int) m->data[x * (m->cols) + y];
            putw(out, foutp);
        }
    }
    fclose(foutp);
}
Matrix* duplicate_matrix(double* data, int n_rows, int n_cols){
    struct Matrix *m = new_matrix(n_rows, n_cols);
    for (int i = 0; i < n_rows*n_cols; i++) {
        m -> data[i] = data[i];
    }
    printf("yarra");
    return m;
}
/*void print_matrix(Matrix* m) {
    for(int x = 0; x < m->rows; x++) {
        for(int y = 0; y < m->cols; y++) {                  //  THIS WORKS
            printf("%f ", m->data[x*(m->cols) + y]);
        }
    }
}*/

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

    // matrix size
    // first part is just to make an array to hold them
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir("arrays"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    Matrix matrices[file_count];

    //this part is to create the matrix
    int memory = 0;
    int column = 0;
    int row = 1;
    char c;
    finp = fopen(argv[1], "r");

    while((c = fgetc(finp)) != EOF){
        if(c == ' '){
            column++;
        }
        if(c == '\n'){
            row++;
        }
    }
    for(c = getc(finp); c != EOF; c = getc(finp)){
        memory +=1;
    }
    column /= 2;
    printf("%d", column);
    printf("%d", row);
    fclose(finp);
    matrices[0] = * new_matrix(row, column); // i dont know how to use struct to make a matrix this is why im doing this
    char string[memory * sizeof(int)];
    arrayp = fopen(argv[1], "r");
    int numbers[column];
    char folder_name[] = "m1"; // this is so wrong but i dont know how to get folder names
    while(fgets(string, memory * sizeof(int), finp) != NULL){// i just want to assign them to array of matrices
        string.Split(' '); // im trying to read the folder and assign the numbers to my matrix
        for(int y = 0; y < row; y++){
            for(int x = 0; x < column; x++){
            }
        }
    }
    //print_matrix(&matrices[0], &foutp, argv[3]);













    printf("asd");

}
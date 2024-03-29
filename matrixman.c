#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

struct Matrix{
    char * name;
    int rows;
    int cols;
    double** data;
};typedef struct Matrix Matrix;


struct Vector{
    char * name;
    int cols;
    int rows;
    double** data;
};typedef struct Vector Vector;

Matrix* new_matrix(char* n_name,int n_rows, int n_cols){
    struct Matrix* m = malloc(sizeof(Matrix));
    m -> name = n_name;
    m -> rows = n_rows;
    m -> cols = n_cols;
    double** data = malloc(sizeof(double*) * n_rows);
    for(int x = 0; x < n_rows; x++){
        data[x] = calloc(n_cols, sizeof(double));
    }
    m -> data = data;
    return m;
}
Vector* new_vector(char* n_name,int n_rows, int n_cols){
    struct Vector* v = malloc(sizeof(Vector));
    v -> name = n_name;
    v-> rows = n_rows;
    v -> cols = n_cols;
    double** data = malloc(sizeof(double*) * n_rows);
    for(int x = 0; x < n_rows; x++){
        data[x] = calloc(n_cols, sizeof(double));
    }
    v -> data = data;
    return v;
}

void print_matrix(Matrix* m, char* fileName) {
    FILE *foutp = fopen(fileName, "a");
    bool flag = false;

    for (int x = 0; x < m->rows; x++) {
        if(flag)
            fprintf(foutp,"\n");
        for (int y = 0; y < m->cols; y++) {
            int out = (int) m->data[x][y];
            fprintf(foutp,"%d ", out);
            flag = true;
        }
    }
    fprintf(foutp,"\n");
    fclose(foutp);
}
void print_vector(Vector* v, char* fileName) {
    FILE *foutp = fopen(fileName, "a");
    bool flag = false;

    for (int x = 0; x < v->rows; x++) {
        if(flag)
            fprintf(foutp,"\n");
        for (int y = 0; y < v->cols; y++) {
            int out = (int) v->data[x][y];
            fprintf(foutp,"%d ", out);
            flag = true;
        }
    }
    fprintf(foutp,"\n");
    fclose(foutp);
}
Matrix* duplicate_matrix(char * name,double* data, int n_rows, int n_cols){
    struct Matrix *m = new_matrix(name, n_rows, n_cols);
    for(int y = 0; y < n_rows; y++) {
        for(int x = 0; x < n_cols; x++) {
            m->data[y][x] = data[n_cols*y+x];
        }
    }
    return m;
}
Vector* duplicate_vector(char * name,double* data, int n_rows, int n_cols){
    struct Vector *v = new_vector(name, n_rows, n_cols);
    for(int y = 0; y < n_rows; y++) {
        for(int x = 0; x < n_cols; x++) {
            v->data[y][x] = data[n_cols*y+x];
        }
    }
    return v;
}


int main(int argc, char **argv) {
    FILE *finp = fopen(argv[2], "r");
    int size_in = 0;
    size_t len = 0;
    ssize_t read;
    int element = 0;
    int element_vector = 0;
    char *line = NULL;
    while((read = getline(&line, &len, finp)) != -1){
        size_in += read;
    }
    size_in *= sizeof(char *);
    fclose(finp);
    finp = fopen(argv[2], "r");
    if(finp == NULL){
        printf("error");
        exit(0);
    }
    FILE *foutp =fopen(argv[3], "w");
    fclose(foutp);
    char *str = malloc(sizeof(char)*size_in);

    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(argv[1]);
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
            file_count++;
        }
    }
    closedir(dirp);
    int matrix_file_count = 0;
    int vector_file_count = 0;
    Matrix** Matrix_Array= malloc(file_count * sizeof(Matrix));
    Vector** Vector_Array= malloc(file_count * sizeof(Vector));
    size_t size_of_name;
    dirp = opendir(argv[1]);
    int flag = 0;
    char *directory = (char *) malloc(strlen(argv[1]) + sizeof(char));
    char** file_names = malloc(file_count* sizeof(char*));
    int file_names_count = 0;
    char *dot;
    dot = malloc(sizeof(char));
    strcpy(dot,".");
    while ((entry = readdir(dirp)) != NULL) {
        if((strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0 || (*entry->d_name) == '.' ))
        {
        }
        else
        {
            printf ("[%s]\n", entry->d_name);
            file_names[file_names_count]=entry->d_name;
            file_names_count++;

            if(strstr(entry -> d_name, ".vec")){
                vector_file_count++;
                char *name = malloc(strlen(entry->d_name) * sizeof(char));
                char *name_tokenized = malloc(strlen(entry->d_name) * sizeof(char));
                int find = '.';
                int value;
                int c = 0;
                char *ptr;
                name = entry->d_name;

                strcpy(name_tokenized, name);

                ptr = strchr(name, find);

                char *v_name = malloc(strlen(name) - strlen(ptr));

                name_tokenized = strtok(name_tokenized,dot);
                v_name = name_tokenized;

                printf("%s\n", v_name);


                strcpy(directory, argv[1]);
                directory = strcat(directory, "/");
                size_t new_mem_size =   sizeof(char) + strlen(entry->d_name);
                directory = (char *) realloc(directory, new_mem_size);
                directory = strcat(directory, entry->d_name);
                FILE *arrayp = fopen(directory, "r");
                int memory = 0;
                int column = 0;
                int row = 0;
                char ch;
                for(ch = getc(arrayp); ch != EOF; ch = getc(arrayp)){
                    memory +=1;
                }
                fclose(arrayp);
                arrayp = fopen(directory,"r");

                char *line = malloc(memory * sizeof(double));
                while(fgets(line, memory * sizeof(double), arrayp) != NULL) {
                    row++;
                    for (char *p = strtok(line, " "); p != NULL; p = strtok(NULL, " ")) {
                        column++;
                    }
                }
                fclose(arrayp);
                arrayp = fopen(directory,"r");
                double *numbers = malloc(column*sizeof(double));
                int i = 0;
                char *string = malloc(memory * sizeof(double));
                while(fgets(string, memory * sizeof(double), arrayp) != NULL) {
                    for (char *pt = strtok(string, " "); pt != NULL; pt = strtok(NULL, " ")) {
                        numbers[i] = atoi(pt);
                        i ++;
                    }
                }
                fclose(arrayp);
                printf("%d\n", column);

                struct Vector* v1 = duplicate_vector(v_name, numbers, row,column);
                Vector_Array[element_vector] = v1;
                element_vector++;
                free(numbers);
                free(string);
                free(v_name);
                free(line);
            }

            if(strstr(entry->d_name, ".mat")) {
                matrix_file_count++;
                char *name = malloc(strlen(entry->d_name) * sizeof(char));
                char *name_tok = malloc(strlen(entry->d_name) * sizeof(char));
                char *name_tokenized = malloc(strlen(entry->d_name) * sizeof(char));
                char** name_tok_ar = malloc(strlen(name) * sizeof(char*));
                int find = '.';
                int value;
                int c = 0;
                char *ptr;
                name = entry->d_name;
                name_tok_ar[0] = NULL;
                strcpy(name_tok, name);

                ptr = strchr(name, find);

                char *m_name = malloc(strlen(name) - strlen(ptr));
                for (int j = 0; j < 2; ++j) {
                    name_tok_ar[j] = strtok(name_tok,dot);
                }

                strcpy(name_tokenized,name_tok_ar[0]);
                m_name = name_tokenized;
                printf("%s\n", m_name);


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
                for(ch = getc(arrayp); ch != EOF; ch = getc(arrayp)){
                    memory +=1;
                }
                fclose(arrayp);
                arrayp = fopen(directory,"r");

                char *line = malloc(memory * sizeof(double));
                while(fgets(line, memory * sizeof(double), arrayp) != NULL) {
                    row++;
                    for (char *p = strtok(line, " "); p != NULL; p = strtok(NULL, " ")) {
                        column++;
                    }
                }
                fclose(arrayp);
                free(line);
                arrayp = fopen(directory,"r");
                double *numbers = malloc(column*row*sizeof(double));
                int i = 0;
                char *string = malloc(memory * sizeof(double));
                while(fgets(string, memory * sizeof(double), arrayp) != NULL) {
                    for (char *pt = strtok(string, " "); pt != NULL; pt = strtok(NULL, " ")) {
                        numbers[i] = atoi(pt);
                        i ++;
                    }
                }
                column /=row ;
                fclose(arrayp);
                free(string);
                printf("%d   %d\n", row, column);

                Matrix* m1 = duplicate_matrix(m_name, numbers, row, column);
                Matrix_Array[element] = m1;
                element ++;
                free(numbers);
                free(m_name);
                free(name_tok);
                free(name_tok_ar);
            }
        }
    }
    FILE* command_filep = fopen(argv[2], "r");
    char ch;
    int mem = 0;
    for(ch = getc(command_filep); ch != EOF; ch = getc(command_filep)){
        mem +=1;
    }
    fclose(command_filep);
    command_filep = fopen(argv[2], "r");
    int i = 0;
    int number_of_elements = 0;
    char *string =(char *) malloc(mem * sizeof(char));
    char *stringcpy =(char *) malloc(mem * sizeof(char));
    char** str_arr = malloc(number_of_elements* sizeof(char *));


    while(fgets(string, mem * sizeof(char), command_filep) != NULL) {
        strcpy(stringcpy,string);
        printf("%s\n", string);
        for (char *pt = strtok(string, " "); pt != NULL; pt = strtok(NULL, " ")) {
            //strcpy(&str_arr[i],pt);
            //printf("%s\n", &str_arr[i]);
            //i += strlen(&str_arr[i]);
            number_of_elements ++;
        }
        str_arr = realloc(str_arr, number_of_elements * sizeof(char*));
        for (char *pt = strtok(stringcpy, " "); pt != NULL; pt = strtok(NULL, " ")) {
            str_arr[i] = malloc(strlen(pt)* sizeof(char*));
            strcpy(str_arr[i], pt);
            printf("%s\n", str_arr[i]);
            i++;
        }
        i = 0;
        //INITIALIZATIONS
        if(strcmp(str_arr[i],"matread") == 0){
            char * name = malloc(strlen(str_arr[i+1]));
            bool flag5 = false;
            str_arr[i+1][strlen(str_arr[i+1])-2] = '\0';
            for (int j = 0; j < file_names_count; ++j) {
                if(strcmp(file_names[j],str_arr[i+1])==0){
                    flag5 = true;
                }

            }
            if(!flag5){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "error\n");
                fclose(foutp);
                continue;
            }
            strcpy(name,str_arr[i+1]);
            name = strtok(name,dot);
            for(int matr = 0; matr<matrix_file_count;matr++) {
                if(strcmp(name, Matrix_Array[matr]->name) == 0){
                    foutp =fopen(argv[3], "a");
                    fprintf(foutp,"%s %s %d %d\n","read matrix", str_arr[i+1], Matrix_Array[matr]->rows, Matrix_Array[matr]->cols);
                    fclose(foutp);
                    print_matrix(Matrix_Array[matr], argv[3]);
                }
            }
            free(name);
            continue;
        }
        if(strcmp(str_arr[i],"vecread") == 0){
            char * name = malloc(strlen(str_arr[i+1]));

            str_arr[i+1][strlen(str_arr[i+1])-2] = '\0';
            bool flag5 = false;
            for (int j = 0; j < file_names_count; ++j) {
                if(strcmp(file_names[j],str_arr[i+1])==0){
                    flag5 = true;
                }

            }
            if(!flag5){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "error\n");
                fclose(foutp);
                continue;
            }
            strcpy(name,str_arr[i+1]);
            name = strtok(name,dot);
            for(int vec = 0; vec<vector_file_count;vec++) {
                if(strcmp(name, Vector_Array[vec]->name) == 0){
                    foutp =fopen(argv[3], "a");
                    fprintf(foutp,"%s %s %d\n","read vector", str_arr[i+1], Vector_Array[vec]->cols);
                    fclose(foutp);
                    print_vector(Vector_Array[vec], argv[3]);
                }
            }
            free(name);
            continue;
        }
        if(strcmp(str_arr[i],"matzeros") == 0){
            str_arr[i+3][strlen(str_arr[i+3])-2] = '\0';
            double *numbers = malloc(atoi(str_arr[i+3])*atoi(str_arr[i+2])*sizeof(double));
            for(int k = 0; k < atoi(str_arr[i+2])*atoi(str_arr[i+3]); k++){
                numbers[k] = 0;

            }
            printf("%d\n",(int)numbers[6]);
            Matrix* m = duplicate_matrix(str_arr[i+1],numbers,atoi(str_arr[i+2]),atoi(str_arr[i+3]));
            matrix_file_count++;
            Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
            Matrix_Array[matrix_file_count-1] = m;
            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d %d\n","created matrix", str_arr[i+1], Matrix_Array[matrix_file_count-1]->rows, Matrix_Array[matrix_file_count-1]->cols);
            fclose(foutp);
            print_matrix(m, argv[3]);
            continue;
        }
        if(strcmp(str_arr[i],"veczeros") == 0){
            str_arr[i+2][strlen(str_arr[i+2])-2] = '\0';
            double *numbers = malloc(atoi(str_arr[i+2])*sizeof(double));
            for(int k = 0; k < atoi(str_arr[i+2]); k++){
                numbers[k] = 0;
            }
            Vector* v = duplicate_vector(str_arr[i+1],numbers,1,atoi(str_arr[i+2]));
            vector_file_count++;
            Vector_Array = realloc(Vector_Array, vector_file_count*sizeof(Vector));
            Vector_Array[vector_file_count-1] = v;
            for(int j = 0; j < vector_file_count; j++){
                printf("%s\n", Vector_Array[j]->name);
            }
            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d\n","created vector", str_arr[i+1], Vector_Array[vector_file_count-1]->cols);
            fclose(foutp);
            print_vector(Vector_Array[vector_file_count-1], argv[3]);
            free(numbers);
            continue;
        }
        //CONCAT
        if(strcmp(str_arr[i],"matstack") ==0){
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;
            str_arr[i+3][strlen(str_arr[i+3])-2] = '\0';
            printf("%s \n", Matrix_Array[matrix_file_count-1]->name);
            for(int j = 0; j < matrix_file_count; j++){


                if(strcmp(str_arr[i+1],Matrix_Array[j]->name)==0){
                    printf("%s \n", Matrix_Array[j]->name);

                    place1 = j;
                    flag1 = true;
                }
                if(strcmp(str_arr[i+2],Matrix_Array[j]->name)==0){
                    printf("%s \n", Matrix_Array[j]->name);

                    place2 = j;
                    flag2 = true;
                }
            }

            if((strcmp(str_arr[i+3],"r") == 0) || !flag1 || !flag2 || (strcmp(str_arr[i+3],"d") == 0)){
                if((strcmp(str_arr[i+3],"r") == 0)) {
                    if (Matrix_Array[place1]->rows != Matrix_Array[place2]->rows) {
                        foutp = fopen(argv[3], "a");
                        fprintf(foutp, "error\n");
                        fclose(foutp);
                        continue;
                    }
                }
                if((strcmp(str_arr[i+3],"d") == 0)){
                    if (Matrix_Array[place1]->cols != Matrix_Array[place2]->cols) {
                        foutp = fopen(argv[3], "a");
                        fprintf(foutp, "error\n");
                        fclose(foutp);
                        continue;
                    }
                }
                int row1 = Matrix_Array[place1]->rows;
                int row2 = Matrix_Array[place2]->rows;
                int col1 = Matrix_Array[place1]->cols;
                int col2 = Matrix_Array[place2]->cols;
                col1 += col2;
                int ar = 0;
                double *numbers = malloc(col1 * row1 * sizeof(double));
                if((strcmp(str_arr[i+3],"r") == 0)) {
                    for (int x = 0; x < Matrix_Array[place1]->rows; x++) {
                        for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                            int out = (int) Matrix_Array[place1]->data[x][y];
                            numbers[x * col1 + y] = out;
                        }
                    }
                    for (int x = 0; x < Matrix_Array[place2]->rows; x++) {
                        for (int y = 0; y < Matrix_Array[place2]->cols; y++) {
                            int out = (int) Matrix_Array[place2]->data[x][y];
                            numbers[Matrix_Array[place1]->cols + x * col1 + y] = out;
                        }
                    }
                }
                if((strcmp(str_arr[i+3],"d") == 0)) {
                    int count = 0;
                    for (int x = 0; x < Matrix_Array[place1]->rows; x++) {
                        for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                            int out = (int) Matrix_Array[place1]->data[x][y];
                            numbers[count] = out;
                            count ++;
                        }
                    }
                    for (int x = 0; x < Matrix_Array[place2]->rows; x++) {
                        for (int y = 0; y < Matrix_Array[place2]->cols; y++) {
                            int out = (int) Matrix_Array[place2]->data[x][y];
                            numbers[count] = out;
                            count++;
                        }
                    }
                }
                Matrix *m;
                char * name = malloc(strlen(Matrix_Array[place1]->name));
                strcpy(name,Matrix_Array[place1]->name);
                if((strcmp(str_arr[i+3],"r") == 0)) {
                    m = duplicate_matrix(name, numbers, row1, col1);
                }
                if((strcmp(str_arr[i+3],"d") == 0)) {
                    m = duplicate_matrix(name, numbers, Matrix_Array[place1]->rows+Matrix_Array[place2]->rows, Matrix_Array[place1]->cols);
                }
                matrix_file_count++;
                Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
                Matrix_Array[matrix_file_count-1] = m;
                Matrix_Array[place1]->name = "-";
                foutp = fopen(argv[3],"a");
                fprintf(foutp, "%s %s %d %d\n", "matrices concatenated", str_arr[i+1], Matrix_Array[matrix_file_count-1]->rows, Matrix_Array[matrix_file_count-1]->cols);
                fclose(foutp);
                Matrix_Array[matrix_file_count ] = m;
                print_matrix(m, argv[3]);
                continue;
            }

        }
        if(strcmp(str_arr[i], "vecstack") == 0){
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;
            str_arr[i+4][strlen(str_arr[i+4])-2] = '\0';
            for(int vec = 0; vec < vector_file_count; vec++){
                if(strcmp(str_arr[i+1],Vector_Array[vec]->name)==0){
                    place1 = vec;
                    flag1 = true;
                }
                if(strcmp(str_arr[i+2],Vector_Array[vec]->name)==0){
                    place2 = vec;
                    flag2 = true;
                }
            }
            printf("%s\n", "asafsfsdf");

            if(Vector_Array[place1]->cols != Vector_Array[place2]->cols){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "error\n");
                fclose(foutp);
                continue;
            }
            int col1 = Vector_Array[place1]->cols;
            int col2 = Vector_Array[place2]->cols;
            col1 += col2;
            int ar = 0;
            double *numbers = malloc(col1*sizeof(double));

            if(!flag1||!flag2||strcmp(str_arr[i+3],"row")==0){
                for (int y = 0; y < Vector_Array[place1]->cols; y++) {
                    int out = (int) Vector_Array[place1]->data[0][y];
                    numbers[ar] = out;
                    ar++;
                }

                for (int y = 0; y < Vector_Array[place2]->cols; y++) {
                    int out = (int) Vector_Array[place2]->data[0][y];
                    numbers[ar + y] = out;
                }
                Matrix* m =duplicate_matrix(str_arr[i+4], numbers, 2, Vector_Array[place1]->cols);
                matrix_file_count++;

                Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
                Matrix_Array[matrix_file_count-1] = m;
                for(int j = 0; j< matrix_file_count;j++){
                    printf("%s\n", Matrix_Array[j]->name);
                }
                printf("%s\n", "asafsfsdf");
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s %s %d %d\n", "vectors concatenated", str_arr[i+4], 2, Vector_Array[place1]->cols);
                fclose(foutp);
                print_matrix(m, argv[3]);
                continue;
            }
            if(!flag1||!flag2||strcmp(str_arr[i+3], "column") == 0){
                for (int y = 0; y < Vector_Array[place1]->cols; y++) {
                    int out = (int) Vector_Array[place1]->data[0][y];
                    numbers[ar] = out;
                    ar++;
                    ar++;
                }
                ar = 1;
                for (int y = 0; y < Vector_Array[place2]->cols; y++) {
                    int out = (int) Vector_Array[place2]->data[0][y];
                    numbers[ar + y * (Vector_Array[place1]->rows + Vector_Array[place2]->rows)] = out;
                }
                Matrix* m =duplicate_matrix(str_arr[i+4], numbers, Vector_Array[place1]->cols, 2);
                matrix_file_count++;

                Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
                Matrix_Array[matrix_file_count-1] = m;
                for(int j = 0; j< matrix_file_count;j++){
                    printf("%s\n", Matrix_Array[j]->name);
                }
                printf("%s\n", "asafsfsdf");
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s %s %d %d\n", "vectors concatenated", str_arr[i+4], Vector_Array[place1]->cols, 2);
                fclose(foutp);
                print_matrix(m, argv[3]);
                continue;
            }
        }
        if(strcmp(str_arr[i],"mvstack") ==0){//----------------------------------------------------------------------------this too
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;
            str_arr[i+3][strlen(str_arr[i+3])-2] = '\0';
            printf("%s \n", Matrix_Array[matrix_file_count-1]->name);
            for(int j = 0; j < matrix_file_count; j++) {
                if (strcmp(str_arr[i + 1], Matrix_Array[j]->name) == 0) {
                    printf("%s \n", Matrix_Array[j]->name);

                    place1 = j;
                    flag1 = true;
                }
            }

            for (int k = 0; k < vector_file_count; ++k) {
                if(strcmp(str_arr[i+2],Vector_Array[k]->name)==0){
                    place2 = k;
                    flag2 = true;
                }
            }
            int ar = 0;

            if((strcmp(str_arr[i+3],"d") == 0) || !flag1 || !flag2){
                if(Matrix_Array[place1]->cols != Vector_Array[place2]->cols){
                    foutp = fopen(argv[3], "a");
                    fprintf(foutp, "error\n");
                    fclose(foutp);
                    continue;
                }
                double * numbers = malloc((Matrix_Array[place1]->rows+1)*Matrix_Array[place1]->cols*sizeof(double));
                for(int x= 0; x<Matrix_Array[place1]->rows;x++) {
                    for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                        int out = (int) Matrix_Array[place1]->data[x][y];
                        numbers[ar] = out;
                        ar++;
                    }
                }
                for (int y = 0; y < Vector_Array[place2]->cols; y++) {
                    int out = (int) Vector_Array[place2]-> data[0][y];
                    numbers[ar] = out;
                    ar++;
                }

                Matrix* m =duplicate_matrix(str_arr[i+1], numbers, (Matrix_Array[place1]->rows+1), Matrix_Array[place1]->cols);
                matrix_file_count++;
                Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));

                Matrix_Array[matrix_file_count-1] = m;
                Matrix_Array[place1]->name = "-";
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s %s %d %d\n", "matrix and vector concatenated", Matrix_Array[matrix_file_count-1]->name, Matrix_Array[matrix_file_count-1]->rows, Matrix_Array[matrix_file_count-1]->cols);
                fclose(foutp);
                printf("%s \n", "asdasdas");
                print_matrix(m, argv[3]);
                free(numbers);
                continue;
            }
            if((strcmp(str_arr[i+3],"r") == 0) || !flag1 || !flag2) {
                if(Matrix_Array[place1]->rows != Vector_Array[place2]->cols){
                    foutp = fopen(argv[3], "a");
                    fprintf(foutp, "error\n");
                    fclose(foutp);
                    continue;
                }
                double * numbers = malloc((Matrix_Array[place1]->rows)*(Matrix_Array[place1]->cols+1)*sizeof(double));
                //double * numbers_mat = malloc((Matrix_Array[place1]->rows)*Matrix_Array[place1]->cols*sizeof(double));
                //double * numbers_vec = malloc(Vector_Array[place2]->cols*sizeof(double));
                int vec = 0;
                for(int x= 0; x<Matrix_Array[place1]->rows;x++) {
                    for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                        int out = (int) Matrix_Array[place1]->data[x][y];
                        numbers[vec] = out;
                        vec++;
                    }
                    vec++;
                }
                for (int y = 0; y < Vector_Array[place2]->cols; y++) {
                    int out = (int) Vector_Array[place2]-> data[0][y];
                    numbers[Matrix_Array[place1]->cols + y + Matrix_Array[place1]->cols * y] = out;
                }


                Matrix* m =duplicate_matrix(str_arr[i+1], numbers, (Matrix_Array[place1]->rows), (Matrix_Array[place1]->cols+1));
                matrix_file_count++;
                Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));

                Matrix_Array[matrix_file_count-1] = m;
                Matrix_Array[place1]->name = "-";
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s %s %d %d\n", "matrix and vector concatenated", Matrix_Array[matrix_file_count-1]->name, Matrix_Array[matrix_file_count-1]->rows, Matrix_Array[matrix_file_count-1]->cols);
                fclose(foutp);
                printf("%s \n", "asdasdas");

                print_matrix(m, argv[3]);
                free(numbers);
                continue;
            }
        }


        //PADDING
        if(strcmp(str_arr[i],"pad")==0) {
            int place1 = 0;
            bool flag1 = false;
            str_arr[i + 4][strlen(str_arr[i + 4]) - 2] = '\0';
            for (int matr = 0; matr < matrix_file_count; matr++) {
                if (strcmp(str_arr[i + 1], Matrix_Array[matr]->name) == 0) {
                    place1 = matr;
                    flag1 = true;
                }
            }
            if (!flag1) {
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            if(strcmp(str_arr[i+4],"maximum")== 0 || strcmp(str_arr[i+4],"minimum")== 0) {
                double *numbers = malloc((Matrix_Array[place1]->cols + atoi(str_arr[i + 3])) *
                                         (Matrix_Array[place1]->rows + atoi(str_arr[i + 2])) * sizeof(double));
                double *numbers_old = malloc(Matrix_Array[place1]->cols * Matrix_Array[place1]->rows * sizeof(double));
                double *numbers_all = malloc(Matrix_Array[place1]->cols * Matrix_Array[place1]->rows * sizeof(double));
                int loc = 0;
                int big = 0;
                int big_col = 0;
                int big_row = 0;
                double *biggest = malloc(Matrix_Array[place1]->rows * sizeof(double));
                double *biggest_col = malloc((Matrix_Array[place1]->cols+atoi(str_arr[i + 3])) * sizeof(double));
                double *all_cols = malloc(Matrix_Array[place1]->rows * sizeof(double));
                bool flag2 = false;
                for (int x = 0; x < Matrix_Array[place1]->rows; x++) {  //rowların sonunu tamamlamak icin
                    loc = 0;
                    for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                        int out = (int) Matrix_Array[place1]->data[x][y];
                        numbers_old[loc] = out;
                        loc++;
                        flag2 = true;
                        biggest[big] = numbers_old[0];

                    }
                    if (flag2 && strcmp(str_arr[i+4],"maximum")== 0) {
                        for (int f = 0; f < Matrix_Array[place1]->cols; f++) {
                            if (biggest[big] <= numbers_old[f]) {
                                biggest[big] = numbers_old[f];
                            }
                        }
                        printf("%d ",(int) biggest[big]);
                        big++;
                    }
                    if(flag2 && strcmp(str_arr[i+4],"minimum")== 0){
                        for (int f = 0; f < Matrix_Array[place1]->cols; f++) {
                            if (biggest[big] >= numbers_old[f]) {
                                biggest[big] = numbers_old[f];
                            }
                        }
                        big++;
                    }
                }
                for (int x = 0; x < Matrix_Array[place1]->rows; x++) {   // columların sonunu tamamlamak icin
                    for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                        int out = (int) Matrix_Array[place1]->data[x][y];
                        numbers_all[x * Matrix_Array[place1]->cols + y] = out;
                    }
                }
                bool flag3 = false;
                printf("\n%s \n",":((((((");
                for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                    for (int x = 0; x < Matrix_Array[place1]->rows; x++) {
                        int out = (int) numbers_all[x * Matrix_Array[place1]->cols + y];
                        printf("%d\n", out);
                        all_cols[x] = out;
                        flag3 = true;
                        biggest_col[big_row] = all_cols[0];
                    }
                    //flag
                    if (strcmp(str_arr[i+4],"maximum")== 0) {

                        for (int j = 0; j < Matrix_Array[place1]->rows; ++j) {
                            if (biggest_col[big_row] <= all_cols[j]) {
                                biggest_col[big_row] = all_cols[j];
                            }
                        }
                        printf("\n%d \n",(int) biggest_col[big_row]);
                        big_row++;
                    }

                    if (flag3 && (strcmp(str_arr[i+4],"minimum")== 0)) {
                        for (int j = 0; j < Matrix_Array[place1]->rows; ++j) {
                            if (biggest_col[big_row] >= all_cols[j]) {
                                biggest_col[big_row] = all_cols[j];
                            }
                        }
                        printf("%d \n",(int) biggest_col[big_row]);
                        big_row++;
                    }
                }
                int big_or_small = 0;
                if (strcmp(str_arr[i+4],"maximum")== 0) {
                    for (int j = 0; j < Matrix_Array[place1]->rows; ++j) {
                        if (biggest_col[big_row] <= biggest[j]) {
                            biggest_col[big_row] = biggest[j];
                            big_or_small=(int)biggest[j];
                        }
                    }
                    big_row++;
                    while(big_row<(atoi(str_arr[i+3])+Matrix_Array[place1]->rows)){
                        biggest_col[big_row]= big_or_small;
                        big_row++;
                    }
                }
                int x = 0;
                int y = 0;
                int x1 = 0;
                int x2 = 0;
                int a = 0;
                for (int j = 0; j < Matrix_Array[place1]->rows; j++) {
                    while (x1 < Matrix_Array[place1]->cols) {
                        numbers[x] = numbers_all[x2];
                        x++;
                        x1++;
                        x2++;
                    }
                    y = 0;
                    while (x1 < Matrix_Array[place1]->cols + atoi(str_arr[i + 3])) {
                        numbers[x] = biggest[a];
                        x++;
                        x1++;
                        y++;
                    }
                    x1 = 0;
                    a++;
                }
                for (int j = 0; j < atoi(str_arr[i + 2]); j++) {
                    for (int k = 0; k < Matrix_Array[place1]->cols+ atoi(str_arr[i+3]); k++) {
                        numbers[x] = biggest_col[k];
                        x++;
                    }


                }
                Matrix *m = duplicate_matrix(str_arr[i + 1], numbers, Matrix_Array[place1]->rows + atoi(str_arr[i + 2]),
                                             Matrix_Array[place1]->cols + atoi(str_arr[i + 3]));
                matrix_file_count++;

                Matrix_Array = realloc(Matrix_Array, matrix_file_count * sizeof(Matrix));
                Matrix_Array[matrix_file_count - 1] = m;
                Matrix_Array[place1]->name = "-";
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s %s %d %d\n", "matrix paded", str_arr[i + 1],
                        Matrix_Array[place1]->rows + atoi(str_arr[i + 2]),
                        Matrix_Array[place1]->cols + atoi(str_arr[i + 3]));
                fclose(foutp);
                print_matrix(m, argv[3]);
                free(numbers);
                free(numbers_all);
                free(numbers_old);
                free(biggest);
                free(all_cols);
                free(biggest_col);
                continue;
            }
        }
        if(strcmp(str_arr[i], "padval")==0){
            int place1 = 0;
            bool flag1 = false;
            str_arr[i + 4][strlen(str_arr[i + 4]) - 2] = '\0';
            for (int matr = 0; matr < matrix_file_count; matr++) {
                if (strcmp(str_arr[i + 1], Matrix_Array[matr]->name) == 0) {
                    place1 = matr;
                    flag1 = true;
                }
            }
            printf("%s\n","asdasdasdasdasdasddas");

            if (!flag1) {
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            printf("%s\n","asdasdasdasdasdasddas");
            double *numbers = malloc((Matrix_Array[place1]->cols + atoi(str_arr[i + 3])) *
                                     (Matrix_Array[place1]->rows + atoi(str_arr[i + 2])) * sizeof(double));
            double *numbers_old = malloc(Matrix_Array[place1]->cols * Matrix_Array[place1]->rows * sizeof(double));
            for (int x = 0; x < Matrix_Array[place1]->rows; x++) {   // columların sonunu tamamlamak icin
                for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                    int out = (int) Matrix_Array[place1]->data[x][y];
                    numbers_old[x * Matrix_Array[place1]->cols + y] = out;
                }
            }
            int x = 0;
            int y = 0;
            int x1 = 0;
            int x2 = 0;
            int a = 0;
            for (int j = 0; j < Matrix_Array[place1]->rows; j++) {
                while (x1 < Matrix_Array[place1]->cols) {
                    numbers[x] = numbers_old[x2];
                    x++;
                    x1++;
                    x2++;
                }
                y = 0;
                while (x1 < Matrix_Array[place1]->cols + atoi(str_arr[i + 3])) {
                    numbers[x] = atoi(str_arr[i+4]);
                    x++;
                    x1++;
                    y++;
                }
                x1 = 0;
                a++;
            }
            for (int j = 0; j < atoi(str_arr[i + 2]); j++) {
                for (int k = 0; k < Matrix_Array[place1]->cols+atoi(str_arr[i+3]); k++) {
                    numbers[x] = atoi(str_arr[i+4]);
                    x++;
                }
                numbers[x] = atoi(str_arr[i+4]);
                x++;
            }
            Matrix *m = duplicate_matrix(str_arr[i + 1], numbers, Matrix_Array[place1]->rows + atoi(str_arr[i + 2]),
                                         Matrix_Array[place1]->cols + atoi(str_arr[i + 3]));
            matrix_file_count++;

            Matrix_Array = realloc(Matrix_Array, matrix_file_count * sizeof(Matrix));
            Matrix_Array[matrix_file_count - 1] = m;
            Matrix_Array[place1]->name = "-";
            foutp = fopen(argv[3], "a");
            fprintf(foutp, "%s %s %d %d\n", "matrix paded", str_arr[i + 1],
                    Matrix_Array[place1]->rows + atoi(str_arr[i + 2]),
                    Matrix_Array[place1]->cols + atoi(str_arr[i + 3]));
            fclose(foutp);
            print_matrix(m, argv[3]);
            continue;
        }
        //SLICING
        if(strcmp(str_arr[i],"vecslice")==0){
            int place1=0;
            str_arr[i+4][strlen(str_arr[i+4])-2] = '\0';

            bool flag1 = false;
            for (int j = 0; j < vector_file_count; ++j) {
                if(strcmp(str_arr[i+1], Vector_Array[j]->name) == 0) {
                    place1 = j;
                    flag1 = true;
                }
            }
            double *numbers = malloc((atoi(str_arr[i+3])-atoi(str_arr[i+2]))*sizeof(double));
            int start = atoi(str_arr[i+2]);
            int stop = atoi(str_arr[i+3]);
            int k = 0;
            if(Vector_Array[place1]->cols < stop){
                stop = Vector_Array[place1]->cols;
            }
            if(Vector_Array[place1]->cols < start){

            }
            while(start<stop){
                int out = (int) Vector_Array[place1]->data[0][start];
                numbers[k] = out;
                start++;
                k++;

            }
            Vector* v = duplicate_vector(str_arr[i+4],numbers,1,stop-atoi(str_arr[i+2]));
            vector_file_count++;
            Vector_Array = realloc(Vector_Array, vector_file_count*sizeof(Vector));
            Vector_Array[vector_file_count-1] = v;

            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d\n","vector sliced", str_arr[i+4], Vector_Array[vector_file_count-1]->cols);
            fclose(foutp);
            print_vector(Vector_Array[vector_file_count-1], argv[3]);
            free(numbers);
            continue;
        }
        if(strcmp(str_arr[i],"matslice")==0){
            int place1=0;
            str_arr[i+6][strlen(str_arr[i+6])-2] = '\0';
            bool flag1 = false;
            for (int j = 0; j < matrix_file_count; ++j) {
                if(strcmp(str_arr[i+1], Matrix_Array[j]->name) == 0) {
                    place1 = j;
                    flag1 = true;
                }
            }
            if(!flag1){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            int start_col = atoi(str_arr[i+2]);
            int stop_col = atoi(str_arr[i+3]);
            int start_row = atoi(str_arr[i+4]);
            int stop_row = atoi(str_arr[i+5]);
            int l =0;
            printf("%s\n",":))))))))))))))))))))))))))");
            double *numbers = malloc((atoi(str_arr[i+3])-atoi(str_arr[i+2]))*(atoi(str_arr[i+5])-atoi(str_arr[i+4]))*sizeof(double));
            for(int k= 0; k<(stop_row-start_row);k++) {
                for (int j = 0; j < (stop_col - start_col); j++) {
                    int out = (int) Matrix_Array[place1]->data[start_row][start_col];
                    numbers[l] = out;
                    start_col++;
                    stop_col++;
                    l++;
                }
                start_row++;
                stop_row++;
                start_col = atoi(str_arr[i+2]);
                stop_col = atoi(str_arr[i+3]);
            }

            for (int j = 0; j < matrix_file_count; ++j) {
                if(strcmp(str_arr[i+6], Matrix_Array[j]->name) == 0) {
                    Matrix_Array[j]->name = "-";
                }
            }
            Matrix* m = duplicate_matrix(str_arr[i+6],numbers,(stop_row-start_row),(stop_col-start_col));
            matrix_file_count++;
            Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
            Matrix_Array[matrix_file_count-1] = m;

            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d %d\n","matrix sliced", str_arr[i+6], Matrix_Array[matrix_file_count-1]->rows, Matrix_Array[matrix_file_count-1]->cols);
            fclose(foutp);
            print_matrix(Matrix_Array[matrix_file_count-1], argv[3]);
            continue;
        }
        if(strcmp(str_arr[i],"matslicecol")==0){
            int place1=0;
            str_arr[i+5][strlen(str_arr[i+5])-2] = '\0';
            bool flag1 = false;
            printf("%d\n", matrix_file_count);
            for (int j = 0; j < matrix_file_count; ++j) {
                printf("%s\n",Matrix_Array[8]->name);
                if(strcmp(str_arr[i+1], Matrix_Array[j]->name) == 0) {
                    place1 = j;
                    flag1 = true;
                }
            }
            printf("%s\n","asfadsd");

            if(!flag1){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            int column = atoi(str_arr[i + 2]);
            int row_start = atoi(str_arr[i +3]);
            int row_end = atoi(str_arr[i +4]);
            int l = 0;
            double *numbers = malloc((row_end-row_start)*sizeof(double));
            for (int j = 0; j < (row_end - row_start); j++) {
                int out = (int) Matrix_Array[place1]->data[row_start][column];
                numbers[l] = out;
                row_start++;
                row_end++;
                l++;
            }
            Vector* v = duplicate_vector(str_arr[i+5],numbers,1,(row_end-row_start));
            vector_file_count++;
            Vector_Array = realloc(Vector_Array, vector_file_count*sizeof(Vector));
            Vector_Array[vector_file_count-1] = v;

            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d\n","vector sliced", str_arr[i+5],  Vector_Array[vector_file_count-1]->cols);
            fclose(foutp);
            print_vector(Vector_Array[vector_file_count-1], argv[3]);
            continue;
        }
        if(strcmp(str_arr[i],"matslicerow")==0){
            int place1=0;
            str_arr[i+5][strlen(str_arr[i+5])-2] = '\0';
            bool flag1 = false;
            for (int j = 0; j < matrix_file_count; ++j) {
                if(strcmp(str_arr[i+1], Matrix_Array[j]->name) == 0) {
                    place1 = j;
                    flag1 = true;
                }
            }
            if(!flag1){
                foutp = fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            int row = atoi(str_arr[i + 2]);
            int col_start = atoi(str_arr[i +3]);
            int col_end = atoi(str_arr[i +4]);
            int l = 0;
            double *numbers = malloc((col_end-col_start)*sizeof(double));
            for (int j = 0; j < (col_end - col_start); j++) {
                int out = (int) Matrix_Array[place1]->data[row][col_start];
                numbers[l] = out;
                col_start++;
                col_end++;
                l++;
            }
            Vector* v = duplicate_vector(str_arr[i+5],numbers,1,(col_end-col_start));
            vector_file_count++;
            Vector_Array = realloc(Vector_Array, vector_file_count*sizeof(Vector));
            Vector_Array[vector_file_count-1] = v;

            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d\n","vector sliced", str_arr[i+5],  Vector_Array[vector_file_count-1]->cols);
            fclose(foutp);
            print_vector(Vector_Array[vector_file_count-1], argv[3]);
            continue;
        }
        //MATH FUNCTIONS
        if((strcmp(str_arr[i],"subtract") == 0)||(strcmp(str_arr[i],"multiply")==0)||(strcmp(str_arr[i],"add") == 0)){
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;

            str_arr[i+2][strlen(str_arr[i+2])-2] = '\0'; //-----------------------------------------------------dosya sonunda patlıyo adlar
            printf("%s\n", str_arr[i+2]);

            for(int matr = 0; matr<matrix_file_count;matr++) {
                if(strcmp(str_arr[i+1], Matrix_Array[matr]->name) == 0) {
                    place1 = matr;
                    flag1 = true;
                }
                if(strcmp(str_arr[i+2], Matrix_Array[matr]->name) == 0) {
                    flag2 = true;
                    place2 = matr;
                }
            }

            if(!flag1 || !flag2 || Matrix_Array[place1]->rows != Matrix_Array[place2] -> rows){
                foutp =fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            if(!flag1 || !flag2 || Matrix_Array[place1]->cols != Matrix_Array[place2] -> cols) {
                foutp =fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }

            double *nums = malloc(Matrix_Array[place1]->cols*Matrix_Array[place1]->rows* sizeof(double));
            for (int x = 0; x < Matrix_Array[place1]->rows; x++) {
                for (int y = 0; y < Matrix_Array[place1]->cols; y++) {
                    int out = (int) Matrix_Array[place1]->data[x][y];
                    nums[Matrix_Array[place1]->cols * x + y] = out;
                }
            }
            if(strcmp(str_arr[i],"subtract")==0) {
                for (int x = 0; x < Matrix_Array[place2]->rows; x++) {
                    for (int y = 0; y < Matrix_Array[place2]->cols; y++) {
                        int out = (int) Matrix_Array[place2]->data[x][y];
                        nums[Matrix_Array[place2]->cols * x + y] -= out;
                    }
                }
            }
            if(strcmp(str_arr[i],"add")==0){
                for (int x = 0; x < Matrix_Array[place2]->rows; x++) {
                    for (int y = 0; y < Matrix_Array[place2]->cols; y++) {
                        int out = (int) Matrix_Array[place2]->data[x][y];
                        nums[Matrix_Array[place2]->cols * x + y] += out ;
                    }
                }
            }
            if(strcmp(str_arr[i],"multiply")==0){
                for (int x = 0; x < Matrix_Array[place2]->rows; x++) {
                    for (int y = 0; y < Matrix_Array[place2]->cols; y++) {
                        int out = (int) Matrix_Array[place2]->data[x][y];
                        nums[Matrix_Array[place2]->cols * x + y] *= out ;
                    }
                }
            }
            Matrix *m = duplicate_matrix(str_arr[i + 1], nums, Matrix_Array[place1]->rows,
                                         Matrix_Array[place1]->cols);
            matrix_file_count++;

            Matrix_Array = realloc(Matrix_Array, matrix_file_count * sizeof(Matrix));
            Matrix_Array[matrix_file_count - 1] = m;
            Matrix_Array[place1]->name = "-";
            char *m_name = malloc(sizeof(char));
            m_name =".";
            Matrix* subs = duplicate_matrix(m_name, nums, Matrix_Array[place2]->rows, Matrix_Array[place2]->cols);
            foutp = fopen(argv[3],"a");
            if(strcmp(str_arr[i],"subtract")==0) {
                fprintf(foutp, "%s %s %s\n", "subtract", str_arr[i+1], Matrix_Array[place2]->name);
                fclose(foutp);
            }
            if(strcmp(str_arr[i],"add")==0){
                fprintf(foutp, "%s %s %s\n", "add", str_arr[i+1], Matrix_Array[place2]->name);
                fclose(foutp);
            }
            if(strcmp(str_arr[i],"multiply")==0) {
                fprintf(foutp, "%s %s %s\n", "multiply", str_arr[i+1], Matrix_Array[place2]->name);
                fclose(foutp);
            }
            print_matrix(subs, argv[3]);
            continue;
        }
        printf("%d\n", (int)strlen(str_arr[i]));
        if(strlen(str_arr[i])==2){

            continue;
        }else if(strlen(str_arr[i]) != 2){
            foutp = fopen(argv[3],"a");
            fprintf(foutp, "%s\n", "error");
            fclose(foutp);
        }
    }
    for (int j1 = 0; j1 < 7; ++j1) {
        if(str_arr[j1] != NULL){
            free(str_arr[j1]);
        }
    }
    free(str_arr);

    for (int n = 0; n < vector_file_count; ++n) {
        free(Vector_Array[n]);
    }
    free(Vector_Array);
    for (int i1 = 0; i1 < matrix_file_count; ++i1) {
        free(Matrix_Array[i1]);
    }
    free(Matrix_Array);
    free(file_names);

    printf("asd");
    fclose(finp);
    free(directory);
    free(str);
    free(string);
    free(stringcpy);
    free(line);
    free(dot);
    free(entry);
}
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


/*struct Vector{
    int rows;
    int cols;
    double** data;
};typedef struct Vector vector;*/

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
Matrix* duplicate_matrix(char * name,double* data, int n_rows, int n_cols){
    struct Matrix *m = new_matrix(name, n_rows, n_cols);
    for(int y = 0; y < n_rows; y++) {
        for(int x = 0; x < n_cols; x++) {
            m->data[y][x] = data[n_cols*y+x];
        }
    }
    return m;
}

int main(int argc, char **argv) {
    FILE *finp = fopen(argv[2], "r");
    int size_in = 0;
    size_t len = 0;
    ssize_t read;
    int element = 0;
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
    Matrix** Matrix_Array= malloc(file_count * sizeof(Matrix));
    size_t size_of_name;
    dirp = opendir(argv[1]);
    int flag = 0;
    char *directory = (char *) malloc(strlen(argv[1]) + sizeof(char));

    while ((entry = readdir(dirp)) != NULL) {
        if((strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0 || (*entry->d_name) == '.' ))
        {
        }
        else
        {
            printf ("[%s]\n", entry->d_name);


            if(strstr(entry -> d_name, ".vec")){
                continue;
            }
            if(strstr(entry->d_name, ".mat")) {
                matrix_file_count++;
                char *name = malloc(strlen(entry->d_name) * sizeof(char));
                char *name_tokenized = malloc(strlen(entry->d_name) * sizeof(char));
                char* dot = malloc(sizeof(char));
                dot = ".";
                int find = '.';
                int value;
                int c = 0;
                char *ptr;
                name = entry->d_name;

                strcpy(name_tokenized, name);

                ptr = strchr(name, find);

                char *m_name = malloc(strlen(name) - strlen(ptr));
                /*while (c <
                       strlen(name) - strlen(ptr)) {                              //just the vector or matrix name part
                    *(m_name + c) = *(name + c);                                        // not sure if we need it tho
                    c++;
                }*/
                name_tokenized = strtok(name_tokenized,dot);
                m_name = name_tokenized;
                //m_name=strcat(m_name,"\0");

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

                printf("%d   %d\n", row, column);

                Matrix* m1 = duplicate_matrix(m_name, numbers, row, column);
                Matrix_Array[element] = m1;
                element ++;

                /*free(name);
                free(directory);
                free(head);*/
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
    while(fgets(string, mem * sizeof(char), command_filep) != NULL) {
        char** str_arr ;
        strcpy(stringcpy,string);
        printf("%s\n", string);
        for (char *pt = strtok(string, " "); pt != NULL; pt = strtok(NULL, " ")) {
            //strcpy(&str_arr[i],pt);
            //printf("%s\n", &str_arr[i]);
            //i += strlen(&str_arr[i]);
            number_of_elements ++;
        }
        str_arr = malloc(number_of_elements* sizeof(char *));
        for (char *pt = strtok(stringcpy, " "); pt != NULL; pt = strtok(NULL, " ")) {
            str_arr[i] = malloc(strlen(pt)* sizeof(char));
            strcpy(str_arr[i], pt);
            printf("%s\n", str_arr[i]);
            i++;
        }
        i = 0;
        //INITIALIZATIONS
        if(strcmp(str_arr[i],"matread") == 0){
            char * name = malloc(strlen(str_arr[i+1]));

            char *dot = malloc(sizeof(char));
            dot = ".";
            str_arr[i+1][strlen(str_arr[i+1])-2] = '\0';
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
        }
        if(strcmp(str_arr[i],"matzeros") == 0){
            str_arr[i+3][strlen(str_arr[i+3])-2] = '\0';
            double *numbers = malloc(atoi(str_arr[i+3])*atoi(str_arr[i+2])*sizeof(double));
            for(int k = 0; k < atoi(str_arr[i+2])*atoi(str_arr[i+3]); k++){
                numbers[i] = 0;
            }
            Matrix* m = duplicate_matrix(str_arr[i+1],numbers,atoi(str_arr[i+2]),atoi(str_arr[i+3]));
            matrix_file_count++;
            Matrix_Array = realloc(Matrix_Array, matrix_file_count*sizeof(Matrix));
            Matrix_Array[matrix_file_count] = m;
            foutp = fopen(argv[3], "a");
            fprintf(foutp,"%s %s %d %d\n","created matrix", str_arr[i+1], Matrix_Array[matrix_file_count]->rows, Matrix_Array[matrix_file_count]->cols);
            fclose(foutp);
            print_matrix(Matrix_Array[matrix_file_count], argv[3]);

        }
        //CONCAT
        if(strcmp(str_arr[i],"matstack") ==0){
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;
            for(int j = 0; j < matrix_file_count; j++){
                if(strcmp(str_arr[i+1],Matrix_Array[j]->name)==0){
                    place1 = j;
                    flag1 = true;
                }
                if(strcmp(str_arr[i+2],Matrix_Array[j]->name)==0){
                    place2 = j;
                    flag2 = true;
                }
            }
        }
        //MATH FUNCTIONS
        if((strcmp(str_arr[i],"subtract") == 0)||(strcmp(str_arr[i],"multiply")==0)||(strcmp(str_arr[i],"add") == 0)){
            int place1 = 0;
            int place2 = 0;
            bool flag1 = false;
            bool flag2 = false;

            str_arr[i+2][strlen(str_arr[i+2])-2] = '\0';

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

            if(flag1 && flag2 && Matrix_Array[place1]->rows != Matrix_Array[place2] -> rows){
                foutp =fopen(argv[3], "a");
                fprintf(foutp, "%s\n", "error");
                fclose(foutp);
                continue;
            }
            if(flag1 && flag2 && Matrix_Array[place1]->cols != Matrix_Array[place2] -> cols) {
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
            char *m_name = malloc(sizeof(char));
            m_name =".";
            Matrix* subs = duplicate_matrix(m_name, nums, Matrix_Array[place2]->rows, Matrix_Array[place2]->cols);
            foutp = fopen(argv[3],"a");
            if(strcmp(str_arr[i],"subtract")==0) {
                fprintf(foutp, "%s %s %s\n", "substract", Matrix_Array[place1]->name, Matrix_Array[place2]->name);
                fclose(foutp);
            }
            if(strcmp(str_arr[i],"add")==0){
                fprintf(foutp, "%s %s %s\n", "add", Matrix_Array[place1]->name, Matrix_Array[place2]->name);
                fclose(foutp);
            }
            if(strcmp(str_arr[i],"multiply")==0) {
                fprintf(foutp, "%s %s %s\n", "multiply", Matrix_Array[place1]->name, Matrix_Array[place2]->name);
                fclose(foutp);
            }
            print_matrix(subs, argv[3]);

        }


    }



    printf("asd");
    fclose(finp);
    free(directory);
    free(str);
}
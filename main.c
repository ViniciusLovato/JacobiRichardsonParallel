#include <stdio.h>
#include <stdlib.h>
#include <Math.h>


/**
 * Structure that hold all the information about the problem
 * J_ORDER : Matrix Order
 * J_ROW_TEST: Row that will be used to test the result
 * J_ERROR: Error value acceptable
 * J_ITE_MAX: Max number of iterations allowed
 * **Ma: Pointer to the Matrix A
 * *Mb: Pointer to the array B 
 */
typedef struct {
    
    int J_ORDER;
    int J_ROW_TEST;
    double J_ERROR;
    int J_ITE_MAX;
    double **Ma;
    double *Mb;

} Data;

/**
 * Read data from file.
 * file: The pointer to the file that contains the data
 * *data: Pointer that will hold the information gathered
 */
int readFromFile(FILE* file, Data *data);

/**
 * It prints all the metadata, Matrix A, and Array B
 *
 */
void printData(Data data);

/**
 * Free all the memory allocated for the data strucute
 *
 */
void freeData(Data *data);




/**
 * Main function
 *
 */
int main(int argc, char* argv[]){
    
    int i, j;

    // if the user has not passed the file path as argument
    if(argc < 2){
        printf("Invalid number of arguments: ./main matrix.txt\n");
        return 1;
    }

    Data *myData;
    FILE *file;

    // Allocate memory
    myData = (Data*) malloc (sizeof(Data));

    // Open file
    file = fopen(argv[1], "r");

    // Read data from file
    readFromFile(file, myData);

    // print Data for testing
    printData(*myData); 

    // Free allocated memory
    freeData(myData);
    fclose(file);

    return 0;
}

void 


int readFromFile(FILE *file, Data *data){

    int i, j;

    // Reading data about the problem metadata. Matrix order, row used for
    // testing purposes, acceptable error value and max number of iterations
    fscanf(file, "%d%d%lf%d", &data->J_ORDER, &data->J_ROW_TEST, &data->J_ERROR, &data->J_ITE_MAX);

    // Allocating memory for Matrix A
    data->Ma = (double**) malloc(sizeof(double*)*data->J_ORDER);
    for(i = 0; i < data->J_ORDER; i++){
        data->Ma[i] = (double*) malloc(sizeof(double)*data->J_ORDER);
    }

    // Reading Matrix A from file
    for(i = 0; i < data->J_ORDER; i++){
        for(j = 0; j < data->J_ORDER; j++){
            fscanf(file, "%lf", &data->Ma[i][j]);
        }
    }  

    // Allocating memory for B array
    data->Mb = (double*) malloc(sizeof(double)*data->J_ORDER);


    // Reading Array B from file
    for(i = 0; i < data->J_ORDER; i++){
        fscanf(file, "%lf", &data->Mb[i]);
    }
}



void printData(Data data){

    int i, j;

    printf("J_ORDER: %d\nJ_ROW_TEST: %d\n, J_ERROR: %lf\n", data.J_ORDER, data.J_ROW_TEST, data.J_ERROR);

    for(i = 0; i < data.J_ORDER; i++){
        for(j = 0; j < data.J_ORDER; j++){
            printf("%lf ", data.Ma[i][j]);
        }
        printf("\n");
    }
    for(i = 0; i < data.J_ORDER; i++){
        printf("%lf \n", data.Mb[i]);
    }

}

void freeData(Data *data){

    int i, j;

    // Free all the columns 
    for(i = 0; i < data->J_ORDER; i++){
        free(data->Ma[i]); 
    }

    // Free Ma pointer
    free(data->Ma);

    // Free Mb pointer
    free(data->Mb);

    // finally free the structure
    free(data);
}

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>


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
    double *testedRow;
    double testedB;
    double **Ma;
    double *Mb;

} Data;

FILE *outputFile;

double average = 0;
double standardDeviation = 0;
int iterations = 0;
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
 * Preapre the Matrix A and the Array B
 * 
 * When using the Jacobi-Richardson method we have A* = L* + I* + R* where
 * A* is the matrix A by its main diagonal
 * 
 * We have to perform the same calculation for the Array B
 *
 */
void prepareMatrices(Data *data);

/**
 * The iterative method itself
 *
 * It calculates X(k+1) = -(L* + R*)x(k) + b*
 * whlie the error < J_ERROR or number of iterations reached < J_ITE_MAX
 *
 */
void JacobiRichardson(Data *data);

/**
 * Calcuates the aboslute error
 * E = || x_current - x_next || 
 *
 * The algorithm wont stop until E < J_ERROR
 *
 */
double getError(double *x_current, double *x_next, int size);

/**
 * Calculates -(L* + R*)x
 *
 */
void LRx(Data *data, double* x_current, double* lrxresult);

/**
 * Main function
 *
 */
int main(int argc, char* argv[]){
    
    int i, j;

    // if the user has not passed the file path as argument
    if(argc < 3){
        printf("Invalid number of arguments: ./main matrix.txt outputFile.txt\n");
        return 1;
    }

    Data *myData;
    FILE *file;

    // Allocate memory

    // Open file
    outputFile = fopen(argv[2], "w");


       for(i = 0; i < 1; i++){
           // Read data from fileI
           // print Data for testing

           iterations = 0;

           myData = (Data*) malloc (sizeof(Data));
           file = fopen(argv[1], "r");
           readFromFile(file, myData);


           prepareMatrices(myData);
           JacobiRichardson(myData);

           fclose(file);
           freeData(myData);
       }

       fprintf(outputFile, "\nAverage: %lf\n", average);
       printf("Number of Iterations: %d\n", iterations);
       printf("Time Average: %lf\n", average);

       // Free allocated memory
       fclose(outputFile);

       return 0;
}  


void prepareMatrices(Data *data){

    // control variables
    int i, j;
    double currentDiagonal;

    // For each item in the Matrix A ...
    for(i = 0; i < data->J_ORDER; i++){

        currentDiagonal = data->Ma[i][i];

        data->Mb[i] = data->Mb[i] / currentDiagonal;
        for(j = 0; j < data->J_ORDER; j++){
            // We divide the position by the correpondent diagonal value
            //printf("%lf / %lf\n", data->Ma[i][j], currentDiagonal);
            data->Ma[i][j] = data->Ma[i][j] / currentDiagonal;
        }
        // Divide the array B by the respective diagonal value
        data->Ma[i][i] = 0;
    }
}


void JacobiRichardson(Data *data){

    // Control variables
    int i, j;

    // Current x value, initial value is 0 for sake of simplicity
    double* x_current;

    // X(k+1)
    double* x_next;   

    // (L* + R*)x_current
    double* lrx_result;

    // Error variable
    double error = 100;

    struct timespec start, finish;
    double time_spent;

    // Variable to keep track the number of iterations

    // Allocates memory for the x values, 
    // the starting point is 0 so we can use calloc to allocate memory here
    // final awnser will be placed at x_next
    x_current = (double*) calloc(sizeof(double), data->J_ORDER);
    x_next = (double*) calloc(sizeof(double), data->J_ORDER);
    lrx_result = (double*) malloc(sizeof(double) * data->J_ORDER);

    // The calculation is not over until the error is lesser than J_ERROR or
    // we haven't reach the maxium number of iterations allowed

    clock_gettime(CLOCK_MONOTONIC, &start);
    do{

        LRx(data, x_current, lrx_result);
        for(i = 0; i < data->J_ORDER; i++){
            x_next[i] = - lrx_result[i] + data->Mb[i];  
        }

        // perform the error calculus
        error = getError(x_current, x_next, data->J_ORDER);
        iterations++; 

        double* temp;
        temp = x_current;
        x_current = x_next;
        x_next = temp;


        // printf("error %lf > %lf data->J_ERROR\n", error, data->J_ERROR);

    } while (error > data->J_ERROR && iterations < data->J_ITE_MAX);

    // Calculates the value for row J_ROW_TEST
    double row_test_result = 0;

    double result = 0;
    for(i = 0; i < data->J_ORDER; i++){
        result = result + data->testedRow[i]*x_current[i];  
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    time_spent = (finish.tv_sec - start.tv_sec);
    time_spent += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    free(x_current);
    free(x_next);
    free(lrx_result);

    average = average + time_spent;

    fprintf(outputFile, "===========================================\n");
    fprintf(outputFile, "Time Spent %lf\n" , time_spent);
    fprintf(outputFile, "Iterations %d\n", iterations);
    fprintf(outputFile, "RowTest: %d => [%lf] =? [%lf]\n", data->J_ROW_TEST, result, data->testedB);
}



void LRx(Data *data, double* x_current, double* lrxresult){

    int i, j, k;
    double temp_result = 0;


    for(i = 0; i < data->J_ORDER; i++){
        temp_result = 0;
        for(j = 0; j < data->J_ORDER; j++){
            temp_result = temp_result + data->Ma[i][j] *  x_current[j];

        }
        lrxresult[i] = temp_result;
    }
}

double getError(double *x_current, double *x_next, int size){

    double error = 0;
    double errorArray[size];

    int i;

    for(i = 0; i < size; i++){
        errorArray[i] = fabs((x_next[i] - x_current[i])/ x_next[i]);
    }

    double max = errorArray[0];
    for(i = 1; i <size; i++){
        if(errorArray[i] > max)
            max = errorArray[i];
    }

    return max;
}

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

    data->testedRow = (double*) malloc(sizeof(double)*data->J_ORDER);
    for(i = 0; i < data->J_ORDER; i++){
        data->testedRow[i] = data->Ma[data->J_ROW_TEST][i];
    }

    data->testedB = data->Mb[data->J_ROW_TEST];
}



void printData(Data data){

    int i, j;

    printf("\nPrint Data\n");
    printf("=================================\n");

    printf("J_ORDER: %d\nJ_ROW_TEST: %d\nJ_ERROR: %lf\n", data.J_ORDER, data.J_ROW_TEST, data.J_ERROR);

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

    free(data->testedRow);

    // Free Mb pointer
    free(data->Mb);

    // finally free the structure
    free(data);
}

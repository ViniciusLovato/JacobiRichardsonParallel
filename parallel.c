#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Structure that hold all the information relevant information
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
    int J_ITE_MAX;
    double J_ERROR;
    double *testedRow;
    double testedB;
    int numberOfThreads;
    double **Ma;
    double *Mb;

} Data;

/**
 *
 * For the sake of simplicty this variables will be declared as global. Dont
 * worry, I know what I'm doing
 *
 * Threads used to calculate each matrix block
 *
 */
typedef struct {
    double J_ERROR;
    int J_ITE_MAX;
    int J_ORDER;
    int start;
    int end;
    double **Ma;
    double *Mb;
    int tNumber;
    
} pthreadData;


pthread_t *pthreads;

pthreadData *pthreadsData;

// Barrier created to sync all the threads
// This is necessary in order guarantee that all the threads are calculating
// the same value of x (awnser array)
pthread_barrier_t barrier;

pthread_mutex_t lock;

double* errorArray;
double *x_current;
double *x_next;
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
 * Function that shall be passed to each thread
 * 
 * Each thread will be responsible for a part of the matrix. We will divide the
 * matrix in several blocks according to the number of threads required by the
 * user
 *
 * In order to keep the result consistent we have to user a barrier to sync all
 * the threads. This must be done at the end of each iteration, after that all
 * the threads can proceed to do the next iteration
 *
 * int start/end: Range of rows that the threads will be responsible
 * int J_ORDER: Matriz Order
 * double** Ma: Pointer to Matrix A
 * double *Mb: Pointer to array B
 * double *x_current: Pointer to the current x_values (also know as xk)
 * double *x_next: Pointer to the values being calculated by this iteration
 * (also know as xk+1)
 *
 */
void* calculateBlock(void *rawData);

/**
 * Set the workload of each thread according to the matrix order and number of
 * threads available. Threads are global, so it does required any parameters
 *
 */
void prepareThreads(Data* data);

/**
 * Main function
 *
 */
int main(int argc, char* argv[]){

    int i, j;

    // if the user has not passed the file path as argument
    if(argc < 3){
        printf("Invalid number of arguments: ./main matrix.txt THREADS_NUMBER\n");
        return 1;
    }

    Data *myData;
    FILE *file;

    // Allocate memory
    myData = (Data*) malloc (sizeof(Data));

    // Open file
    file = fopen(argv[1], "r");

    // 
    myData->numberOfThreads = atoi(argv[2]);

    // Read data from file
    readFromFile(file, myData);

    // print Data for testing
    prepareMatrices(myData);
    prepareThreads(myData);

    JacobiRichardson(myData);

    // Free allocated memory
    freeData(myData);

    fclose(file);

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

void prepareThreads(Data *data){

    int i;

    // workload assigned to each thread
    int workload;

    errorArray = (double*) malloc(sizeof(double) * data->J_ORDER);
    // Allocate memory for threads
    pthreadsData = (pthreadData*) malloc (sizeof(pthreadData) * data->numberOfThreads);
    pthreads = (pthread_t*) malloc (sizeof(pthread_t) * data->numberOfThreads);

    pthread_mutex_init(&lock, NULL);

    workload = data->J_ORDER / data->numberOfThreads;

    int init = 0;
    for(i = 0; i < data->numberOfThreads; i++){
         pthreadsData[i].J_ORDER = data->J_ORDER;
         pthreadsData[i].J_ERROR = data->J_ERROR;
         pthreadsData[i].J_ITE_MAX = data->J_ITE_MAX;
         pthreadsData[i].start = init;
         pthreadsData[i].end = init + workload;
         pthreadsData[i].Ma = data->Ma;
         pthreadsData[i].Mb = data->Mb;
         pthreadsData[i].tNumber = i;
         init = init + workload;
    }

    // Initialize barrier
    // The, NULL for de default attrs
    // the last parameter is the number of threads that must wait in the
    // barrier before all the threads can proceed further
    pthread_barrier_init(&barrier, NULL, data->numberOfThreads);
}


void JacobiRichardson(Data *data){

    // Control variables
    int i, j;

   
    // Current x value, initial value is 0 for sake of simplicity
    // Allocates memory for the x values, 
    // the starting point is 0 so we can use calloc to allocate memory here
    // final awnser will be placed at x_next
    x_current = (double*) calloc(sizeof(double), data->J_ORDER);
    x_next = (double*) malloc(sizeof(double) * data->J_ORDER);

    // The calculation is not over until the error is lesser than J_ERROR or
    // we haven't reach the maxium number of iterations allowed

    for(i = 0; i < data->numberOfThreads; i++){
        pthread_create(&pthreads[i], NULL, &calculateBlock, &(pthreadsData[i]));
    }

    for(i = 0; i < data->numberOfThreads; i++){
        pthread_join(pthreads[i], NULL);
    }


    // Calculates the value for row J_ROW_TEST
    double row_test_result = 0;

    double result = 0;
    for(i = 0; i < data->J_ORDER; i++){
        result = result + data->testedRow[i]*x_current[i];  
    }
    //printf("Iterations: %d\n", iterations);
    printf("RowTest: %d => [%lf] =? [%lf]\n", data->J_ROW_TEST, result, data->testedB);
}

void* calculateBlock(void* rawData){

    pthreadData* tData = (pthreadData*) rawData;
    int i, j, k; 
    double temp_result = 0;
    double temp;
    int iteration=0;
    double maxError = 100;
    //printf("start: %d\n", tData->start);
    //printf("end: %d\n", tData->end);

    do{

        for(i = tData->start; i < tData->end; i++){
            temp_result = 0;
            for(j = 0; j < tData->J_ORDER; j++){
               temp_result = temp_result + tData->Ma[i][j] * x_current[j];
            }
            x_next[i] = - temp_result + tData->Mb[i];

            errorArray[i] = fabs((x_next[i] - x_current[i])/ x_next[i]);

        }

        pthread_barrier_wait(&barrier);
        for(i = tData->start; i < tData->end; i++){
            temp = x_current[i];
            x_current[i]  = x_next[i];
            x_next[i] = temp;
        }
        /*printf("\n Current: " );
        for(k = 0; k < tData->J_ORDER; k++){
            printf("%lf ", x_next[k]);
        }


        printf("Next: ");
        for(k = 0; k < tData->J_ORDER; k++){
            printf("%lf ", x_current[k]);
        }
        printf("\n");*/


       // printf("\nBarreira");

        // wait all the other thread to proceed to the next iteration
        
        //printf("\nDepois barreira\n");

        pthread_barrier_wait(&barrier);

    
        iteration++;

            maxError = errorArray[0];
            for(i = 0; i < tData->J_ORDER; i++){
                if(errorArray[i] > maxError)
                    maxError = errorArray[i];
            }
        pthread_barrier_wait(&barrier);

    } while (maxError > tData->J_ERROR && iteration < tData->J_ITE_MAX);

    printf("iterations number %d\n", iteration);

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

    // Free Mb pointer
    free(data->Mb);

    // finally free the structure
    free(data);

    pthread_mutex_destroy(&lock);
    pthread_barrier_destroy(&barrier);
}

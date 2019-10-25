#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

// matrices
int **matA;
int **matB;
int **result;
// matrices rows and columns
int rowsA, colsA;
int rowsB, colsB;
// variable to calculate time
clock_t t;

// struct to send parameters to thread
struct parameters{
    int row;
    int col;
};

// methods declarations
void matrix_multiplication_row();
void* multiply_row(void* p);
void matrix_multiplication_element();
void* multiply_element(void* p);
void read_matrix();

int main()
{
    read_matrix();
    t = clock();
    matrix_multiplication_element();
    t = clock();
    matrix_multiplication_row();
    return 0;
}

// method to read matrices from file then initialize them
void read_matrix() {
    freopen("matrix-readme.txt", "r", stdin);
    // read size of first matrix
    cin >> rowsA >> colsA;
    matA = new int*[rowsA];
    // initialize the matrix
    for(int i = 0; i < rowsA; i++){
        matA[i] = new int[colsA];
        for(int j = 0; j < colsA; j++){
            cin >> matA[i][j];
        }
    }
    // read size of second matrix
    cin >> rowsB >> colsB;
    // initialize the matrix
    matB = new int*[rowsB];
    for(int i = 0; i < rowsB; i++){
        matB[i] = new int[colsB];
        for(int j = 0; j < colsB; j++){
            cin >> matB[i][j];
        }
    }

}

// function that calculate the matrix multiplication using thread for each row
void matrix_multiplication_row() {
    //initialize result matrix to zeros
    result = new int*[rowsA];
    for(int i = 0; i < rowsA; i++){
        result[i] = new int[colsB];
        for(int j = 0; j < colsB; j++){
            result[i][j] = 0;
        }
    }

    // create threads to the same number of rows of the array and call mulitply function in each thread
    pthread_t threads[rowsA];
    for (int i = 0; i < rowsA; i++) {
        parameters *params = new parameters();
        params->row = i;
        pthread_create(&threads[i], NULL, multiply_row, (void *) params);
    }

    // wait until all threads finish
    for (int i = 0; i < rowsA; i++){
        pthread_join(threads[i], NULL);
    }

    // calculate time that the calculation take
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    // Displaying the result matrix
    cout << endl
         << "Multiplication of A and B -- ROW---" << endl;
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++)
            cout << result[i][j] << " ";
        cout << endl;
    }
    cout << endl << "time taken: " << time_taken << endl;

}

void* multiply_row(void* p) {
    struct parameters *params = (parameters *) p;
   // cout << endl << "asdadasd: " << params->row << endl;
        for (int j = 0; j < colsB; j++){
            for (int k = 0; k < colsA; k++){
                result[params->row][j] += matA[params->row][k] * matB[k][j];
            }
        }
}


// function that calculate the matrix multiplication using thread for each element
void matrix_multiplication_element() {
    // initialize the resukt array with zeros
    result = new int*[rowsA];
    for(int i = 0; i < rowsA; i++){
        result[i] = new int[colsB];
        for(int j = 0; j < colsB; j++){
            result[i][j] = 0;
        }
    }
    // create thread for each element to calculate its value
    pthread_t threads[rowsA][colsB];
    for (int row_number = 0; row_number < rowsA; row_number++) {
        for(int col_number = 0; col_number < colsB; col_number++) {
            parameters *params = new parameters();
            params->row = row_number;
            params->col = col_number;
            pthread_create(&threads[row_number][col_number], NULL, multiply_element, (void *)params);
        }
    }

    // wait until all threads finish
    for (int i = 0; i < rowsA; i++){
        for(int j = 0; j < colsB; j++){
            pthread_join(threads[i][j], NULL);
        }
    }

    //calculate the time that the calculation take
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    // Displaying the result matrix
    cout << endl << "Multiplication of A and B -- ELEMENT---" << endl;
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++)
            cout << result[i][j] << " ";
        cout << endl;
    }
    cout << endl << "time taken: " << time_taken << endl;

}

void* multiply_element(void* p) {
    struct parameters *params = (parameters *) p;
    for (int k = 0; k < colsA; k++){
        result[params->row][params->col] += matA[params->row][k] * matB[k][params->col];
    }
}

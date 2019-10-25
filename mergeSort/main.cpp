#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int size;
int *arr;

struct parameters{
    int low;
    int high;
};

void read_array();
void* merge_sort(void* arg);
void merge_sort(int low, int high);
void merge(int low, int mid, int high);

int main()
{
    read_array();
    merge_sort(0,size - 1);
    cout << "--- after ---" << endl;
    for(int i = 0; i < size; i++){
            cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}

void read_array() {
    freopen("merge-sort-readme.txt", "r", stdin);
    // read size of array
    cin >> size;
    arr = new int[size];
    // initialize the array
    for(int i = 0; i < size; i++){
            cin >> arr[i];
    }
    // print the array before sort
    cout << "--- before ---" << endl;
    for(int i = 0; i < size; i++){
            cout << arr[i] << " ";
    }
    cout << endl;
}

void merge_sort(int low, int high)
{
    // get mid element of the array
    int mid = low + (high - low) / 2;
    if (low < high) {
        // create 2 threads one for each half
        pthread_t t1, t2;
        // parameters of both halves of the array to be sent in the thread call
        parameters *fh = new parameters();
        fh->low = low;
        fh->high = mid;
        parameters *sh = new parameters();
        sh->low = mid+1;
        sh->high = high;

        // create thread to merge sort the first half
        pthread_create(&t1,NULL,merge_sort,(void *)fh);

        // create thread to merge sort the first half
        pthread_create(&t2,NULL,merge_sort,sh);

        // wait until both threads finish executing
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);

        // merge the halves
        merge(low, mid, high);
    }
}

// same merge sort function but to be accepted in the thread
void* merge_sort(void* p)
{
    // calculating mid point of array
    struct parameters *params = (parameters *) p;
    int low = params->low;
    int high = params->high;
    // get mid element of the array
    int mid = low + (high - low) / 2;
    if (low < high) {
        // create 2 threads one for each half
        pthread_t t1, t2;
        // parameters of both halves of the array to be sent in the thread call
        parameters *fh = new parameters();
        fh->low = low;
        fh->high = mid;
        parameters *sh = new parameters();
        sh->low = mid+1;
        sh->high = high;

        // create thread to merge sort the first half
        pthread_create(&t1,NULL,merge_sort,(void *)fh);

        // create thread to merge sort the first half
        pthread_create(&t2,NULL,merge_sort,sh);

        // wait until both threads finish executing
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);

        // merge the halves
        merge(low, mid, high);
    }
}

void merge(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];
    int n1 = mid - low + 1;
    int n2 = high - mid, i, j;

    for (i = 0; i < n1; i++){
        left[i] = arr[i + low];
    }

    for (i = 0; i < n2; i++) {
        right[i] = arr[i + mid + 1];
    }

    int k = low;
    i = j = 0;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]){
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < n1) {
        arr[k++] = left[i++];
    }

    while (j < n2) {
        arr[k++] = right[j++];
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CYLINDERS 5000
#define REQUESTS 1000


// Function to perform selection sort on an array
int* selectionSort(const int requests[], int size) {
    // Create a new array to store the sorted elements
    int* sortedArr = (int*)malloc(size * sizeof(int));
   

    // Copy the original array to the new array
    for (int i = 0; i < size; i++) {
        sortedArr[i] = requests[i];
    }

    // Perform selection sort
    for (int i = 0; i < size - 1; i++) {
        // Find the minimum element in the unsorted part of the array
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (sortedArr[j] < sortedArr[minIndex]) {
                minIndex = j;
            }
        }

        // Swap the found minimum element with the first element
        int temp = sortedArr[i];
        sortedArr[i] = sortedArr[minIndex];
        sortedArr[minIndex] = temp;
    }

    // Return the sorted array
    return sortedArr;
}


// FCFS algoritm FUNKAR!
int fcfs(int requests[], int initalPosition) {
    int currentHeadPosition = initalPosition;
    int totalHeadMovement = 0;

    for(int i = 0; i < REQUESTS; i++) {
       
        totalHeadMovement += abs(currentHeadPosition - requests[i]);

        currentHeadPosition = requests[i];
    }
    return totalHeadMovement;
}

//SSTF algoritm FUNKAR
int sstf(int requests[], int initalPosition){
    int currentHeadPosition = initalPosition;
    int totalHeadMovement = 0;

    bool check[REQUESTS];

    for(int i = 0; i < REQUESTS; i++){
        check[i] = false;
    }

    for(int i = 0; i < REQUESTS; i++){
        int minDistance = __INT_MAX__;
        int nextValue = -1;

        for(int j = 0; j < REQUESTS; j++){
           if(!check[j]) {
                int distance = abs(currentHeadPosition - requests[j]);
                if(distance < minDistance){
                    minDistance = distance;
                    nextValue = j;
                }
            } 
        }

        //printf("next value: %d, mindistance: %d\n", nextValue, minDistance);
        check[nextValue] = true;
        totalHeadMovement += minDistance;
        currentHeadPosition = requests[nextValue]; 
    }
    return totalHeadMovement;   
}

//Scan funkar!!!
int scan(int requests[], int initialPosition){
    int currentHeadPosition = initialPosition;
    int totalHeadMovement = 0;

    int* sortedRequests = selectionSort(requests, REQUESTS);

    int index = -1;

    //Find starting index
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] > initialPosition){
            index = i;
            break;
        }
    }

    for(int i = index - 1; i >= 0; i--){
        totalHeadMovement += abs(currentHeadPosition - sortedRequests[i]);
        currentHeadPosition = sortedRequests[i];
    }
    totalHeadMovement += abs(currentHeadPosition - 0);
    currentHeadPosition = 0;

    for(int i = index; i < REQUESTS; i++){
        totalHeadMovement += abs(currentHeadPosition - sortedRequests[i]);
        currentHeadPosition = sortedRequests[i];
    }

    free(sortedRequests);
    return totalHeadMovement;
}



//cScan FUNKAR!! (trorjag iaf)
int cScan(int requests[], int initialPosition){
    int headPosition = initialPosition;
    int totalHeadMovement = 0;

    int* sortedRequests = selectionSort(requests, REQUESTS);

    int index = -1;

    //Find starting index
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] > initialPosition){
            index = i;
            break;
        }
    }
    //printf("index:%d\n", index);

    for(int i = index; i < REQUESTS; i++){
        if(headPosition < sortedRequests[i]){
            totalHeadMovement += abs(headPosition - sortedRequests[i]);
            //printf("index %d, value:%d, addedValue:%d\n", i, sortedRequests[i], abs(headPosition-sortedRequests[i]));
            headPosition = sortedRequests[i];
            
        }
    }
    //printf("current Head positiond%d\n", headPosition);
    totalHeadMovement += abs(CYLINDERS-1-headPosition);
   // printf("addedValue:%d\n", abs(headPosition-199));
    totalHeadMovement += abs(CYLINDERS - 1 - 0);
    //printf("addedValue:%d\n", abs(headPosition-0));
    headPosition = 0;
    for(int i = 0; i < index; i++){
        totalHeadMovement += abs(headPosition - sortedRequests[i]);
        //printf("index %d, value:%d, addedValue:%d\n", i, sortedRequests[i], abs(headPosition-sortedRequests[i]));
        headPosition = sortedRequests[i];
    }

    free(sortedRequests);
    return totalHeadMovement;
}
//LOOk algortihm
int look(int requests[], int initialPosition){
    int headPosition = initialPosition;
    int totalHeadMovement = 0;

   int* sortedRequests = selectionSort(requests, REQUESTS);

    int index = -1;

    //Find starting index
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] > initialPosition){
            index = i;
            break;
        }
    }

    for(int i = index; i < REQUESTS; i++){
        totalHeadMovement += abs(headPosition - sortedRequests[i]);
        headPosition = sortedRequests[i];
    }

    for(int i = index - 1; i >= REQUESTS; i--){
        totalHeadMovement += abs(headPosition - sortedRequests[i]);
        headPosition = sortedRequests[i];
    }

    free(sortedRequests);
    return totalHeadMovement;


}

int cLook(int requests[], int initialPosition){
    int headPosition = initialPosition;
    int totalHeadMovement = 0;

   int* sortedRequests = selectionSort(requests, REQUESTS);

    int index = -1;

    //Find starting index
    for(int i = 0; i < REQUESTS; i++){
        if(sortedRequests[i] > initialPosition){
            index = i;
            break;
        }
    }

    for(int i = index; i < REQUESTS; i++){
        totalHeadMovement += abs(headPosition - sortedRequests[i]);
        headPosition = sortedRequests[i];
    }

    for(int i = 0; i < index; i++){
        totalHeadMovement += abs(headPosition - sortedRequests[i]);
        headPosition = sortedRequests[i];
    }

    free(sortedRequests);
    return totalHeadMovement;


}



// Function to generate an array of random cylinder requests


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_position>\n", argv[0]);
        return 1;
    }

    int initialPosition = atoi(argv[1]);

    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};


    int fcfsCount = fcfs(requests, initialPosition);
    int sstfCount = sstf(requests, initialPosition);
    int scanCount = scan(requests, initialPosition);
    int cscanCount = cScan(requests, initialPosition);
    int lookCount = look(requests, initialPosition);
    int clookCount = cLook(requests, initialPosition);

    printf("FCFS total head movements: %d\n", fcfsCount);
    printf("SSTF total head movements: %d\n", sstfCount);
    printf("SCAN total head movements: %d\n", scanCount);
    printf("CSCAN total head movements: %d\n", cscanCount);
    printf("Look total head movements: %d\n", lookCount);
    printf("CLOOK total head movements: %d\n", clookCount);


    return 0;
}

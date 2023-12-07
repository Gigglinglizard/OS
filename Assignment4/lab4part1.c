#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CYLINDERS 5000
#define REQUESTS 1000

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

    bool check[1000];

    for(int i = 0; i < 1000; i++){
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

        printf("next value: %d, mindistance: %d\n", nextValue, minDistance);
        check[nextValue] = true;
        totalHeadMovement += minDistance;
        currentHeadPosition = requests[nextValue]; 
    }
    return totalHeadMovement;   
}

//Scan algoritm, Kanske funkar????? riktig spagetthi kod helt ärligt
int scan(int requests[], int initalPosition){
    int currentHeadPosition = initalPosition;
    int totalHeadMovement = 0;

     bool check[1000];

    for(int i = 0; i < 1000; i++){
        check[i] = false;
    }

    // 0 = vänster, 1 = höger
    int movementDirection = 0;

    for(int i = 0; i < 8; i++) {
        int minDistance = __INT_MAX__;
        int nextValue = -1;

        for(int j = 0; j < 8; j++){
           if(!check[j]) {
                if(movementDirection == 0){
                    if(requests[j] < currentHeadPosition) {
                        int distance = abs(currentHeadPosition - requests[j]);
                        if(distance < minDistance){
                            minDistance = distance;
                            nextValue = j;
                        }    
                    }
                    
                }
                if(movementDirection == 1){
                    if(requests[j] > currentHeadPosition) {
                        int distance = abs(currentHeadPosition - requests[j]);
                        if(distance < minDistance){
                            minDistance = distance;
                            nextValue = j;
                        }    
                    }  
                }
            } 
        }
        if(minDistance == __INT_MAX__){
            printf("moving direction\n");
            totalHeadMovement += currentHeadPosition;
            currentHeadPosition = 0;
            movementDirection = 1;
            for(int k = 0; k < 8; k++) {
                if(!check[k]){
                    if(movementDirection == 1){
                        if(requests[k] > currentHeadPosition) {
                            int distance = abs(currentHeadPosition - requests[k]);
                            if(distance < minDistance){
                                minDistance = distance;
                                nextValue = k;
                            }    
                        }  
                    }    
                }
                
            }
            printf("next value: %d, mindistance: %d, value:%d\n", nextValue, minDistance, requests[nextValue]);
            check[nextValue] = true;
            totalHeadMovement += minDistance;
            currentHeadPosition = requests[nextValue];

        } else {
            printf("next value: %d, mindistance: %d, value:%d\n", nextValue, minDistance, requests[nextValue]);
            check[nextValue] = true;
            totalHeadMovement += minDistance;
            currentHeadPosition = requests[nextValue];
        }
         
    }
    return totalHeadMovement;   
}

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


// Function to generate an array of random cylinder requests


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_position>\n", argv[0]);
        return 1;
    }

    int initialPosition = atoi(argv[1]);

    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};

    int cscanCount = cScan(requests, initialPosition);

    printf("scan total head movements: %d\n", cscanCount);


    return 0;
}

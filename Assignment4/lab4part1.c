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


// Function to generate an array of random cylinder requests


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_position>\n", argv[0]);
        return 1;
    }

    int initialPosition = atoi(argv[1]);

    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};

    int scanCount = scan(requests, initialPosition);

    printf("scan total head movements: %d\n", scanCount);


    return 0;
}

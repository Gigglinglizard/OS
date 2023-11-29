#include <stdio.h>
#include <stdlib.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define BACKING_STORE_SIZE 65536

typedef struct{
    int physical_address;
} PhysicalMemory;

typedef struct{
    int valid;
    int frame_number;
} PageTableEntry;

PageTableEntry page_table[256]; 

void initializePageTable() {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        page_table[i].valid = 0;
    }
}

void extract_page_and_offset(int number, int *page_number, int *page_offset) {
    *page_number = (number >> 8) & 0xFF;  // Right shift by 8 bits to get the page number, then mask to keep only the lower 8 bits
    *page_offset = number & 0xFF;  // Mask to keep only the lower 8 bits for the page offset 
}

int main() {
    PhysicalMemory physicalMemory[256*256]; 
    initializePageTable();
    

    int numbers[] = {16916, 62493, 30198, 53683};
    int num_elements = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < num_elements; i++) {
        int page, offset;
        extract_page_and_offset(numbers[i], &page, &offset);

    page_table[i].valid = 1;
    page_table[i].frame_number = i; 

    // Check if the page table entry is valid
    if (page_table[i].valid) {
        // Calculate the physical address using the frame number and offset
        int physical_address = (page_table[i].frame_number * PAGE_SIZE) + offset;
        printf("Virtual Address: %d, Physical address: %d\n", numbers[i], physical_address);
    } else {
        // Page fault: The page is not currently in memory
        printf("Page fault for virtual address %d\n", i);
        return -1; // Return an error code for simplicity
    }

        


        //printf("Number: %d, Page Number: %d, Page Offset: %d\n", numbers[i], page, offset);
    }

    return 0;
}



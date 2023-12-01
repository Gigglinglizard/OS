#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define BACKING_STORE_SIZE 65536

FILE *backing_store;

typedef struct {
    int frame_number;
    int page_number;
} TLBEntry;

typedef struct{
    int valid;
    int frame_number;
} PageTableEntry;

int tlbHit = 0;
int page_fault = 0;
int totalMemoryAccesses = 0;

PageTableEntry page_table[256]; 
char physicalMemory[PHYSICAL_MEMORY_SIZE]; 
TLBEntry tlb[TLB_SIZE];
bool frame_exist[PHYSICAL_MEMORY_SIZE] = {false}; 

int tlb_index = 0;

void initializeTLB() {
    for (int i = 0; i < TLB_SIZE; ++i) {
        tlb[i].page_number = -1; 
    }
}

void initializePageTable() {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        page_table[i].valid = 0;
    }
}

void initialisePhysicalMemory(){
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; ++i) {
        physicalMemory[i] = -1;
    }
}

void openBackingStore() {
    backing_store = fopen("BACKING_STORE.bin", "rb");
}

void closeBackingStore() {
    fclose(backing_store);
}

void readPageFromBackingStore(int page, char *buffer) {
    fseek(backing_store, page * PAGE_SIZE, SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, backing_store);
}

void extract_page_and_offset(int number, int *page_number, int *page_offset) {
    *page_number = number >> 8;  
    *page_offset = number & 0xFF;  
}

int find_empty_frame() {
    int first_byte_in_frame = 0;
    while(frame_exist[first_byte_in_frame]){
        first_byte_in_frame++;
    }
    for(int i = first_byte_in_frame; i < PAGE_SIZE+first_byte_in_frame; i++){ 
        frame_exist[i] = true;
    }

    return first_byte_in_frame;
}


void calc_phys(int i, int logical_address, char *physicalMemory){
    int page, offset;
    extract_page_and_offset(logical_address, &page, &offset);
    
    for (int i = 0; i < TLB_SIZE; ++i) {
        if (tlb[i].page_number == page) {
            // TLB hit

            tlbHit++;
            int hit_phys_address = (tlb[i].frame_number * PAGE_SIZE) + offset;

            // Read the signed byte value from the translated physical address
            signed char value = physicalMemory[hit_phys_address];

            printf("Virtual Address: %d, page Number: %d, Physical address: %d, Value: %d\n", logical_address, page, hit_phys_address, value);
            return;
        }
    }
    
    // Check if the page table entry is valid
    if (page_table[page].valid) {
        // TLB miss

        // Calculate the physical address using the frame number and offset
        int miss_physical_address = (page_table[page].frame_number * PAGE_SIZE) + offset;

        signed char value = physicalMemory[miss_physical_address];

        // Update TLB
        tlb[tlb_index].page_number = page;
        tlb[tlb_index].frame_number = page_table[page].frame_number;
        tlb_index = (tlb_index + 1) % TLB_SIZE; // Implement FIFO replacement strategy

        printf("Virtual Address: %d, pagenumber: %d, Physical address: %d, Value: %d\n", logical_address, page, miss_physical_address, value);
        return;
    } else {
        // Page fault handling 

        page_fault++;
        // Read the page from the BACKING_STORE.bin file and store it in buffer
        char buffer[PAGE_SIZE];
        readPageFromBackingStore(page, buffer);
        
        // Find an available frame in physical memory
        int frame = find_empty_frame(); 
        
        // Copy to physical memory
        for (int i = 0; i < PAGE_SIZE; i++){
            physicalMemory[frame+i] = buffer[i];
        }
        
        // Calculate address (page size not needed considering phys mem is single array)
        int fault_phys_address = frame + offset;

        signed char value = physicalMemory[fault_phys_address];

        // Update page table and TLB
        page_table[page].valid = 1;
        page_table[page].frame_number = i;

        tlb[tlb_index].page_number = page;
        tlb[tlb_index].frame_number = page_table[page].frame_number;
        tlb_index = (tlb_index + 1) % TLB_SIZE;

        printf("Virtual Address: %d, Page Number: %d, Physical Address: %d, Value: %d\n", logical_address, page, fault_phys_address, value);
        return;
    }
}

int main(int argc, char *argv[]) {
    initializePageTable();
    initializeTLB();
    openBackingStore();
    initialisePhysicalMemory();
    
    FILE *file = fopen(argv[1], "r");
    int logical_address;

    int i = 0;

    while ((fscanf(file, "%d", &logical_address) == 1)) {
        totalMemoryAccesses++;
        calc_phys(i, logical_address, physicalMemory);
        i++;
        if(i >= 256){
            i = 0;
        }
    }

    float tlbHitRate = ((float)tlbHit / totalMemoryAccesses) * 100;
    float pageFaultRate = ((float)page_fault / totalMemoryAccesses) * 100;

    printf("TLB Hit Rate: %.2f%%\n", tlbHitRate);
    printf("Page Fault Rate: %.2f%%\n", pageFaultRate);
    closeBackingStore();

    return 0;
}



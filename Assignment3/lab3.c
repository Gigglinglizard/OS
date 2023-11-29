#include <stdio.h>
#include <stdlib.h>

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
    int physical_address;
} PhysicalMemory;

typedef struct{
    int valid;
    int frame_number;
} PageTableEntry;

int tlbHit = 0;

PageTableEntry page_table[256]; 

TLBEntry tlb[TLB_SIZE];
int tlb_index = 0;

void initializeTLB() {
    for (int i = 0; i < TLB_SIZE; ++i) {
        tlb[i].page_number = -1; // Initialize page numbers to -1 to indicate empty entries
    }
}

void initializePageTable() {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        page_table[i].valid = 0;
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
    fread(buffer, 1, PAGE_SIZE, backing_store);
}

void extract_page_and_offset(int number, int *page_number, int *page_offset) {
    *page_number = (number >> 8) & 0xFF;  // Right shift by 8 bits to get the page number, then mask to keep only the lower 8 bits
    *page_offset = number & 0xFF;  // Mask to keep only the lower 8 bits for the page offset 
}

void calc_phys(int i, int logical_address){
    int page, offset;
    extract_page_and_offset(logical_address, &page, &offset);
    

    for (int i = 0; i < TLB_SIZE; ++i) {
        if (tlb[i].page_number == page) {
            // TLB hit
            tlbHit++;
            printf("lol\n");
            int phys_address = (tlb[i].frame_number * PAGE_SIZE) + offset;
            printf("Virtual Address: %d, page Number: %d, Physical address: %d\n", logical_address, page, phys_address);
            return;
        }
    }
    
    //update page table since tlb failed
    //tog bort denhär för backendstore skiten, kanske ska tillbakakommenteras
    /*
    if(page_table[page].valid != 1) {
        page_table[page].valid = 1;
        page_table[page].frame_number = i; 
    } */
   
    
    // Check if the page table entry is valid
    if (page_table[page].valid) {
        // Calculate the physical address using the frame number and offset
        int physical_address = (page_table[page].frame_number * PAGE_SIZE) + offset;

        tlb[tlb_index].page_number = page;
        tlb[tlb_index].frame_number = page_table[page].frame_number;
        tlb_index = (tlb_index + 1) % TLB_SIZE; // Implement FIFO replacement strategy

        printf("Virtual Address: %d, pagenumber: %d, Physical address: %d\n", logical_address, page, physical_address);
        return;
    } else {
        //printf("Page Fault: Virtual Address: %d, Page Number: %d\n", logical_address, page);

        // Read the page from the BACKING_STORE.bin file and store it in physical memory
        char buffer[PAGE_SIZE];
        readPageFromBackingStore(page, buffer);

        // Find an available frame in physical memory
        int frame = i; // For simplicity, using the same index as the frame number

        // Update page table and TLB
        page_table[page].valid = 1;
        page_table[page].frame_number = frame;

        tlb[tlb_index].page_number = page;
        tlb[tlb_index].frame_number = frame;
        tlb_index = (tlb_index + 1) % TLB_SIZE;

        printf("Virtual Address: %d, Page Number: %d, Physical Address: %d\n", logical_address, page, frame * PAGE_SIZE + offset);
        return;
    }
}






int main(int argc, char *argv[]) {
    PhysicalMemory physicalMemory[256][256]; 
    initializePageTable();
    initializeTLB();
    openBackingStore();
    

    FILE *file = fopen(argv[1], "r");
    int logical_address;

    int numbers[] = {16916, 62493, 30198, 53683, 40185, 28781, 24462, 48399, 64815, 18295, 12218, 22760, 57982, 27966, 54894, 38929};
    int num_elements = sizeof(numbers) / sizeof(numbers[0]);

    int i = 0;

    while ((fscanf(file, "%d", &logical_address) == 1)) {
        // Process each logical_address as needed
        // For example, you can print each address:
        calc_phys(i, logical_address);
        i++;
    }
    /*
    for (int i = 0; i < num_elements; i++) {
        calc_phys(i, numbers[i]);
    } */
    printf("amount of tlb hits: %d\n", tlbHit);

    return 0;
}



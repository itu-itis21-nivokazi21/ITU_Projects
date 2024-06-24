/**
 * BLG 312E - Computer Operating Systems
 * Assignment 2
 * Erblina Nivokazi
 * 150200917
 */


#include "procedures.h"
#define NUM_PROCESSES 10


size_t roundPageSize(size_t size) {  // Function to round up the size to the nearest multiple of page size
    size_t remainder = size % SYSTEM_PAGE_SIZE;
    if (remainder == 0) {
        return size;
    } else {
        return size + SYSTEM_PAGE_SIZE - remainder;
    }
}


int InitMyMalloc(int HeapSize) { // function to initialize memory allocation system

    // Chec if the heap is already initialized or if the requested size is invalid
    if (heap_start != NULL || HeapSize <= 0) {
        return -1;
    }

    // calculate the required heap size, rounded up to the nearest page size
    heap_size = roundPageSize(HeapSize + sizeof(MemoryBlock));

    //allocate memory using mmap
    heap_start = mmap(NULL, 
                      heap_size, 
                      PROT_READ | PROT_WRITE, 
                      MAP_PRIVATE | MAP_ANONYMOUS, 
                      -1, 0);
    
    if (heap_start == MAP_FAILED) { // Check if memory allocation failed
        return -1;
    }

    // Initialize the block list at the start of the allocated memory
    block_list = (MemoryBlock *)heap_start;

    block_list->size = heap_size - sizeof(MemoryBlock); // Adjust for the header
    // No next block yet
    block_list->next = NULL;
    // Mark as free
    block_list->is_free = true;

    return 0; // Successss
}


// func to allocate memory
void *MyMalloc(int size, int strategy) {
    if (size <= 0 || heap_start == NULL) {  // Validate the input size and th heap initialization
        return NULL;
    }

    MemoryBlock *current = block_list, *optimalBlock = NULL;
    // Search through the memory blocks to find the most appropriate one based on the allocation strategy

    bool looped_once = false;

    // I added this to keep track of the last allocated block
    // THis helped with the implementation of the Next Fit strategy
    if(strategy == 3 && last_allocated != NULL && last_allocated->next != NULL) {
        current = last_allocated->next;
    }

    for (; current != NULL; current = current->next) {
        if (current->is_free && current->size >= size) {  // Check if block is free and large enough
            // Apply the allocation strategy to select the optimal block
            if ((strategy == 0 && (optimalBlock == NULL || current->size < optimalBlock->size)) ||  // Best Fit strat
                (strategy == 1 && (optimalBlock == NULL || current->size > optimalBlock->size)) ||  // Worst Fit strat
                (strategy == 2 && optimalBlock == NULL) ||  // First Fit strat
                (strategy == 3 && optimalBlock == NULL)) {  // Next Fit strat
                optimalBlock = current;
                if (strategy == 2 || strategy == 3) {
                    break;  // exit early for First Fit and Next Fit after finding the first suitable block
                }
            }
        }

        if(strategy == 3 && current->next == NULL && !looped_once) {
            current = block_list;
            looped_once = true;
        }
    }

    if (optimalBlock == NULL) {  // If no suitable block is found, return NULL
        return NULL;
    }

    // If the optimal block has more space than needed, split it
    if (optimalBlock->size > size + sizeof(MemoryBlock)) {
        MemoryBlock *newBlock = (MemoryBlock *)((char *)optimalBlock + sizeof(MemoryBlock) + size);
        // Adjust the size and pointers of the new block
        newBlock->size = optimalBlock->size - size - sizeof(MemoryBlock);
        // Update the pointers of the new block
        newBlock->next = optimalBlock->next;
        //update the pointers of the current block
        newBlock->is_free = true;
        last_allocated = newBlock;
        
        optimalBlock->next = newBlock;
        optimalBlock->size = size;  // Update size of the allocated block
    }else {
        // If the optimal block is the right size update the last allocated block
        last_allocated = optimalBlock;
    }

    optimalBlock->is_free = false;  // Mark block as not free
    void* ptr = (void *)((char *)optimalBlock + sizeof(MemoryBlock));  //calculate the pointer to return
    return ptr;  // Return the address right after the block header
}


int MyFree(void *ptr) { // Function to free allocated memory
    
    if (!ptr || !heap_start) { // check the pointer and check if the memory management system has been initialized
        return -1;  // Return an error if the pointer is NULL or the heap isn't initialized
    }

    // Retrieve the memory block's header using the given pointer
    MemoryBlock *freedBlock = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock));
    freedBlock->is_free = true;  // Mark the block as free

    // Traverse the block list to merge adjacent free blocks and reduce fragmentation
    for (MemoryBlock *current = block_list; current; current = current->next) {
        // Check if the current block can be merged with the next one
        if (current->is_free && current->next && current->next->is_free) {
            char *boundary = (char *)current + current->size + sizeof(MemoryBlock);
            if (boundary == (char *)current->next) {
                // If the current and next blocks are contiguous merge them
                current->size += current->next->size + sizeof(MemoryBlock);
                // Check if the last allocated block is gonna be merged
                if(last_allocated == current->next) {
                    last_allocated = current;
                }
                current->next = current->next->next;
                
            }
        }
    }

    return 0;  // Indicate successful memory freeing and coalescing
}



void DumpFreeList() { // Function to print the free list for debugging
    MemoryBlock *current = block_list;
    
    // Print the header for the dump output
    printf("Address\t\tSize\t\tStatus\n");
    printf("---------------------------------------\n");

    // Traverse the free list and print each blocks details
    while (current != NULL) {
        // Determine the status of the block
        const char *status = current->is_free ? "Free" : "Allocated";

        //output the address, size and status of the current block
        printf("%p\t%lu\t\t%s\n", (void *)current, current->size, status);

        // Move to the next block in the list
        current = current->next;
    }

    // Print a newline for better separation after the list
    printf("\n");
}

int main() {
    int requested_heap_size, sizes[NUM_PROCESSES] = {1000, 200, 300, 400};
    void *pointers[NUM_PROCESSES] = {NULL}; // Array to store pointers to allocated memory
    int free_index;

    // I made the processes number fixed for a better user experience, since already in the input there aa lot info
    // being collected
    printf("Memory allocation will be tested on %d child processes.\n", NUM_PROCESSES);

    // Request the size of the heap from the user
    //  The heap size is the total memory available for allocation
    printf("Enter the size of the heap: ");
    if (scanf("%d", &requested_heap_size) != 1) {
        printf("Failed to read heap size.\n");
        return 1;  // Exit if input is invalid
    }

    // Initialize memory allocation outside the child processes to have a single heap for all
    if (InitMyMalloc(requested_heap_size) == -1) {
        printf("Memory allocation has failed at initialization!\n");
        return 1;
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        int strategy, process_size;
        printf("\n--------------------------------\n");
        printf("Allocation attempt for Process %d\n", i + 1);

        printf("Enter the size for this process: ");
        if (scanf("%d", &process_size) != 1) {
            printf("Failed to read process size.\n");
            continue;  // Skip this iteration if the  input fails
        }

        printf("Enter the allocation strategy for this process (0-BF, 1-WF, 2-FF, 3-NF): ");
        if (scanf("%d", &strategy) != 1) {
            printf("Failed to read allocation strategy.\n");
            continue;  // Skip this iteration if the  input fails
        }

        sizes[i] = process_size;  // Update the size for this process
        pointers[i] = MyMalloc(sizes[i], strategy);
        if (pointers[i] == NULL) {
            printf("Memory allocation failed for %d bytes using strategy %d.\n", sizes[i], strategy);
        } else {
            printf("Allocation successful: Pointer: %p, Size: %zu\n", pointers[i], ((MemoryBlock *)((char *)pointers[i] - sizeof(MemoryBlock)))->size);
        }
        DumpFreeList();

        // Ask if the user wants to free any allocated memory so that we can create different heap memory allocation scenarios
        // And test the strategies
        printf("Do you want to free any allocated memory now? Enter the process number (1-10), or 0 to skip: ");
        if (scanf("%d", &free_index) == 1 && free_index > 0 && free_index <= NUM_PROCESSES && pointers[free_index - 1] != NULL) {
            MyFree(pointers[free_index - 1]);
            
            pointers[free_index - 1] = NULL; // Ensure the pointer is marked as freed
            
            printf("Memory for process %d has been freed.\n", free_index);
            DumpFreeList();
        }
    }

    // Optionally free remaining allocated memory
    printf("Freeing any remaining allocated memory...\n");
    for (int j = 0; j < NUM_PROCESSES; j++) {
        if (pointers[j] != NULL) {

            printf("Freeing memory for process %d\n", j + 1);
            
            MyFree(pointers[j]);
            DumpFreeList();
        }
    }

    // Deallocate all allocated memory and exit
    if (munmap(heap_start, heap_size) == -1) {

        printf("Memory deallocation has failed!\n");
        return 1;
    }

    return 0;
}

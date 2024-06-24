/**
 * BLG 312E - Computer Operating Systems
 * Assignment 2
 * Erblina Nivokazi
 * 150200917
 */

#ifndef PROCEDURES_H
#define PROCEDURES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>

#define SYSTEM_PAGE_SIZE getpagesize()


typedef struct MemoryBlock {  // Definitions for the memory management

    size_t size;    // Size of the memory block
    struct MemoryBlock *next;  // Pointer to the next block
    struct MemoryBlock *prev;  // Pointer to the previous block
    bool is_free;                 // Status of the block (free or allocated)
} MemoryBlock;

// Global pointers for memory management
MemoryBlock *block_list = NULL;

MemoryBlock *last_allocated = NULL;

void *heap_start = NULL;

size_t heap_size = 0;

// Declarations of memory management functions which are implemented in main.c
int InitMyMalloc(int HeapSize);

void *MyMalloc(int size, int strategy);

int MyFree(void *ptr);

void DumpFreeList();

#endif // PROCEDURES_H

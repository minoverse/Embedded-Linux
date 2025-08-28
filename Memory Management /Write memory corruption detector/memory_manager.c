#include "memory_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MEMORY_POOL_SIZE 1024

// Private state (not visible outside this file)
static char memory_box[MEMORY_POOL_SIZE];
static int  is_used[MEMORY_POOL_SIZE];
static int  pool_started = 0;

#define START_MAGIC 0xDEADBEEF
#define END_MAGIC   0xCAFEBABE

typedef struct {
    uint32_t start_magic;
    int      block_size;
    int      is_free;
    char*    user_data;
} memory_block;

//  (private) 
static int find_free_space(int bytes_needed) {
    for (int start_pos = 0; start_pos <= MEMORY_POOL_SIZE - bytes_needed; start_pos++) {
        bool ok = true;
        for (int p = start_pos; p < start_pos + bytes_needed; p++) {
            if (is_used[p]) { ok = false; break; }
        }
        if (ok) return start_pos;
    }
    return -1;
}

static void mark_memory_used(int start_pos, int size) {
    for (int p = start_pos; p < start_pos + size && p < MEMORY_POOL_SIZE; p++) {
        is_used[p] = 1;
    }
}

//  API 
void setup_memory_pool(void) {
    if (pool_started) return;
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) is_used[i] = 0;
    memset(memory_box, 0, sizeof(memory_box));
    pool_started = 1;
    printf("Memory pool initialized with %d bytes\n", MEMORY_POOL_SIZE);
}

void* my_malloc(int bytes_needed) {
    setup_memory_pool();
    if (bytes_needed <= 0) {
        printf(" Error: Asked for %d bytes (must be positive!)\n", bytes_needed);
        return NULL;
    }

    int total_needed = (int)sizeof(memory_block) + bytes_needed + (int)sizeof(uint32_t);
    if (total_needed > MEMORY_POOL_SIZE) {
        printf(" Error: Asked for %d bytes, but max is %d\n", total_needed, MEMORY_POOL_SIZE);
        return NULL;
    }

    int start_pos = find_free_space(total_needed);
    if (start_pos == -1) {
        printf(" Out of memory! Couldn't find %d free bytes\n", total_needed);
        return NULL;
    }

    // Set up header first (safer), then footer, then mark used
    memory_block* block = (memory_block*)(&memory_box[start_pos]);
    block->block_size  = bytes_needed;
    block->user_data   = (char*)block + sizeof(memory_block);
    block->start_magic = START_MAGIC;
    block->is_free     = 0;

    uint32_t* end_magic = (uint32_t*)(block->user_data + bytes_needed);
    *end_magic = END_MAGIC;

    mark_memory_used(start_pos, total_needed);

    printf(" Allocated %d bytes at position %d\n", bytes_needed, start_pos);
    return block->user_data;
}

void my_free(void* user_pointer) {
    if (user_pointer == NULL) {
        printf("free NULL pointer\n");
        return;
    }

    memory_block* block = (memory_block*)((char*)user_pointer - sizeof(memory_block));

    if (block->start_magic != START_MAGIC) {
        printf("CORRUPTION DETECTED! Bad start magic number\n");
        return;
    }

    uint32_t* end_magic = (uint32_t*)((char*)user_pointer + block->block_size);
    if (*end_magic != END_MAGIC) {
        printf(" CORRUPTION DETECTED! Bad end magic number (buffer overflow?)\n");
        return;
    }

    if (block->is_free == 1) {
        printf(" DOUBLE FREE DETECTED! This memory was already freed\n");
        return;
    }

    block->is_free = 1;

    int start_pos = (int)((char*)block - memory_box);
    int total_size = (int)sizeof(memory_block) + block->block_size + (int)sizeof(uint32_t);
    for (int p = start_pos; p < start_pos + total_size && p < MEMORY_POOL_SIZE; p++) {
        is_used[p] = 0;
    }

    printf(" Freed %d bytes\n", block->block_size);
}

int check_memory_corruption(void) {
    printf("\n Checking for memory corruption...\n");
    int corrupted_blocks = 0;

    for (int pos = 0; pos <= MEMORY_POOL_SIZE - (int)sizeof(memory_block); pos++) {
        if (is_used[pos]) {
            memory_block* block = (memory_block*)(&memory_box[pos]);
            if (block->start_magic == START_MAGIC) {
                printf("Found block at position %d, size %d bytes\n", pos, block->block_size);
                uint32_t* end_magic = (uint32_t*)(block->user_data + block->block_size);
                if (*end_magic != END_MAGIC) {
                    printf(" CORRUPTION: Block at %d has bad end magic!\n", pos);
                    corrupted_blocks++;
                }
                pos += (int)sizeof(memory_block) + block->block_size + (int)sizeof(uint32_t) - 1;
            }
        }
    }

    if (corrupted_blocks == 0) {
        printf(" No corruption detected!\n");
    } else {
        printf(" Found %d corrupted blocks!\n", corrupted_blocks);
    }
    return corrupted_blocks;
}

void show_memory_stats(void) {
    printf("\n Memory Pool Statistics:\n");
    int free_bytes = 0, used_bytes = 0;
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (is_used[i]) used_bytes++; else free_bytes++;
    }
    printf("Total: %d bytes\n", MEMORY_POOL_SIZE);
    printf("Used:  %d bytes\n", used_bytes);
    printf("Free:  %d bytes\n", free_bytes);
    printf("Usage: %.1f%%\n", (float)used_bytes / MEMORY_POOL_SIZE * 100.0f);
}

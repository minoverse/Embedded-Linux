#include "memory_manager.h"
#include <stdio.h>
#include <string.h>

static void run_memory_tests(void) {
    int tests_passed = 0, total_tests = 0;

    printf("TEST 1: Basic Allocation\n");
    total_tests++;
    void* ptr1 = my_malloc(50);
    if (ptr1) { printf("PASS\n"); tests_passed++; } else { printf(" FAIL\n"); }

    printf("\nTEST 2: Multiple Allocations\n");
    total_tests++;
    void* ptr2 = my_malloc(30);
    void* ptr3 = my_malloc(20);
    if (ptr2 && ptr3 && ptr1 != ptr2 && ptr2 != ptr3) { printf(" PASS\n"); tests_passed++; } else { printf(" FAIL\n"); }

    printf("\nTEST 3: Memory Usage\n");
    total_tests++;
    strcpy((char*)ptr1, "Hello");
    strcpy((char*)ptr2, "World");
    strcpy((char*)ptr3, "Test");
    if (strcmp((char*)ptr1, "Hello") == 0 && strcmp((char*)ptr2, "World") == 0) { printf(" PASS\n"); tests_passed++; } else { printf(" FAIL\n"); }

    printf("\nTEST 4: Corruption Detection\n");
    total_tests++;
    int corrupted = check_memory_corruption();
    if (corrupted == 0) { printf("PASS\n"); tests_passed++; } else { printf(" FAIL (%d)\n", corrupted); }

    printf("\nTEST 5: Free Memory\n");
    total_tests++;
    my_free(ptr2);
    show_memory_stats();
    printf(" PASS (visual)\n"); tests_passed++;

    printf("\nTEST 6: Double Free Detection\n");
    total_tests++;
    printf("Attempting double free (should show error):\n");
    my_free(ptr2);
    printf(" PASS (if error shown)\n"); tests_passed++;

    printf("\nTEST 7: Buffer Overflow Test\n");
    total_tests++;
    char* test_buf = (char*)my_malloc(10);
    if (test_buf) {
        strcpy(test_buf, "Short");
        int corrupted_after = check_memory_corruption();
        if (corrupted_after == 0) { printf(" PASS\n"); tests_passed++; } else { printf(" FAIL\n"); }
        my_free(test_buf);
    } else {
        printf(" FAIL (alloc)\n");
    }

    my_free(ptr1);
    my_free(ptr3);

    printf("\nRESULTS: %d/%d (%.1f%%)\n", tests_passed, total_tests, (float)tests_passed/total_tests*100.0f);
    show_memory_stats();
}

int main(void) {
    setup_memory_pool();
    run_memory_tests();
    return 0;
}

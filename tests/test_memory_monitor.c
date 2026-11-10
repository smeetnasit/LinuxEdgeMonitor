#include <stdio.h>

#include "memory_monitor.h"

int main(void)
{
    int passed = 0;
    int failed = 0;

    struct MemoryStats memory;

    printf("========== MEMORY MONITOR TESTS ==========\n");

    /* Test 1: Read real memory statistics */
    if (get_memory_stats(&memory))
    {
        printf("[PASS] Read memory statistics\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Read memory statistics\n");
        failed++;
    }

    /* Test 2: Total memory must be greater than zero */
    if (memory.total_mb > 0)
    {
        printf("[PASS] Total memory is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Total memory is valid\n");
        failed++;
    }

    /* Test 3: Available memory cannot exceed total */
    if (memory.available_mb <= memory.total_mb)
    {
        printf("[PASS] Available memory is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Available memory is valid\n");
        failed++;
    }


    /* Test 4: Used + available must equal total */
    if (memory.used_kb + memory.available_kb == memory.total_kb)
    {
       printf("[PASS] Memory calculation is consistent\n");
       passed++;
    }
    else
    {
       printf("[FAIL] Memory calculation is consistent\n");
       failed++;
    }

    /* Test 5: Memory usage percentage must be valid */
    if (memory.usage_percentage >= 0.0 &&
        memory.usage_percentage <= 100.0)
    {
        printf("[PASS] Memory percentage is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Memory percentage is valid\n");
        failed++;
    }

    /* Test 6: NULL pointer validation */
    if (!get_memory_stats(NULL))
    {
        printf("[PASS] NULL pointer validation\n");
        passed++;
    }
    else
    {
        printf("[FAIL] NULL pointer validation\n");
        failed++;
    }

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("==================================\n");

    return (failed == 0) ? 0 : 1;
}

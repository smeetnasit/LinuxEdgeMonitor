#include <stdio.h>

#include "disk_space_monitor.h"

int main(void)
{
    int passed = 0;
    int failed = 0;

    struct DiskSpaceStats disk;

    printf("========== DISK SPACE MONITOR TESTS ==========\n");

    /* Test 1: Read actual disk space statistics */
    if (get_disk_space_stats(&disk))
    {
        printf("[PASS] Read disk space statistics\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Read disk space statistics\n");
        failed++;
    }

    /* Test 2: Total disk space must be greater than zero */
    if (disk.total_gb > 0.0)
    {
        printf("[PASS] Total disk space is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Total disk space is valid\n");
        failed++;
    }

    /* Test 3: Available space cannot exceed total space */
    if (disk.available_gb <= disk.total_gb)
    {
        printf("[PASS] Available disk space is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Available disk space is valid\n");
        failed++;
    }

    /* Test 4: Used + available should approximately equal total */
    double calculated_total = disk.used_gb + disk.available_gb;

    if (calculated_total <= disk.total_gb + 0.000001 &&
        calculated_total >= disk.total_gb - 0.000001)
    {
        printf("[PASS] Disk space calculation is consistent\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Disk space calculation is consistent\n");
        failed++;
    }

    /* Test 5: Disk usage percentage must be valid */
    if (disk.usage_percentage >= 0.0 &&
        disk.usage_percentage <= 100.0)
    {
        printf("[PASS] Disk usage percentage is valid\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Disk usage percentage is valid\n");
        failed++;
    }

    /* Test 6: NULL pointer validation */
    if (!get_disk_space_stats(NULL))
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

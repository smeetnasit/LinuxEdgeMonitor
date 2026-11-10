#include <stdio.h>

#include "disk_io_monitor.h"

int main(void)
{
    int passed = 0;
    int failed = 0;

    printf("========== DISK I/O MONITOR TESTS ==========\n");

    struct DiskCounters previous = {
        .sectors_read = 1000,
        .sectors_written = 2000
    };

    struct DiskCounters current = {
        .sectors_read = 1100,
        .sectors_written = 2100
    };

    struct DiskIOStats result;

    /* Test 1: Read actual disk counters */
    struct DiskCounters actual_counters;

    if (read_disk_counters(&actual_counters))
    {
        printf("[PASS] Read Linux disk counters\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Read Linux disk counters\n");
        failed++;
    }

    /* Test 2: Calculate read and write speed */
    if (calculate_disk_io_speed(
            &previous, &current, 2.0, &result) &&
        result.read_speed_kbps == 25.0 &&
        result.write_speed_kbps == 25.0)
    {
        printf("[PASS] Calculate disk read/write speed\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Calculate disk read/write speed\n");
        failed++;
    }

    /* Test 3: Reject zero interval */
    if (!calculate_disk_io_speed(
            &previous, &current, 0.0, &result))
    {
        printf("[PASS] Reject zero sampling interval\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Reject zero sampling interval\n");
        failed++;
    }

    /* Test 4: Reject negative interval */
    if (!calculate_disk_io_speed(
            &previous, &current, -2.0, &result))
    {
        printf("[PASS] Reject negative sampling interval\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Reject negative sampling interval\n");
        failed++;
    }

    /* Test 5: Reject decreasing counters */
    struct DiskCounters invalid = {
        .sectors_read = 900,
        .sectors_written = 2100
    };

    if (!calculate_disk_io_speed(
            &previous, &invalid, 2.0, &result))
    {
        printf("[PASS] Reject decreasing disk counters\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Reject decreasing disk counters\n");
        failed++;
    }

    /* Test 6: Reject NULL previous pointer */
    if (!calculate_disk_io_speed(
            NULL, &current, 2.0, &result))
    {
        printf("[PASS] NULL previous pointer validation\n");
        passed++;
    }
    else
    {
        printf("[FAIL] NULL previous pointer validation\n");
        failed++;
    }

    /* Test 7: Reject NULL result pointer */
    if (!calculate_disk_io_speed(
            &previous, &current, 2.0, NULL))
    {
        printf("[PASS] NULL result pointer validation\n");
        passed++;
    }
    else
    {
        printf("[FAIL] NULL result pointer validation\n");
        failed++;
    }

    /* Test 8: Reject NULL counter pointer */
    if (!read_disk_counters(NULL))
    {
        printf("[PASS] NULL disk counter pointer validation\n");
        passed++;
    }
    else
    {
        printf("[FAIL] NULL disk counter pointer validation\n");
        failed++;
    }

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("==================================\n");

    return (failed == 0) ? 0 : 1;
}


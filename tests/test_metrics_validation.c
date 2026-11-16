#include <stdio.h>
#include <math.h>

#include "metrics_validation.h"

int tests_passed = 0;
int tests_failed = 0;

void run_test(
    const char *test_name,
    double cpu,
    const struct MemoryStats *memory,
    const struct DiskSpaceStats *disk,
    const struct DiskIOStats *disk_io,
    int expected
)
{
    int actual = are_metrics_valid(cpu, memory, disk, disk_io);

    if (actual == expected)
    {
        printf("[PASS] %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s | Expected: %d | Actual: %d\n",
               test_name, expected, actual);
        tests_failed++;
    }
}

int main(void)
{
    struct MemoryStats memory = {
        .total_mb = 8000,
        .used_mb = 4000,
        .available_mb = 4000,
        .usage_percentage = 50.0
    };

    struct DiskSpaceStats disk = {
        .total_gb = 500.0,
        .used_gb = 200.0,
        .available_gb = 300.0,
        .usage_percentage = 40.0
    };

    struct DiskIOStats disk_io = {
        .read_speed_kbps = 100.0,
        .write_speed_kbps = 50.0
    };

    /* Test 1: Valid metrics */
    run_test(
        "Valid metrics",
        45.0, &memory, &disk, &disk_io, 1
    );

    /* Test 2: Negative CPU usage */
    run_test(
        "Negative CPU usage",
        -1.0, &memory, &disk, &disk_io, 0
    );

    /* Test 3: CPU usage above 100% */
    run_test(
        "CPU usage above 100%",
        110.0, &memory, &disk, &disk_io, 0
    );

    /* Test 4: NaN CPU usage */
    run_test(
        "NaN CPU usage",
        NAN, &memory, &disk, &disk_io, 0
    );

    /* Test 5: NULL memory pointer */
    run_test(
        "NULL memory pointer",
        45.0, NULL, &disk, &disk_io, 0
    );

    /* Test 6: Invalid memory values */
    struct MemoryStats invalid_memory = memory;
    invalid_memory.used_mb = 9000;

    run_test(
        "Memory usage exceeds total",
        45.0, &invalid_memory, &disk, &disk_io, 0
    );

    /* Test 7: NULL disk pointer */
    run_test(
        "NULL disk pointer",
        45.0, &memory, NULL, &disk_io, 0
    );

    /* Test 8: Invalid disk values */
    struct DiskSpaceStats invalid_disk = disk;
    invalid_disk.used_gb = -10.0;

    run_test(
        "Negative disk usage",
        45.0, &memory, &invalid_disk, &disk_io, 0
    );

    /* Test 9: NULL disk I/O pointer */
    run_test(
        "NULL disk I/O pointer",
        45.0, &memory, &disk, NULL, 0
    );

    /* Test 10: Negative disk read speed */
    struct DiskIOStats invalid_disk_io = disk_io;
    invalid_disk_io.read_speed_kbps = -1.0;

    run_test(
        "Negative disk read speed",
        45.0, &memory, &disk, &invalid_disk_io, 0
    );

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("==================================\n");

    return (tests_failed == 0) ? 0 : 1;
}

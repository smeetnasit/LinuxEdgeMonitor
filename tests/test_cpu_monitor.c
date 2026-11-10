#include <stdio.h>

#include "cpu_monitor.h"

int tests_passed = 0;
int tests_failed = 0;

void test_cpu_usage(
    const char *test_name,
    struct CpuStats *previous,
    struct CpuStats *current,
    double expected)
{
    double actual = calculate_cpu_usage(previous, current);

    int passed;

    if (expected < 0.0)
    {
        passed = (actual < 0.0);
    }
    else
    {
        passed = (actual >= 0.0 &&
                  actual <= 100.0 &&
                  (actual - expected < 0.01) &&
                  (expected - actual < 0.01));
    }

    if (passed)
    {
        printf("[PASS] %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s | Expected: %.2f | Actual: %.2f\n",
               test_name, expected, actual);
        tests_failed++;
    }
}

int main(void)
{
    struct CpuStats previous = {
        .user = 100,
        .system = 50,
        .idle = 800,
        .iowait = 50
    };

    /* Test 1: Normal CPU usage = 20% */
    struct CpuStats normal = {
        .user = 110,
        .system = 60,
        .idle = 880,
        .iowait = 50
    };

    test_cpu_usage(
        "Normal CPU usage (20%)",
        &previous,
        &normal,
        20.0
    );

    /* Test 2: CPU usage = 0% */
    struct CpuStats idle = {
        .user = 100,
        .system = 50,
        .idle = 900,
        .iowait = 50
    };

    test_cpu_usage(
        "Idle CPU usage (0%)",
        &previous,
        &idle,
        0.0
    );

    /* Test 3: CPU usage = 100% */
    struct CpuStats busy = {
        .user = 200,
        .system = 50,
        .idle = 800,
        .iowait = 50
    };

    test_cpu_usage(
        "Fully busy CPU (100%)",
        &previous,
        &busy,
        100.0
    );

    /* Test 4: Invalid counter values */
    struct CpuStats invalid = {
        .user = 90,
        .system = 50,
        .idle = 800,
        .iowait = 50
    };

    test_cpu_usage(
        "Invalid decreasing counter",
        &previous,
        &invalid,
        -1.0
    );

    /* Test 5: NULL pointer */
    test_cpu_usage(
        "NULL pointer validation",
        NULL,
        &normal,
        -1.0
    );

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("==================================\n");

    return (tests_failed == 0) ? 0 : 1;
}

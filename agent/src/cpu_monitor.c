#include <stdio.h>

#include "cpu_monitor.h"

int read_cpu_stats(struct CpuStats *cpu)
{
    if (cpu == NULL)
    {
        return 0;
    }

    FILE *file;
    char line[256];

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return 0;
    }

    fclose(file);

    int result = sscanf(line,
        "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &cpu->user,
        &cpu->nice,
        &cpu->system,
        &cpu->idle,
        &cpu->iowait,
        &cpu->irq,
        &cpu->softirq,
        &cpu->steal
    );

    return result == 8;
}

double calculate_cpu_usage(
    struct CpuStats *previous,
    struct CpuStats *current)
{
    if (previous == NULL || current == NULL)
    {
        return -1.0;
    }

    unsigned long long total_previous =
        previous->user + previous->nice +
        previous->system + previous->idle +
        previous->iowait + previous->irq +
        previous->softirq + previous->steal;

    unsigned long long total_current =
        current->user + current->nice +
        current->system + current->idle +
        current->iowait + current->irq +
        current->softirq + current->steal;

    unsigned long long idle_previous =
        previous->idle + previous->iowait;

    unsigned long long idle_current =
        current->idle + current->iowait;

    /* Check that counters have not moved backwards */
    if (total_current < total_previous ||
        idle_current < idle_previous)
    {
        return -1.0;
    }

    unsigned long long total_difference =
        total_current - total_previous;

    unsigned long long idle_difference =
        idle_current - idle_previous;

    if (total_difference == 0 ||
        idle_difference > total_difference)
    {
        return -1.0;
    }

    double cpu_usage =
        ((double)(total_difference - idle_difference)
        / total_difference) * 100.0;

    /* Validate the calculated percentage */
    if (cpu_usage < 0.0 || cpu_usage > 100.0)
    {
        return -1.0;
    }

    return cpu_usage;
}

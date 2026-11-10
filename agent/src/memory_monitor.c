#include <stdio.h>

#include "memory_monitor.h"

int get_memory_stats(struct MemoryStats *memory)
{
    if (memory == NULL)
    {
        return 0;
    }

    FILE *file;
    char line[256];

    unsigned long total_kb = 0;
    unsigned long available_kb = 0;

    file = fopen("/proc/meminfo", "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "MemTotal: %lu kB", &total_kb) == 1)
        {
            continue;
        }

        if (sscanf(line, "MemAvailable: %lu kB", &available_kb) == 1)
        {
            break;
        }
    }

    fclose(file);

    if (total_kb == 0 || available_kb == 0 ||
        available_kb > total_kb)
    {
        return 0;
    }

    unsigned long used_kb = total_kb - available_kb;

    memory->total_kb = total_kb;
    memory->available_kb = available_kb;
    memory->used_kb = used_kb;

    memory->total_mb = total_kb / 1024;
    memory->available_mb = available_kb / 1024;
    memory->used_mb = used_kb / 1024;

    memory->usage_percentage =
        ((double)used_kb / total_kb) * 100.0;

    if (memory->usage_percentage < 0.0 ||
        memory->usage_percentage > 100.0)
    {
        return 0;
    }

    return 1;
}

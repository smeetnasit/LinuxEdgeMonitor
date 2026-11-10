#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

struct MemoryStats
{
    unsigned long total_kb;
    unsigned long available_kb;
    unsigned long used_kb;

    unsigned long total_mb;
    unsigned long available_mb;
    unsigned long used_mb;

    double usage_percentage;
};

int get_memory_stats(struct MemoryStats *memory);

#endif

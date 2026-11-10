#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

struct CpuStats
{
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
};

int read_cpu_stats(struct CpuStats *cpu);

double calculate_cpu_usage(
    struct CpuStats *previous,
    struct CpuStats *current
);

#endif

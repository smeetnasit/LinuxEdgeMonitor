#include <stdio.h>
#include <unistd.h>

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

int read_cpu_stats(struct CpuStats *cpu)
{
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

int main()
{
    struct CpuStats first;
    struct CpuStats second;

    printf("Reading first CPU statistics...\n");

    if (!read_cpu_stats(&first))
    {
        printf("Error reading CPU statistics.\n");
        return 1;
    }

    sleep(2);

    printf("Reading second CPU statistics...\n");

    if (!read_cpu_stats(&second))
    {
        printf("Error reading CPU statistics.\n");
        return 1;
    }

    unsigned long long total1 =
        first.user + first.nice + first.system +
        first.idle + first.iowait + first.irq +
        first.softirq + first.steal;

    unsigned long long total2 =
        second.user + second.nice + second.system +
        second.idle + second.iowait + second.irq +
        second.softirq + second.steal;

    unsigned long long idle1 = first.idle + first.iowait;
    unsigned long long idle2 = second.idle + second.iowait;

    unsigned long long total_difference = total2 - total1;
    unsigned long long idle_difference = idle2 - idle1;

    if (total_difference == 0)
    {
        printf("No CPU activity interval detected.\n");
        return 1;
    }

    double cpu_usage =
        ((double)(total_difference - idle_difference)
        / total_difference) * 100.0;

    printf("\nCPU Usage: %.2f%%\n", cpu_usage);

    return 0;
}

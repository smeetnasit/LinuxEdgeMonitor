#include <stdio.h>
#include <unistd.h>

int main()
{
    FILE *file;
    char line[256];

    unsigned long long user1, nice1, system1, idle1;
    unsigned long long iowait1, irq1, softirq1, steal1;

    unsigned long long user2, nice2, system2, idle2;
    unsigned long long iowait2, irq2, softirq2, steal2;

    printf("Reading CPU 0 - First snapshot...\n");

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        printf("Cannot open /proc/stat\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "cpu0 %llu %llu %llu %llu %llu %llu %llu %llu",
            &user1, &nice1, &system1, &idle1,
            &iowait1, &irq1, &softirq1, &steal1) == 8)
        {
            break;
        }
    }

    fclose(file);

    sleep(2);

    printf("Reading CPU 0 - Second snapshot...\n");

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        printf("Cannot open /proc/stat\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "cpu0 %llu %llu %llu %llu %llu %llu %llu %llu",
            &user2, &nice2, &system2, &idle2,
            &iowait2, &irq2, &softirq2, &steal2) == 8)
        {
            break;
        }
    }

    fclose(file);

    unsigned long long total1 =
        user1 + nice1 + system1 + idle1 +
        iowait1 + irq1 + softirq1 + steal1;

    unsigned long long total2 =
        user2 + nice2 + system2 + idle2 +
        iowait2 + irq2 + softirq2 + steal2;

    unsigned long long idle_total1 = idle1 + iowait1;
    unsigned long long idle_total2 = idle2 + iowait2;

    unsigned long long total_difference = total2 - total1;
    unsigned long long idle_difference = idle_total2 - idle_total1;

    if (total_difference == 0 || idle_difference > total_difference)
    {
        printf("Invalid CPU readings.\n");
        return 1;
    }

    double cpu_usage =
        ((double)(total_difference - idle_difference)
        / total_difference) * 100.0;

    printf("\nCPU 0 Usage: %.2f%%\n", cpu_usage);

    return 0;
}

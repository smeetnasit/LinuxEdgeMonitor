#include <stdio.h>

int main()
{
    FILE *file;
    char line[256];

    unsigned long total_memory = 0;
    unsigned long available_memory = 0;
    unsigned long used_memory = 0;
    double memory_usage = 0.0;

    file = fopen("/proc/meminfo", "r");

    if (file == NULL)
    {
        printf("Error: Cannot open /proc/meminfo\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "MemTotal: %lu kB", &total_memory) == 1)
        {
            printf("Total Memory: %lu kB\n", total_memory);
        }

        if (sscanf(line, "MemAvailable: %lu kB", &available_memory) == 1)
        {
            printf("Available Memory: %lu kB\n", available_memory);
        }
    }
     used_memory = total_memory - available_memory;

     memory_usage = ((double)used_memory / total_memory) * 100.0;

    fclose(file);

    printf("\nTotal Memory: %.2f MB\n", total_memory / 1024.0);
    printf("Available Memory: %.2f MB\n", available_memory / 1024.0);
    printf("Used Memory: %.2f MB\n", used_memory / 1024.0);
    printf("Memory Usage: %.2f%%\n", memory_usage);
    return 0;
}

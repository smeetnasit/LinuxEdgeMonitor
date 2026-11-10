#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct DiskStats
{
    unsigned long long sectors_read;
    unsigned long long sectors_written;
};

int read_disk_stats(struct DiskStats *disk)
{
    FILE *file;
    char line[256];
    char device[32];

    unsigned int major, minor;

    unsigned long long reads_completed;
    unsigned long long reads_merged;
    unsigned long long read_time;

    unsigned long long writes_completed;
    unsigned long long writes_merged;

    unsigned long long write_time;

    file = fopen("/proc/diskstats", "r");

    if (file == NULL)
    {
        printf("Error: Cannot open /proc/diskstats\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        int result = sscanf(line,
            "%u %u %31s %llu %llu %llu %llu %llu %llu %llu %llu",
            &major, &minor, device,
            &reads_completed, &reads_merged,
            &disk->sectors_read, &read_time,
            &writes_completed, &writes_merged,
            &disk->sectors_written, &write_time);

        if (result == 11 && strcmp(device, "sda") == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main()
{
    struct DiskStats first, second;

    printf("Reading first disk statistics...\n");

    if (!read_disk_stats(&first))
    {
        printf("Could not read first disk statistics.\n");
        return 1;
    }

    sleep(2);

    printf("Reading second disk statistics...\n");

    if (!read_disk_stats(&second))
    {
        printf("Could not read second disk statistics.\n");
        return 1;
    }

    unsigned long long read_difference =
        second.sectors_read - first.sectors_read;

    unsigned long long write_difference =
        second.sectors_written - first.sectors_written;

    double read_speed =
        (read_difference * 512.0) / 2.0 / 1024.0;

    double write_speed =
        (write_difference * 512.0) / 2.0 / 1024.0;

    printf("\nDisk: sda\n");
    printf("Read Speed: %.2f KiB/s\n", read_speed);
    printf("Write Speed: %.2f KiB/s\n", write_speed);

    return 0;
}

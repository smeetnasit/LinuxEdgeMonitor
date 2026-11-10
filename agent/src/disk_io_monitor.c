#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "disk_io_monitor.h"

int read_disk_counters(struct DiskCounters *counters)
{
    /* Validate pointer */
    if (counters == NULL)
    {
        return 0;
    }

    FILE *file;
    char line[512];
    int found = 0;

    file = fopen("/proc/diskstats", "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, " sda ") != NULL)
        {
            int result = sscanf(line,
                "%*u %*u %*s %*u %*u %llu %*u %*u %*u %llu",
                &counters->sectors_read,
                &counters->sectors_written
            );

            if (result == 2)
            {
                found = 1;
            }

            break;
        }
    }

    fclose(file);

    return found;
}

int calculate_disk_io_speed(
    struct DiskCounters *previous,
    struct DiskCounters *current,
    double interval_seconds,
    struct DiskIOStats *result)
{
    /* Validate pointers */
    if (previous == NULL ||
        current == NULL ||
        result == NULL)
    {
        return 0;
    }

    /* Validate sampling interval */
    if (interval_seconds <= 0.0)
    {
        return 0;
    }

    /* Check for decreasing counters */
    if (current->sectors_read < previous->sectors_read ||
        current->sectors_written < previous->sectors_written)
    {
        return 0;
    }

    unsigned long long read_difference =
        current->sectors_read - previous->sectors_read;

    unsigned long long write_difference =
        current->sectors_written - previous->sectors_written;

    /* One Linux disk sector = 512 bytes */
    double read_bytes = read_difference * 512.0;
    double write_bytes = write_difference * 512.0;

    /* Calculate KiB per second */
    result->read_speed_kbps =
        read_bytes / 1024.0 / interval_seconds;

    result->write_speed_kbps =
        write_bytes / 1024.0 / interval_seconds;

    return 1;
}

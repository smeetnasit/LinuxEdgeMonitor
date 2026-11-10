#ifndef DISK_IO_MONITOR_H
#define DISK_IO_MONITOR_H

struct DiskCounters
{
    unsigned long long sectors_read;
    unsigned long long sectors_written;
};

struct DiskIOStats
{
    double read_speed_kbps;
    double write_speed_kbps;
};

int read_disk_counters(struct DiskCounters *counters);

int calculate_disk_io_speed(
    struct DiskCounters *previous,
    struct DiskCounters *current,
    double interval_seconds,
    struct DiskIOStats *result
);

#endif


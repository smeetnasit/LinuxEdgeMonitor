#ifndef DISK_SPACE_MONITOR_H
#define DISK_SPACE_MONITOR_H

struct DiskSpaceStats
{
    double total_gb;
    double used_gb;
    double available_gb;
    double usage_percentage;
};

int get_disk_space_stats(struct DiskSpaceStats *disk);

#endif

#include <sys/statvfs.h>
#include <stddef.h>
#include "disk_space_monitor.h"

int get_disk_space_stats(struct DiskSpaceStats *disk)
{
    /* Validate pointer */
    if (disk == NULL)
    {
        return 0;
    }

    struct statvfs filesystem;

    /* Read filesystem information */
    if (statvfs("/", &filesystem) != 0)
    {
        return 0;
    }

    /* Calculate total and available storage */
    unsigned long long total_bytes =
        (unsigned long long)filesystem.f_blocks * filesystem.f_frsize;

    unsigned long long available_bytes =
        (unsigned long long)filesystem.f_bavail * filesystem.f_frsize;

    /* Validate storage values */
    if (total_bytes == 0 || available_bytes > total_bytes)
    {
        return 0;
    }

    unsigned long long used_bytes = total_bytes - available_bytes;

    double bytes_per_gb = 1024.0 * 1024.0 * 1024.0;

    /* Convert bytes to GiB */
    disk->total_gb = total_bytes / bytes_per_gb;
    disk->used_gb = used_bytes / bytes_per_gb;
    disk->available_gb = available_bytes / bytes_per_gb;

    /* Calculate disk usage percentage */
    disk->usage_percentage =
        ((double)used_bytes / total_bytes) * 100.0;

    /* Validate percentage */
    if (disk->usage_percentage < 0.0 ||
        disk->usage_percentage > 100.0)
    {
        return 0;
    }

    return 1;
}

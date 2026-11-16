#include <math.h>
#include <stddef.h>

#include "metrics_validation.h"

int are_metrics_valid(
    double cpu_usage,
    const struct MemoryStats *memory,
    const struct DiskSpaceStats *disk,
    const struct DiskIOStats *disk_io
)
{
    /* Validate pointers */
    if (memory == NULL || disk == NULL || disk_io == NULL)
    {
        return 0;
    }

    /* Validate CPU usage */
    if (!isfinite(cpu_usage) ||
        cpu_usage < 0.0 ||
        cpu_usage > 100.0)
    {
        return 0;
    }

    /* Validate memory statistics */
    if (memory->total_mb == 0 ||
        memory->used_mb > memory->total_mb ||
        memory->available_mb > memory->total_mb ||
        !isfinite(memory->usage_percentage) ||
        memory->usage_percentage < 0.0 ||
        memory->usage_percentage > 100.0)
    {
        return 0;
    }

    /* Validate disk-space statistics */
    if (!isfinite(disk->total_gb) ||
        !isfinite(disk->used_gb) ||
        !isfinite(disk->available_gb) ||
        !isfinite(disk->usage_percentage) ||
        disk->total_gb <= 0.0 ||
        disk->used_gb < 0.0 ||
        disk->available_gb < 0.0 ||
        disk->used_gb > disk->total_gb ||
        disk->available_gb > disk->total_gb ||
        disk->usage_percentage < 0.0 ||
        disk->usage_percentage > 100.0)
    {
        return 0;
    }

    /* Validate disk I/O speeds */
    if (!isfinite(disk_io->read_speed_kbps) ||
        !isfinite(disk_io->write_speed_kbps) ||
        disk_io->read_speed_kbps < 0.0 ||
        disk_io->write_speed_kbps < 0.0)
    {
        return 0;
    }

    return 1;
}

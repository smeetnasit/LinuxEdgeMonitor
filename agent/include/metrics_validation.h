#ifndef METRICS_VALIDATION_H
#define METRICS_VALIDATION_H

#include "memory_monitor.h"
#include "disk_space_monitor.h"
#include "disk_io_monitor.h"

int are_metrics_valid(
    double cpu_usage,
    const struct MemoryStats *memory,
    const struct DiskSpaceStats *disk,
    const struct DiskIOStats *disk_io
);

#endif

#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

#include <stddef.h>

#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_space_monitor.h"
#include "disk_io_monitor.h"

int build_metrics_json(
    char *buffer,
    size_t buffer_size,
    double cpu_usage,
    struct MemoryStats *memory,
    struct DiskSpaceStats *disk,
    struct DiskIOStats *disk_io
);

#endif

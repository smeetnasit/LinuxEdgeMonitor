#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_space_monitor.h"
#include "disk_io_monitor.h"

void print_metrics_json(
    double cpu_usage,
    struct MemoryStats *memory,
    struct DiskSpaceStats *disk,
    struct DiskIOStats *disk_io
);

#endif

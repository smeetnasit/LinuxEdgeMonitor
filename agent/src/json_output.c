#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "json_output.h"

int build_metrics_json(
    char *buffer,
    size_t buffer_size,
    double cpu_usage,
    struct MemoryStats *memory,
    struct DiskSpaceStats *disk,
    struct DiskIOStats *disk_io
)
{
    if (buffer == NULL ||
        buffer_size == 0 ||
        memory == NULL ||
        disk == NULL ||
        disk_io == NULL)
    {
        return -1;
    }

    const char *device_id = "DEV-1001";

    time_t current_time = time(NULL);
    struct tm utc_time;

    if (gmtime_r(&current_time, &utc_time) == NULL)
    {
        return -1;
    }

    char timestamp[25];

    if (strftime(
            timestamp,
            sizeof(timestamp),
            "%Y-%m-%dT%H:%M:%SZ",
            &utc_time) == 0)
    {
        return -1;
    }

    int written = snprintf(
        buffer,
        buffer_size,
        "{"
        "\"timestamp\":\"%s\","
        "\"deviceId\":\"%s\","
        "\"agentVersion\":\"1.0.0\","
        "\"cpu\":{\"usagePercentage\":%.2f},"
        "\"memory\":{"
            "\"usagePercentage\":%.2f,"
            "\"usedMb\":%lu,"
            "\"availableMb\":%lu"
        "},"
        "\"disk\":{"
            "\"usagePercentage\":%.2f,"
            "\"usedGb\":%.2f,"
            "\"availableGb\":%.2f,"
            "\"readKibPerSecond\":%.2f,"
            "\"writeKibPerSecond\":%.2f"
        "}"
        "}",
        timestamp,
        device_id,
        cpu_usage,
        memory->usage_percentage,
        memory->used_mb,
        memory->available_mb,
        disk->usage_percentage,
        disk->used_gb,
        disk->available_gb,
        disk_io->read_speed_kbps,
        disk_io->write_speed_kbps
    );

    if (written < 0 || (size_t)written >= buffer_size)
    {
        buffer[0] = '\0';
        return -1;
    }

    return 0;
}

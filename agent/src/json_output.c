#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "json_output.h"

void print_metrics_json(
    double cpu_usage,
    struct MemoryStats *memory,
    struct DiskSpaceStats *disk,
    struct DiskIOStats *disk_io
)
{
    /* Get Linux device hostname */
    char device_id[256];

    if (gethostname(device_id, sizeof(device_id)) != 0)
    {
        return;
    }

    /* Ensure hostname is null-terminated */
    device_id[sizeof(device_id) - 1] = '\0';
    /* Get current system time */
    time_t current_time = time(NULL);

    /* Convert system time to UTC */
    struct tm utc_time;

    if (gmtime_r(&current_time, &utc_time) == NULL)
    {
        return;
    }

    /* Format timestamp */
    char timestamp[25];

    if (strftime(
            timestamp,
            sizeof(timestamp),
            "%Y-%m-%dT%H:%M:%SZ",
            &utc_time) == 0)
    {
        return;
    }
    if (memory == NULL || disk == NULL || disk_io == NULL)
    {
        return;
    }

    printf("{\n");

    printf("  \"timestamp\": \"%s\",\n", timestamp);

    printf("  \"device_id\": \"%s\",\n", device_id);

    printf("  \"agent_version\": \"1.0.0\",\n");

    printf("  \"metrics\": {\n");

    printf("    \"cpu\": {\n");
    printf("      \"usage_percentage\": %.2f\n", cpu_usage);
    printf("    },\n");

    printf("    \"memory\": {\n");
    printf("      \"usage_percentage\": %.2f,\n", memory->usage_percentage);
    printf("      \"used_mb\": %lu,\n", memory->used_mb);
    printf("      \"available_mb\": %lu\n", memory->available_mb);
    printf("    },\n");

    printf("    \"disk\": {\n");
    printf("      \"usage_percentage\": %.2f,\n", disk->usage_percentage);
    printf("      \"used_gb\": %.2f,\n", disk->used_gb);
    printf("      \"available_gb\": %.2f,\n", disk->available_gb);
    printf("      \"read_kib_per_second\": %.2f,\n", disk_io->read_speed_kbps);
    printf("      \"write_kib_per_second\": %.2f\n", disk_io->write_speed_kbps);
    printf("    }\n");
    printf("  }\n");

    printf("}\n");
}

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "json_output.h"
#include "http_client.h"

#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_space_monitor.h"
#include "disk_io_monitor.h"
#include "metrics_validation.h"

volatile sig_atomic_t running = 1;

void handle_signal(int signal_number)
{
    (void)signal_number;
    running = 0;
}

double get_elapsed_seconds(
    struct timespec *start,
    struct timespec *end)
{
    double start_seconds =
        start->tv_sec + start->tv_nsec / 1000000000.0;

    double end_seconds =
        end->tv_sec + end->tv_nsec / 1000000000.0;

    return end_seconds - start_seconds;
}

int main(void)
{
    struct CpuStats previous_cpu;
    struct CpuStats current_cpu;

    double cpu_usage = -1.0;

    struct MemoryStats memory;

    struct DiskCounters previous_disk;
    struct DiskCounters current_disk;

     struct DiskSpaceStats disk;
     struct DiskIOStats disk_io;

    struct timespec previous_sample_time;
    struct timespec current_sample_time;

    fprintf(stderr, "========== LINUX EDGE MONITOR ==========\n");
    fprintf(stderr, "Continuous monitoring started...\n");
    fprintf(stderr, "Sampling interval: 5 seconds\n");
    fprintf(stderr, "Press Ctrl + C to stop.\n");

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Take the initial CPU reading */
    if (!read_cpu_stats(&previous_cpu))
    {
        fprintf(stderr, "Error: Could not read initial CPU statistics.\n");
        return 1;
    }

    /* Take the initial disk I/O reading */
    if (!read_disk_counters(&previous_disk))
    {
        fprintf(stderr, "Error: Could not read initial disk counters.\n");
        return 1;
    }

    /* Record the initial sampling time */
    clock_gettime(CLOCK_MONOTONIC, &previous_sample_time);

    while (running)
    {
        fprintf(stderr, "\nWaiting for the next sample...\n");
        sleep(5);
	clock_gettime(CLOCK_MONOTONIC, &current_sample_time);

	double actual_interval_seconds =
        get_elapsed_seconds(
        &previous_sample_time,
        &current_sample_time
    );

        if (!running)
        {
            break;
        }

	/* Track whether all required metrics are valid */
        int sample_valid = 1;

        /* CPU monitoring */
        if (read_cpu_stats(&current_cpu))
        {
            cpu_usage =
                calculate_cpu_usage(&previous_cpu, &current_cpu);

            /* Update baseline for the next CPU calculation */
            previous_cpu = current_cpu;
        }
        else
        {
            fprintf(stderr, "[ERROR] Failed to read CPU statistics.\n");
            sample_valid = 0;
        }

        /* Memory monitoring */
        if (!get_memory_stats(&memory))
        {
            fprintf(stderr, "[ERROR] Failed to collect memory statistics.\n");
            sample_valid = 0;
        }

        /* Disk space monitoring */
        if (!get_disk_space_stats(&disk))
        {
            fprintf(stderr, "[ERROR] Failed to collect disk space statistics.\n");
            sample_valid = 0;
        }

        /* Disk I/O monitoring */
        if (read_disk_counters(&current_disk))
        {
            if (!calculate_disk_io_speed(
                    &previous_disk,
                    &current_disk,
                    actual_interval_seconds,
                    &disk_io))
            {
                fprintf(stderr, "[ERROR] Failed to calculate disk I/O speed.\n");
                sample_valid = 0;
            }

            /* Update disk baseline after a successful counter read */
            previous_disk = current_disk;
            previous_sample_time = current_sample_time;
        }
        else
        {
            fprintf(stderr, "[ERROR] Failed to read disk I/O counters.\n");
            sample_valid = 0;
        }
        /* Validate all collected metrics before sending */
        if (sample_valid &&
            !are_metrics_valid(
                cpu_usage,
                &memory,
                &disk,
                &disk_io))
        {
            fprintf(stderr, "[ERROR] Metrics validation failed.\n");
            sample_valid = 0;
        }

        if (sample_valid)
        {
            char json_buffer[1024];

            if (build_metrics_json(
                json_buffer,
                sizeof(json_buffer),
                cpu_usage,
                &memory,
                &disk,
                &disk_io) == 0)
            {
                if (send_metrics_json(json_buffer) != 0)
                {
                    fprintf(
                        stderr,
                        "[ERROR] Failed to send metrics to API.\n");
                }
            }
            else
            {
                fprintf(
                    stderr,
                    "[ERROR] Could not build metrics JSON.\n");
            }
        }
        else
        {
            fprintf(
                stderr,
                "[WARNING] Skipping invalid metrics sample.\n");
        }
    
    }

    fprintf(stderr, "\nStopping Linux Edge Monitor...\n");
    fprintf(stderr, "Monitoring stopped successfully.\n");

    return 0;
}

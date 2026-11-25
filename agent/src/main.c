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
        printf("Error: Could not read initial CPU statistics.\n");
        return 1;
    }

    /* Take the initial disk I/O reading */
    if (!read_disk_counters(&previous_disk))
    {
        printf("Error: Could not read initial disk counters.\n");
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


        /* CPU monitoring */

	if (read_cpu_stats(&current_cpu))
	{
          cpu_usage =
          calculate_cpu_usage(&previous_cpu, &current_cpu);

	  previous_cpu = current_cpu;
	}
	else
	{
          cpu_usage = -1.0;
	}

        /* Memory monitoring */

	if (!get_memory_stats(&memory))
	{
         memory.usage_percentage = -1.0;
	}

        /* Disk space monitoring */
	if (!get_disk_space_stats(&disk))
	{
          disk.usage_percentage = -1.0;
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
             disk_io.read_speed_kbps = -1.0;
             disk_io.write_speed_kbps = -1.0;
    	   }

           previous_disk = current_disk;
           previous_sample_time = current_sample_time;
	}
	else
	{
          disk_io.read_speed_kbps = -1.0;
          disk_io.write_speed_kbps = -1.0;
	}

        /* Build JSON and send telemetry to the API */
        char json_buffer[1024];

        if (memory.usage_percentage >= 0.0 &&
            disk.usage_percentage >= 0.0)
        {
            if (build_metrics_json(
                    json_buffer,
                    sizeof(json_buffer),
                    cpu_usage,
                    &memory,
                    &disk,
                    &disk_io) == 0)
            {
                send_metrics_json(json_buffer);
            }
            else
            {
                fprintf(stderr, "Error: Could not build metrics JSON.\n");
            }
        }
        else
        {
            fprintf(stderr, "Error: Failed to collect system metrics.\n");
        }
    }

    fprintf(stderr, "\nStopping Linux Edge Monitor...\n");
    fprintf(stderr, "Monitoring stopped successfully.\n");

    return 0;
}

#include <stdio.h>
#include <sys/statvfs.h>

int main()
{
    struct statvfs disk;
    unsigned long long total_bytes;
    unsigned long long available_bytes;
    unsigned long long used_bytes;

    double total_gb;
    double available_gb;
    double used_gb;
    double usage_percentage;

    if (statvfs("/", &disk) != 0)
    {
        printf("Error reading filesystem information.\n");
        return 1;
    }
	total_bytes = (unsigned long long)disk.f_blocks * disk.f_frsize;

	available_bytes = (unsigned long long)disk.f_bavail * disk.f_frsize;

	used_bytes = total_bytes - available_bytes;

	total_gb = total_bytes / (1024.0 * 1024.0 * 1024.0);

	available_gb = available_bytes / (1024.0 * 1024.0 * 1024.0);

	used_gb = used_bytes / (1024.0 * 1024.0 * 1024.0);

	usage_percentage = ((double)used_bytes / total_bytes) * 100.0;

    printf("Block size: %lu bytes\n",
           (unsigned long)disk.f_frsize);

    printf("Total blocks: %lu\n",
           (unsigned long)disk.f_blocks);

    printf("Available blocks: %lu\n",
           (unsigned long)disk.f_bavail);

	printf("\n--- Disk Space Information ---\n");
	printf("Total Space: %.2f GiB\n", total_gb);
	printf("Used Space: %.2f GiB\n", used_gb);
	printf("Available Space: %.2f GiB\n", available_gb);
	printf("Disk Usage: %.2f%%\n", usage_percentage);
    return 0;
}

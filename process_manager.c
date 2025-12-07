#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define CPU_BOUND_THRESHOLD 20.0
#define IO_BOUND_THRESHOLD 1048576  // 1 MB

// Read CPU usage of process using /proc/[pid]/stat
float get_cpu_usage(int pid) {
    char path[256], buffer[1024];
    FILE *fp;
    long utime, stime;
    float total_time;

    sprintf(path, "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (!fp) return 0;

    for (int i = 0; i < 13; i++) fscanf(fp, "%s", buffer); // skip first 13 fields
    fscanf(fp, "%ld %ld", &utime, &stime);

    fclose(fp);

    total_time = (utime + stime) / (float)sysconf(_SC_CLK_TCK);
    return total_time * 100;  
}

// Read memory usage (RSS)
long get_memory_usage(int pid) {
    char path[256];
    FILE *fp;
    long rss = 0;

    sprintf(path, "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if (!fp) return 0;

    fscanf(fp, "%*s %ld", &rss);
    fclose(fp);

    long page_size = sysconf(_SC_PAGESIZE);
    return rss * page_size; // bytes
}

// Read I/O usage using /proc/[pid]/io
long get_io_usage(int pid) {
    char path[256], label[64];
    FILE *fp;
    long read_bytes = 0, write_bytes = 0, value;

    sprintf(path, "/proc/%d/io", pid);
    fp = fopen(path, "r");
    if (!fp) return 0;

    while (fscanf(fp, "%63s %ld", label, &value) == 2) {
        if (strcmp(label, "read_bytes:") == 0)
            read_bytes = value;
        if (strcmp(label, "write_bytes:") == 0)
            write_bytes = value;
    }

    fclose(fp);
    return read_bytes + write_bytes;
}

// Check if system or user process
const char* get_process_type(int uid) {
    if (uid == 0)
        return "System";
    return "User";
}

// Read process UID
int get_uid(int pid) {
    char path[256], key[64];
    FILE *fp;
    int uid = -1;

    sprintf(path, "/proc/%d/status", pid);
    fp = fopen(path, "r");
    if (!fp) return -1;

    while (fscanf(fp, "%63s %d", key, &uid) == 2) {
        if (strcmp(key, "Uid:") == 0) {
            fclose(fp);
            return uid;
        }
    }

    fclose(fp);
    return -1;
}

// Read process name
void get_process_name(int pid, char *name) {
    char path[256];
    FILE *fp;

    sprintf(path, "/proc/%d/comm", pid);
    fp = fopen(path, "r");
    if (!fp) {
        strcpy(name, "unknown");
        return;
    }

    fscanf(fp, "%255s", name);
    fclose(fp);
}


int main() {
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (!dir) {
        printf("Failed to open /proc\n");
        return 1;
    }

    printf("===== Linux Process Manager =====\n\n");

    while ((entry = readdir(dir)) != NULL) {

        if (!isdigit(entry->d_name[0]))
            continue;

        int pid = atoi(entry->d_name);
        char name[256];

        get_process_name(pid, name);

        int uid = get_uid(pid);
        float cpu = get_cpu_usage(pid);
        long mem = get_memory_usage(pid);
        long io = get_io_usage(pid);

        const char *ptype = get_process_type(uid);

        // Classification
        const char *category;
        if (cpu > CPU_BOUND_THRESHOLD && io < IO_BOUND_THRESHOLD)
            category = "CPU-Bound";
        else if (io >= IO_BOUND_THRESHOLD)
            category = "I/O-Bound";
        else
            category = "Normal";

        printf("PID: %d  |  Name: %s\n", pid, name);
        printf("Type: %s Process\n", ptype);
        printf("CPU Usage: %.2f %%\n", cpu);
        printf("Memory: %.2f MB\n", mem / (1024.0 * 1024));
        printf("I/O: %.2f MB\n", io / (1024.0 * 1024));
        printf("Category: %s\n", category);
        printf("---------------------------------------------\n");
    }

    closedir(dir);
    return 0;
}

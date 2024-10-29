#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "/home/tuitachi/project_internVNPT/server/management_system/log/inc/log.h"

static FILE *log_fp = NULL;

void init_log(const char *log_file) {
    log_fp = fopen(log_file, "a");
    if (!log_fp) {
        perror("Failed to open log file");
    }
}

void write_log(int level, const char *format, ...) {
    if (!log_fp) return;

    // Lấy thời gian hiện tại
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Ghi thời gian và cấp độ log
    fprintf(log_fp, "[%02d-%02d-%02d %02d:%02d:%02d] ",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);

    switch (level) {
        case MSG_DEBUG: fprintf(log_fp, "[DEBUG] "); break;
        case MSG_INFO: fprintf(log_fp, "[INFO] "); break;
        case MSG_WARNING: fprintf(log_fp, "[WARNING] "); break;
        case MSG_ERROR: fprintf(log_fp, "[ERROR] "); break;
        default: fprintf(log_fp, "[UNKNOWN] "); break;
    }

    // Ghi nội dung log
    va_list args;
    va_start(args, format);
    vfprintf(log_fp, format, args);
    va_end(args);

    fprintf(log_fp, "\n");
    fflush(log_fp);
}

void close_log() {
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}

void wpa_printf(int level, const char *fmt, ...) {
    if (!log_fp) return;

    va_list args;
    va_start(args, fmt);
    write_log(level, fmt, args);
    va_end(args);
}

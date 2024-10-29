// log.h
#ifndef LOG_H
#define LOG_H

#define MSG_DEBUG 0
#define MSG_INFO  1
#define MSG_WARNING 2
#define MSG_ERROR 3

void init_log(const char *log_file);
void write_log(int level, const char *format, ...);
void close_log();

// Định nghĩa hàm ghi log
void wpa_printf(int level, const char *fmt, ...);

#endif // LOG_H

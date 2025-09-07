#include "logger.h"
#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static FILE *log_file = NULL;

int logger_init() {
    log_file = fopen("log.csv", "a");
    if(!log_file) return 0;

    fseek(log_file, 0, SEEK_END);
    if(ftell(log_file) == 0) {
        fprintf(log_file, "timestamp,usage,length,data\n");
    }

    fflush(log_file);
    return 1;
}

void logger_log(uint32_t usage, size_t length, const uint8_t *data) {
    if(!log_file) return;

    time_t now = time(NULL);
    fprintf(log_file, "%ld,0x%X,%zu,\"", now, usage, length);
    for(size_t i = 0; i < length; ++i) {
        fprintf(log_file, "%02X ", data[i]);
    }
    fprintf(log_file, "\"\n");
    fflush(log_file);
}

void logger_close() {
    if(log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

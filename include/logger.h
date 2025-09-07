#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <CoreFoundation/CoreFoundation.h>

int logger_init();
void logger_log(uint32_t usage, size_t length, const uint8_t *data);
void logger_close();

#endif

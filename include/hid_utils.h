#ifndef HID_UTILS
#define HID_UTILS

#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>

void parser_usage_0x24(const uint8_t *data, CFIndex length);
void parser_vendor_usage(uint8_t *data, CFIndex length);
void parser_standart_usage(uint32_t usage, int int_value, CFIndex length, const uint8_t *data);

#endif

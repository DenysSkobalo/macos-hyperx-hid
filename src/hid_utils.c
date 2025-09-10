#include "hid_utils.h"
#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>

void parser_vendor_usage(uint8_t *data, CFIndex length) {
    if(length == 5 && data[0] == 0x03) {
        uint8_t code = data[1];
        uint8_t value = data[2];

        printf("[Vendor] Code: 0x%02x, Value: %d\n", code, value);
    }
}

void parser_standart_usage(uint32_t usage, int int_value, CFIndex length, const uint8_t *data) {
    switch (usage) {
        case 0x01:
            printf("\U0001F5B1  Left Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x02:
            printf("\U0001F5B1  Right Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x03:
            printf("\U0001F5B1  Middle Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x30:
            printf("\u27A1  Move X: %d\n", int_value);
            break;
        case 0x31:
            printf("\u2B06  Move Y: %d\n", int_value);
            break;
        case 0x38:
            printf("\U0001F300 Scroll: %d\n", int_value);
            break;
        default:
            printf("Usage: 0x%x | Length: %ld | Data: ", usage, length);
            for (CFIndex i = 0; i < length; ++i) {
                printf("%02x ", data[i]);
            }
            printf("\n");
            break;
    }
}

void parser_usage_0x24(const uint8_t *data, CFIndex length) {
    if (length != 63) {
        printf("[0x24] Unexpected packet length: %ld\n", length);
        return;
    }

    uint8_t report_id = data[0];
    uint8_t button_code = data[1];
    uint8_t value = data[3];

    printf("[0x24] Full packet: ");
    for (int i = 0; i < length; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");

    switch (button_code) {
        case 0x01:
            printf("🖱️ [0x24] Left Button (extended): %s\n", value ? "Pressed" : "Released");
            break;
        case 0x02:
            printf("🖱️ [0x24] Right Button (extended): %s\n", value ? "Pressed" : "Released");
            break;
        case 0x04:
            printf("🖱️ [0x24] Middle Button: %s\n", value ? "Pressed" : "Released");
            break;
        case 0x08:
            printf("🖲️ [0x24] Side Button 5 (bottom): %s\n", value ? "Pressed" : "Released");
            break;
        case 0x10:
            printf("🖲️ [0x24] Side Button 6 (top): %s\n", value ? "Pressed" : "Released");
            break;
        case 0x20:
            printf("🎯 [0x24] DPI Button: %s\n", value ? "Pressed" : "Released");
            break;
        default:
            printf("🧩 [0x24] Unknown button code: 0x%02x, value: 0x%02x\n", button_code, value);
            break;
    }
}

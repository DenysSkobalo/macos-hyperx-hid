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

void parser_usage_FFFFFFFF(const uint8_t* data, CFIndex length) {
    printf("[0xFFFFFFFF] Vendor-defined message (%ld byte(s)): ", length);
    for (CFIndex i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    if (length == 5 && data[0] == 0x03) {
        uint8_t code = data[1];
        uint8_t value = data[2];
        printf("🔧 [Vendor] Type: 0x%02X, Code: 0x%02X, Value: 0x%02X\n", data[0], code, value);
        switch (code) {
            case 0x50:
                printf("🎯 Possible DPI report: value = %d\n", value);
                break;
            case 0x59:
                printf("🎨 Possible RGB or profile switch\n");
                break;
            default:
                printf("❓ Unknown vendor code\n");
        }
    }
}

void parser_dpi_status_packet(const uint8_t *data, size_t length) {
    if (length >= 3 && data[0] == 0x08) {
        uint8_t profile_index = data[1];
        uint8_t dpi_code = data[2];

        printf("🎯 [DPI Status] Profile Index: %u | DPI Code: 0x%02X", profile_index, dpi_code);

        const char *dpi_value = "Unknown";
        switch (dpi_code) {
            case 0x0F: dpi_value = "800 DPI"; break;
            case 0x1F: dpi_value = "1600 DPI"; break;
            case 0x2F: dpi_value = "2400 DPI"; break;
            case 0x3F: dpi_value = "3200 DPI"; break;
        }

        printf(" → Approx DPI: %s\n", dpi_value);
    }
}

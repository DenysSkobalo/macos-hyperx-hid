#include <stdio.h>
#include <stdlib.h>
#include <IOKit/hid/IOHIDManager.h>
#include <CoreFoundation/CoreFoundation.h>
#include "logger.h"

#define VENDOR_ID 0x03f0
#define PRODUCT_ID 0x0b97

// Creating dictionary for searching device
CFMutableDictionaryRef CreateMatchingDictionary(int vendorID, int productID) {
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFNumberRef vid = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &vendorID);
    CFNumberRef pid = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &productID);

    CFDictionarySetValue(dict, CFSTR(kIOHIDVendorIDKey), vid);
    CFDictionarySetValue(dict, CFSTR(kIOHIDProductIDKey), pid);

    CFRelease(vid);
    CFRelease(pid);

    return dict;
}

// Callback, that calling on every HID input event
void HadnleInput(void *context, IOReturn result, void *sender, IOHIDValueRef value) {
    IOHIDElementRef element = IOHIDValueGetElement(value);
    uint32_t usage = IOHIDElementGetUsage(element);
    CFIndex length = IOHIDValueGetLength(value);
    const uint8_t *data = IOHIDValueGetBytePtr(value);
    int int_value = IOHIDValueGetIntegerValue(value);

    switch (usage) {
        case 0x01:
            printf("🖱️  Left Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x02:
            printf("🖱️  Right Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x03:
            printf("🖱️  Middle Button: %s\n", int_value ? "Pressed" : "Released");
            break;
        case 0x30:
            printf("➡️  Move X: %d\n", int_value);
            break;
        case 0x31:
            printf("⬆️  Move Y: %d\n", int_value);
            break;
        case 0x38:
            printf("🌀 Scroll: %d\n", int_value);
            break;
        default:
            printf("Usage: 0x%x | Length: %ld | Data: ", usage, length);
            for (CFIndex i = 0; i < length; ++i) {
                printf("%02x ", data[i]);
            }
            printf("\n");
            break;
        }

    logger_log(usage, length, data);
}

int main() {
    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!manager) {
        fprintf(stderr, "Failed to create IOHIDManager\n");
        return 1;
    }

    CFDictionaryRef matchDict = CreateMatchingDictionary(VENDOR_ID, PRODUCT_ID);
    IOHIDManagerSetDeviceMatching(manager, matchDict);
    CFRelease(matchDict);

    IOHIDManagerRegisterInputValueCallback(manager, HadnleInput, NULL);
    IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    IOReturn openResult = IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone);
    if (openResult != kIOReturnSuccess) {
        fprintf(stderr, "Failed to open HID Manager\n");
        CFRelease(manager);
        return 2;
    }

    logger_init("log.csv");

    CFSetRef deviceSet = IOHIDManagerCopyDevices(manager);
    if (deviceSet) {
        CFIndex deviceCount = CFSetGetCount(deviceSet);
        printf("🟢 Found %ld device(s)\n", deviceCount);

        IOHIDDeviceRef *devices = malloc(sizeof(IOHIDDeviceRef) * deviceCount);
        CFSetGetValues(deviceSet, (const void **)devices);

        for (CFIndex i = 0; i < deviceCount; ++i) {
            IOHIDDeviceRef device = devices[i];
            CFStringRef product = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
            CFStringRef manufacturer = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDManufacturerKey));
            int vendor = 0, product_id = 0;

            CFNumberRef vidRef = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
            CFNumberRef pidRef = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));
            if (vidRef) CFNumberGetValue(vidRef, kCFNumberIntType, &vendor);
            if (pidRef) CFNumberGetValue(pidRef, kCFNumberIntType, &product_id);

            char productStr[256] = {0}, manufacturerStr[256] = {0};
            if (product) CFStringGetCString(product, productStr, sizeof(productStr), kCFStringEncodingUTF8);
            if (manufacturer) CFStringGetCString(manufacturer, manufacturerStr, sizeof(manufacturerStr), kCFStringEncodingUTF8);

            printf("🔹 Device %ld: %s by %s (VID: 0x%04x, PID: 0x%04x)\n",
                   i + 1, productStr, manufacturerStr, vendor, product_id);
        }

        free(devices);
        CFRelease(deviceSet);
    } else {
        printf("🔴 No devices found\n");
    }

    printf("Listening HID-device...\n");
    CFRunLoopRun();

    IOHIDManagerClose(manager, kIOHIDOptionsTypeNone);
    CFRelease(manager);

    logger_close();

    return 0;
}

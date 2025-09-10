#include <stdio.h>
#include <stdlib.h>
#include <IOKit/hid/IOHIDManager.h>
#include <CoreFoundation/CoreFoundation.h>
#include "logger.h"
#include "hid_utils.h"

#define VENDOR_ID  0x03f0
#define PRODUCT_ID 0x0b97

// Create a matching dictionary for vendor and product ID
static CFMutableDictionaryRef create_matching_dict(int vendorID, int productID) {
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(
        kCFAllocatorDefault, 0,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );

    CFNumberRef vid = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &vendorID);
    CFNumberRef pid = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &productID);

    CFDictionarySetValue(dict, CFSTR(kIOHIDVendorIDKey), vid);
    CFDictionarySetValue(dict, CFSTR(kIOHIDProductIDKey), pid);

    CFRelease(vid);
    CFRelease(pid);

    return dict;
}

// Callback for each HID input event
static void handle_input(void *context, IOReturn result, void *sender, IOHIDValueRef value) {
    IOHIDElementRef element = IOHIDValueGetElement(value);
    uint32_t usage = IOHIDElementGetUsage(element);
    CFIndex length = IOHIDValueGetLength(value);
    const uint8_t *data = IOHIDValueGetBytePtr(value);
    int int_value = IOHIDValueGetIntegerValue(value);

    switch (usage) {
        case 0x24:
            parser_usage_0x24(data, length);
            break;
        case 0xFFFFFFFF:
            parser_vendor_usage(data, length);
            break;
        default:
            parser_standart_usage(usage, int_value, length, data);
            break;
    }

    logger_log(usage, length, data);
}

int main(void) {
    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!manager) {
        fprintf(stderr, "Failed to create IOHIDManager\n");
        return EXIT_FAILURE;
    }

    CFDictionaryRef matchDict = create_matching_dict(VENDOR_ID, PRODUCT_ID);
    IOHIDManagerSetDeviceMatching(manager, matchDict);
    CFRelease(matchDict);

    IOHIDManagerRegisterInputValueCallback(manager, handle_input, NULL);
    IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    if (IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
        fprintf(stderr, "Failed to open HID Manager\n");
        CFRelease(manager);
        return EXIT_FAILURE;
    }

    logger_init("log.csv");

    CFSetRef deviceSet = IOHIDManagerCopyDevices(manager);
    if (deviceSet) {
        CFIndex deviceCount = CFSetGetCount(deviceSet);
        printf("\U0001F7E2 Found %ld device(s)\n", deviceCount);

        IOHIDDeviceRef *devices = malloc(sizeof(IOHIDDeviceRef) * deviceCount);
        if (devices) {
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

                char productStr[256] = "", manufacturerStr[256] = "";
                if (product) CFStringGetCString(product, productStr, sizeof(productStr), kCFStringEncodingUTF8);
                if (manufacturer) CFStringGetCString(manufacturer, manufacturerStr, sizeof(manufacturerStr), kCFStringEncodingUTF8);

                printf("\U0001F539 Device %ld: %s by %s (VID: 0x%04x, PID: 0x%04x)\n",
                       i + 1, productStr, manufacturerStr, vendor, product_id);
            }

            free(devices);
        }
        CFRelease(deviceSet);
    } else {
        printf("\U0001F534 No devices found\n");
    }

    printf("Listening HID-device...\n");
    CFRunLoopRun();

    IOHIDManagerClose(manager, kIOHIDOptionsTypeNone);
    CFRelease(manager);
    logger_close();

    return EXIT_SUCCESS;
}


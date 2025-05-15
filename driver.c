// just a basic driver which detects my testing mosue

#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

const GUID GUID_DEVCLASS_USB = { 0x36fc9e60, 0xc465, 0x11cf, {0x80,0x56,0x44,0x45,0x53,0x54,0x00,0x00} };

int main() {
    HDEVINFO deviceInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB,
        NULL,
        NULL,
        DIGCF_PRESENT
    );

    if (deviceInfo == INVALID_HANDLE_VALUE) {
        printf("Failed to get device info set.\n");
        return 1;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD index = 0;
    BOOL found = FALSE;

    while (SetupDiEnumDeviceInfo(deviceInfo, index, &deviceInfoData)) {
        DWORD dataType, dataSize = 0;

        SetupDiGetDeviceRegistryProperty(
            deviceInfo,
            &deviceInfoData,
            SPDRP_HARDWAREID,
            &dataType,
            NULL,
            0,
            &dataSize
        );

        if (dataSize == 0) {
            index++;
            continue;
        }

        BYTE* buffer = (BYTE*)malloc(dataSize);
        if (!buffer) {
            printf("Memory allocation failed.\n");
            break;
        }

        if (SetupDiGetDeviceRegistryProperty(
                deviceInfo,
                &deviceInfoData,
                SPDRP_HARDWAREID,
                &dataType,
                buffer,
                dataSize,
                NULL
            )) {
            char* hardwareId = (char*)buffer;

            if (strstr(hardwareId, "VID_18F8") && strstr(hardwareId, "PID_0FC0")) {
                printf("Mouse with VID_18F8 & PID_0FC0 is connected.\n");
                found = TRUE;
                free(buffer);
                break;
            }
        }
        free(buffer);
        index++;
    }

    if (!found) {
        printf("Mouse not found.\n");
    }

    SetupDiDestroyDeviceInfoList(deviceInfo);
    return 0;
}

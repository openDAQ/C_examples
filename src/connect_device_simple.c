// An example

#include <connect_device.h>

int main()
{
    daqInstance* instance = NULL;
    createInstance(&instance);

    daqList* availableDevices = NULL;
    discoverDevices(&availableDevices, instance);

    daqDevice* connectedDevice = NULL;
    connectDevice(availableDevices, instance, &connectedDevice);

    daqDeviceInfo* devInfo = NULL;
    daqDevice_getInfo(connectedDevice, &devInfo);

    daqString* name = NULL;
    daqDeviceInfo_getName(devInfo, &name);

    daqConstCharPtr* name_constChar = NULL;
    daqString_getCharPtr(name, name_constChar);

    if (name_constChar == NULL)
    {
        printf("Device failed to connect.\r\n");
    }
    else
    {
        printf("Device with the name %s is connected.\r\n", *name_constChar);
    }

    daqReleaseRef(name_constChar);
    daqReleaseRef(name);
    daqReleaseRef(devInfo);
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);

    return 0;
}
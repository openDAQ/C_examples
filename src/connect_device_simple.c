#include <connect_device.h>

// To connect to a device in openDAQ, we first need a connection string that will give us an address where the device can be found (IPv4, IPv6, ...) and a protocol through which we can connect to it (native, OPCUA, ...)
// This connection string can be found within the device info object stored on the device and can be accessed when the device is discovered.

int main()
{
    daqInstance* instance = NULL;
    createInstanceWithDefaultParameters(&instance);

    daqDeviceInfo* firstAvailableDeviceInfo = NULL;
    getDeviceInfoFromFirstDevice(instance, &firstAvailableDeviceInfo);

    daqString* connectionString = NULL;
    daqDeviceInfo_getConnectionString(firstAvailableDeviceInfo, &connectionString);

    daqDevice* instanceRootDevice = NULL;
    daqInstance_getRootDevice(instance, &instanceRootDevice);

    daqDevice* connectedDevice = NULL;
    daqDevice_addDevice(instanceRootDevice, &connectedDevice, connectionString, NULL);

    daqDeviceInfo* connectedDeviceInfo = NULL;
    daqDevice_getInfo(connectedDevice, &connectedDeviceInfo);

    daqString* connectedDeviceName = NULL;
    daqDeviceInfo_getName(connectedDeviceInfo, &connectedDeviceName);

    daqString* availableDeviceName = NULL;
    daqDeviceInfo_getName(firstAvailableDeviceInfo, &availableDeviceName);

    daqConstCharPtr connectedDeviceNameConstChar = NULL;
    daqString_getCharPtr(connectedDeviceName, &connectedDeviceNameConstChar);

    daqConstCharPtr availableDeviceNameConstChar = NULL;
    daqString_getCharPtr(availableDeviceName, &availableDeviceNameConstChar);

    if (!strcmp(connectedDeviceNameConstChar, availableDeviceNameConstChar))
    {
        printf("Device failed to connect.\r\n");
    }
    else
    {
        printf("Device with the name %s is connected.\r\n");
    }

    daqReleaseRef(connectedDeviceInfo);
    daqReleaseRef(connectedDevice);
    daqReleaseRef(instanceRootDevice);
    daqReleaseRef(connectionString);
    daqReleaseRef(firstAvailableDeviceInfo);
    daqReleaseRef(instance);

    return 0;
}
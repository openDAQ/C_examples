#include <copendaq.h>
#include <stdio.h>
#include "discover_devices_simple.c"



void connectDevice(daqList* listOfAvailableDevices, daqInstance* instance, daqDevice* device)
{
    // In the simple example we will connect to 
    daqIterator* iterator = NULL;
    daqList_createStartIterator(listOfAvailableDevices, &iterator);

    daqDeviceInfo* deviceInfo = NULL;
    daqIterator_getCurrent(iterator, (daqBaseObject**)&deviceInfo);

    if (deviceInfo == NULL)
    {
        printf("Device not found\r\n");
        daqReleaseRef(iterator);
        daqReleaseRef(deviceInfo);
        return 0;
    }

    daqString* connectionString = NULL;
    daqDeviceInfo_getConnectionString(deviceInfo, &connectionString);

    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);

    daqDevice_addDevice(rootDevice, &device, connectionString, NULL);

    daqReleaseRef(iterator);
    daqReleaseRef(deviceInfo);
    daqReleaseRef(connectionString);
    daqReleaseRef(rootDevice);
}

int main()
{
    daqInstance* instance = NULL;
    createInstance(instance);

    daqList* availableDevices = NULL;
    discoverDevices(availableDevices, instance);

    daqDevice* connectedDevice = NULL;
    connectDevice(availableDevices, instance, connectedDevice);

    // TODO: Display the device info...

    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);

    return 0;
}
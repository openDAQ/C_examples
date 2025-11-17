#include <copendaq.h>
#include <stdio.h>
#include "discover_devices_simple.c"

void connectDevice(daqList* listOfAvailableDevices, daqInstance* instance, daqDevice* device)
{
    // In the simple example we will connect to 
    daqIterator* iterator = NULL;
    daqList_createStartIterator(listOfAvailableDevices, &iterator);

    // To startup the iteration
    if (daqIterator_moveNext(iterator) != DAQ_SUCCESS)
    {
        printf("Failed to initialize the iterator when looking for the available device!\r\n");
        daqReleaseRef(iterator);
        return;
    }

    daqDeviceInfo* deviceInfo = NULL;
    daqIterator_getCurrent(iterator, (daqBaseObject**)&deviceInfo);

    if (deviceInfo == NULL)
    {
        printf("Device not found\r\n");
        daqReleaseRef(iterator);
        daqReleaseRef(deviceInfo);
        return;
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
    createInstance(&instance);

    daqList* availableDevices = NULL;
    discoverDevices(&availableDevices, instance);

    daqDevice* connectedDevice = NULL;
    connectDevice(availableDevices, instance, connectedDevice);

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
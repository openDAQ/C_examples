#include <discover_devices.h>

void connectDevice(daqList* listOfAvailableDevices, daqInstance* instance, daqDevice** device)
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
        return;
    }

    daqString* connectionString = NULL;
    daqDeviceInfo_getConnectionString(deviceInfo, &connectionString);

    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);

    daqDevice_addDevice(rootDevice, device, connectionString, NULL);

    daqReleaseRef(iterator);
    daqReleaseRef(deviceInfo);
    daqReleaseRef(connectionString);
    daqReleaseRef(rootDevice);
}

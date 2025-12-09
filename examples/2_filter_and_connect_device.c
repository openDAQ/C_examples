/*
 * In this example we will search through the available devices to find the first one that has its 
 * manufacturer set to 'openDAQ'.Then connect the device into our component tree as a child device.
 */

#include <daq_utils.h>

int main(void)
{
    daqInstance* instance = NULL;
    daqList* availableDevices = NULL;

    createInstance(&instance, "");

    daqDevice_getAvailableDevices((daqDevice*)instance, &availableDevices);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);

    daqDevice* connectedDevice = NULL;

    uint8_t connected = 0;

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDevInfo = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentDevInfo);

        daqString* manufacturer = NULL;
        daqDeviceInfo_getManufacturer(currentDevInfo, &manufacturer);

        daqConstCharPtr manufacturer_constChar = NULL;
        daqString_getCharPtr(manufacturer, &manufacturer_constChar);

        int result = strcmp(manufacturer_constChar, "openDAQ");

        if (!result)
        {
            daqString* connectionString = NULL;
            daqDeviceInfo_getConnectionString(currentDevInfo, &connectionString);

            daqDevice_addDevice((daqDevice*)instance, &connectedDevice, connectionString, NULL);
            daqReleaseRef(connectionString);

            connected = 1;
        }

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(manufacturer);

        if (connected)
            break;
    }

    if (connected) 
    {
        daqDeviceInfo* connectedDeviceInfo = NULL;
        daqDevice_getInfo(connectedDevice, &connectedDeviceInfo);

        daqString* name = NULL;
        daqDeviceInfo_getName(connectedDeviceInfo, &name);

        printDaqFormattedString("The name fo the connected device is:", name);

        daqReleaseRef(name);
        daqReleaseRef(connectedDevice);
    }

    daqReleaseRef(availableDevices);
    daqReleaseRef(instance);

    return 0;
}

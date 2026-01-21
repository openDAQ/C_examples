/*
 * This example demonstrated how to create an openDAQ instance and detect devices.
 * Then it goes through all detected devices and displays their name and manufacturer.
 */

#include <daq_utils.h>


int main(void)
{
    daqInstanceBuilder* builder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&builder);
    
    daqString* modulePath = NULL;
    daqString_createString(&modulePath, MODULE_PATH);
    daqInstanceBuilder_setModulePath(builder, modulePath);
    
    daqInstanceBuilder_setGlobalLogLevel(builder, daqLogLevelOff);

    daqInstance* instance = NULL;
    daqInstance_createInstanceFromBuilder(&instance, builder);

    daqList* availableDevices = NULL;
    daqDevice_getAvailableDevices((daqDevice*)instance, &availableDevices);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDevInfo = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentDevInfo);

        daqString* name = NULL;
        daqDeviceInfo_getName(currentDevInfo, &name);

        daqString* manufacturer = NULL;
        daqDeviceInfo_getManufacturer(currentDevInfo, &manufacturer);

        printDaqFormattedString("Name of the device: %s\n", name);
        printDaqFormattedString("Manufacturer of the device: %s\n", manufacturer);

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(name);
        daqReleaseRef(manufacturer);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableDevices);
    daqReleaseRef(instance);
    daqReleaseRef(builder);

    return 0;
}


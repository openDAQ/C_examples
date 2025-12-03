/*
 * This example demonstrated how to create an openDAQ instance and discover all available devices for connection
 */

#include <daq_utils.h>


int main(void)
{
    daqInstanceBuilder* builder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&builder);

    daqInstance* instance = NULL;
    daqInstance_createInstanceFromBuilder(&instance, builder);

    daqList* availableDevices = NULL;
    daqDevice_getAvailableDevices((daqDevice*)instance, &availableDevices);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);
   
    // Go through all detected devices and print out their respective domain info
    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDevInfo = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentDevInfo);

        daqString* name = NULL;
        daqDeviceInfo_getName(currentDevInfo, &name);

        daqString* manufacturer = NULL;
        daqDeviceInfo_getManufacturer(currentDevInfo, &manufacturer);

        daqConstCharPtr name_constChar = NULL;
        daqString_getCharPtr(name, &name_constChar);

        daqConstCharPtr manufacturer_constChar = NULL;
        daqString_getCharPtr(manufacturer, &manufacturer_constChar);

        printf("Name of the device: %s\n", name_constChar);
        printf("Manufacturer of the device: %s\n", manufacturer_constChar);

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


#include <daq_utils.h>

// TODO: Move up
// This example demonstrated how to create an openDAQ instance and discover all available devices for connection

int main(void)
{
    daqInstanceBuilder* builder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&builder);

    // The created Instance gets its settings from the builder that was used to create it
    daqInstance* instance = NULL;
    daqInstance_createInstanceFromBuilder(&instance, builder);

    // Discovering devices:

    // Calling the function that discovers all the available devices for connection
    daqList* availableDevices = NULL;
    daqDevice_getAvailableDevices((daqDevice*)instance, &availableDevices);

    // Due to the nature of the C language we need to create an Iterator object when traversing the daqList object
    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);
 
    // TODO: A bit too many comments
    // OpenDAQ iterators need to be "booted up" with a daqIterator_moveNext call as they do not 
    // start pointing to the first element of the list in the beginning.
   
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


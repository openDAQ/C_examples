#include <discover_devices.h>

// To discover available devices that your instance can connect to, the instance itself has an internal root device that has an available function called getAvailableDevices.
// In this example we will demonstrate how to access it and create a list of all available devices that you can connect to and display device connection candidate's name and manufacturer.

int main(void)
{
    daqInstance* instance = NULL;
    createInstanceWithDefaultParameters(&instance);

    // Discovering devices:
    
    // Calling the function that discovers all the available devices for connection
    daqList* availableDevices = NULL;
    daqDevice_getAvailableDevices((daqDevice*) instance, &availableDevices);

    // Due to the nature of the C language we need to create an Iterator object when traversing the daqList object
    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);

    // Note on iterator objects and their behaviour in openDAQ.
    /*
        OpenDAQ iterators need to be "booted up" with a daqIterator_moveNext call as they do not start pointing to the first element of the list in the beginning.
    */

    // Go through all detected devices and print out their respective domain info
    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDevInfo = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentDevInfo);
        
        daqString* name = NULL;
        daqDeviceInfo_getName(currentDevInfo, &name);

        daqString* manufacturer = NULL;
        daqDeviceInfo_getManufacturer(currentDevInfo, &manufacturer);

        daqConstCharPtr nameConstChar = NULL;
        daqString_getCharPtr(name, &nameConstChar);

        daqConstCharPtr manufacturer_constChar = NULL;
        daqString_getCharPtr(manufacturer, &manufacturer_constChar);

        printf("Name of the device: %s\n", nameConstChar);
        printf("Manufacturer of the device: %s\n", manufacturer_constChar);

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(name);
        daqReleaseRef(manufacturer);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableDevices);
    daqReleaseRef(instance);

    return 0;
}
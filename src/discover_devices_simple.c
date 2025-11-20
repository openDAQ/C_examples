// An example that looks through the available devices that are discovered

#include <discover_devices.h>

int main()
{
    daqInstance* instance = NULL;
    createInstance(&instance);

    daqList* availableDevices = NULL;
    discoverDevices(&availableDevices, instance);

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
        printf("Manufactorer of the device: %s\n", manufacturer_constChar);

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(name);
        daqReleaseRef(manufacturer);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableDevices);
    daqReleaseRef(instance);

    return 0;
}
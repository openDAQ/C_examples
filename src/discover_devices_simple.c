#include <copendaq.h>
#include <stdio.h>
#include <build_instance.c>

void discoverDevices(daqList* listOfDevices, daqInstance* instance) 
{
    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);
    daqDevice_getAvailableDevices(rootDevice, &listOfDevices);

    daqReleaseRef(rootDevice);
}

int main()
{
    daqInstance* instance = NULL;
    createInstance(instance);

    daqList* availableDevices = NULL;
    discoverDevices(availableDevices, instance);

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

        daqConstCharPtr* name_constChar = NULL;
        daqString_getCharPtr(name, &name_constChar);

        daqConstCharPtr* manufacturer_constChar = NULL;
        daqString_getCharPtr(manufacturer, &name_constChar);

        printf("Name of the device: %s\n", name_constChar);
        printf("Manufactorer of the device: %s\n", manufacturer_constChar);

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(name);
        daqReleaseRef(manufacturer);
        daqReleaseRef(name_constChar);
        daqReleaseRef(manufacturer_constChar);
    }

    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(iterator);

    return 0;
}
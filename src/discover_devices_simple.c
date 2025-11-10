#include <copendaq.h>
#include <./build_instance.c>

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


    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);

    return 0;
}
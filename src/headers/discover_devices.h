#include <build_instance.h>

void discoverDevices(daqList** listOfDevices, daqInstance* instance)
{
    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);
    daqDevice_getAvailableDevices(rootDevice, listOfDevices);

    daqReleaseRef(rootDevice);
}
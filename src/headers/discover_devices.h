#include <build_instance.h>

void discoverDevices(daqList** listOfDevices, daqInstance* instance)
{
    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);

    daqDevice_getAvailableDevices(rootDevice, listOfDevices);

    daqReleaseRef(rootDevice);
}

void getInfoToFirstDevice(daqInstance* instance, daqDevice** device)
{
    daqDevice* rootDevice = DAQ_BORROW_INTERFACE(instance, DAQ_DEVICE_INTF_ID);
    //daqInstance_getRootDevice(instance, &rootDevice);

    daqList* listOfDevices = NULL;
    daqDevice_getAvailableDevices(rootDevice, &listOfDevices);

    daqList_popFront(listOfDevices, (daqBaseObject**)device);



    //daqReleaseRef(rootDevice);
    daqReleaseRef(listOfDevices);
}
#include <build_instance.h>

// Method for creating a list of all available devices for connection to the instance
void discoverDevicesFromInstance(daqList** listOfDevices, daqInstance* instance)
{
    daqDevice* rootDevice = NULL;
    daqInstance_getRootDevice(instance, &rootDevice);

    daqDevice_getAvailableDevices(rootDevice, listOfDevices);

    daqReleaseRef(rootDevice);
}

// Method for recieving the DeviceInfo of the first available device in the list of all available devices from a given instance
void getDeviceInfoFromFirstDevice(daqInstance* instance, daqDevice** device)
{
    daqDevice* rootDevice = DAQ_BORROW_INTERFACE(instance, DAQ_DEVICE_INTF_ID);

    daqList* listOfDevices = NULL;
    daqDevice_getAvailableDevices(rootDevice, &listOfDevices);

    daqList_popFront(listOfDevices, (daqBaseObject**)device);

    daqReleaseRef(listOfDevices);
}
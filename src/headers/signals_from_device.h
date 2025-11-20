#include <connect_device.h>
#include <copendaq.h>

void signalsFromDevices(daqDevice* device, daqSignal** signal)
{
    daqPropertyObject_setPropertyValue((daqPropertyObject*)device, NULL, NULL);

    // Connect to the device and connect the signal
    daqList* signals = NULL;
    
    // The recursive search though all the signals available in a device
    daqDevice_getSignalsRecursive(device, &signals, NULL);

    daqSignal* outSig = NULL;

    daqSizeT numOfSignals = 0;
    daqList_getCount(signals, &numOfSignals);


    if (numOfSignals < 1)
    {
        daqDeviceInfo* deviceInfo = NULL;
        daqDevice_getInfo(device, &deviceInfo);

        daqString* nameDevice = NULL;
        daqDeviceInfo_getName(deviceInfo, &nameDevice);

        daqConstCharPtr nameDeviceConstChar = NULL;
        daqString_getCharPtr(nameDevice, &nameDeviceConstChar);

        printf("There are no signals available in the device %s\n", nameDeviceConstChar);

        daqReleaseRef(deviceInfo);
        daqReleaseRef(nameDevice);
        daqReleaseRef(signals);
        return;
    }

    // We take the first available signal that we recieve from the connected device
    daqList_popFront(signals, (daqBaseObject**) & outSig);

    *signal = outSig;

    daqReleaseRef(signals);

}


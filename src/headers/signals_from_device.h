#include <connect_device.h>

void signalsFromDevices(daqDevice* device, daqSignal** signal)
{
    // Connect to the device and connect the signal
    daqList* signals = NULL;
    daqDevice_getSignals(device, &signals, NULL);

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

    daqList_getItemAt(signals, 0, (daqBaseObject**)&outSig);

    // Identical to above
    //daqList_popFront(signals, (daqBaseObject**) & outSig);

    signal = outSig;

    daqReleaseRef(signals);
    
    // We will need the object down the line
    // daqReleaseRef(outSig);

}


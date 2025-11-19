#include <connect_device.h>

void signalsFromDevices(daqDevice* device, daqSignal** signal)
{
    // Connect to the device and connect the signal
    daqList* signals = NULL;
    daqDevice_getSignals(device, &signals, NULL);

    daqIterator* iterator = NULL;
    
    daqList_createStartIterator(signals, &iterator);

    daqSignal* outSig = NULL;

    daqList_popFront(signals, (daqBaseObject**) & outSig);

    signal = outSig;

    if (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currSig = NULL;
        daqIterator_getCurrent(iterator, &currSig);

        signal = &currSig;
    }

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, &currentSignal);

        daqDataDescriptor* dataDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &dataDescriptor);

        daqString* name = NULL;
        daqDataDescriptor_getName(currentSignal, &name);

        daqConstCharPtr name_constChar = NULL;
        daqString_getCharPtr(name, &name_constChar);

        printf("Name of the signal: %s\n", name_constChar);

        //signal = &currentSignal;

        daqReleaseRef(currentSignal);
        daqReleaseRef(name);
    }
}

